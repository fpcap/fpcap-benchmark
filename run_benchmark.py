import argparse
import json
import os
import platform
import subprocess
import sys
from collections import defaultdict
from datetime import date

import matplotlib.pyplot as plt


LIBRARY_COLORS = {
    "fpcap (mmap)": "#ee4d2e",
    "fpcap (fread)": "#ee4d2e",
    "libpcap": "#aaaaaa",
    "PcapPlusPlus": "#777777",
}

# Rename libraries from benchmark names to display names
LIBRARY_DISPLAY_NAMES = {
    "fpcap": "fpcap (mmap)",
    "fpcap-fread": "fpcap (fread)",
}

# Normalize zst/zstd formats into a single group
FORMAT_ALIASES = {
    "pcapng.zst": "pcapng.zst(d)",
    "pcapng.zstd": "pcapng.zst(d)",
}

FORMAT_ORDER = ["pcap", "pcapng", "pcapng.zst(d)"]

TIME_UNIT_TO_MS = {
    "ns": 1e-6,
    "us": 1e-3,
    "ms": 1.0,
    "s": 1e3,
}


def _get_os_name():
    """Return the OS name and version."""
    try:
        if sys.platform == "win32":
            out = subprocess.check_output(
                ["wmic", "os", "get", "Caption,Version", "/value"],
                text=True, stderr=subprocess.DEVNULL,
            )
            fields = {}
            for line in out.splitlines():
                if "=" in line:
                    k, v = line.split("=", 1)
                    fields[k.strip()] = v.strip()
            caption = fields.get("Caption", "")
            version = fields.get("Version", "")
            if caption:
                return f"{caption} ({version})" if version else caption
    except Exception:
        pass
    return f"{platform.system()} {platform.release()} ({platform.version()})"


def _get_cpu_model():
    """Return the CPU model string."""
    try:
        if sys.platform == "win32":
            out = subprocess.check_output(
                ["wmic", "cpu", "get", "Name", "/value"],
                text=True, stderr=subprocess.DEVNULL,
            )
            for line in out.splitlines():
                if line.startswith("Name="):
                    return line.split("=", 1)[1].strip()
        elif sys.platform == "linux":
            with open("/proc/cpuinfo") as f:
                for line in f:
                    if line.startswith("model name"):
                        return line.split(":", 1)[1].strip()
        elif sys.platform == "darwin":
            return subprocess.check_output(
                ["sysctl", "-n", "machdep.cpu.brand_string"],
                text=True, stderr=subprocess.DEVNULL,
            ).strip()
    except Exception:
        pass
    return platform.processor() or "unknown"


def _get_ram_total():
    """Return total RAM as a human-readable string."""
    try:
        if sys.platform == "win32":
            out = subprocess.check_output(
                ["wmic", "computersystem", "get", "TotalPhysicalMemory", "/value"],
                text=True, stderr=subprocess.DEVNULL,
            )
            for line in out.splitlines():
                if line.startswith("TotalPhysicalMemory="):
                    total = int(line.split("=", 1)[1].strip())
                    return f"{total / (1024 ** 3):.1f} GB"
        elif sys.platform == "linux":
            with open("/proc/meminfo") as f:
                for line in f:
                    if line.startswith("MemTotal:"):
                        kb = int(line.split()[1])
                        return f"{kb / (1024 ** 2):.1f} GB"
        elif sys.platform == "darwin":
            out = subprocess.check_output(
                ["sysctl", "-n", "hw.memsize"],
                text=True, stderr=subprocess.DEVNULL,
            ).strip()
            return f"{int(out) / (1024 ** 3):.1f} GB"
    except Exception:
        pass
    return "unknown"


def check_file_locked(path):
    """On Windows, check if the executable is locked by another process."""
    if sys.platform != "win32":
        return
    try:
        with open(path, "ab"):
            pass
    except PermissionError:
        print(f"Error: '{path}' is locked by another process. "
              "Close any programs using it (debugger, IDE, etc.) and retry.",
              file=sys.stderr)
        sys.exit(1)


def run_benchmark(executable):
    """Run the benchmark executable and return parsed JSON output."""
    check_file_locked(executable)
    try:
        result = subprocess.run(
            [executable, "--benchmark_format=json"],
            capture_output=True,
            text=True,
            check=True,
        )
    except FileNotFoundError:
        print(f"Error: executable not found: {executable}", file=sys.stderr)
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"Error: benchmark exited with code {e.returncode}", file=sys.stderr)
        if e.stderr:
            print(e.stderr, file=sys.stderr)
        sys.exit(1)

    return json.loads(result.stdout)


def parse_benchmarks(data):
    """Parse benchmark JSON into {format: {library: time_ms}} structure."""
    grouped = defaultdict(dict)

    for bm in data["benchmarks"]:
        name = bm["name"]
        real_time = bm["real_time"]
        time_unit = bm["time_unit"]

        # Convert to milliseconds
        time_ms = real_time * TIME_UNIT_TO_MS[time_unit]

        # Parse "library (format)" pattern
        parts = name.split(" (", 1)
        if len(parts) != 2 or not parts[1].endswith(")"):
            print(f"Warning: skipping unrecognized benchmark name: {name}",
                  file=sys.stderr)
            continue

        library = parts[0]
        fmt = parts[1][:-1]  # strip trailing ')'

        # Rename libraries to display names
        library = LIBRARY_DISPLAY_NAMES.get(library, library)

        # Normalize zst/zstd
        fmt = FORMAT_ALIASES.get(fmt, fmt)

        grouped[fmt][library] = time_ms

    return grouped


def plot_results(grouped, output_path="benchmark_results.png"):
    """Create a horizontal bar chart grouped by file format."""
    # Collect all libraries that appear in results, preserving a stable order
    all_libraries = []
    for fmt in FORMAT_ORDER:
        if fmt in grouped:
            for lib in grouped[fmt]:
                if lib not in all_libraries:
                    all_libraries.append(lib)

    # Sort formats by FORMAT_ORDER, putting unknown formats at the end
    formats = [f for f in FORMAT_ORDER if f in grouped]
    formats += [f for f in grouped if f not in FORMAT_ORDER]

    fig, ax = plt.subplots(figsize=(10, max(4, len(formats) * len(all_libraries) * 0.4)))

    bar_height = 0.6
    group_gap = 1.0
    y_positions = []
    y_labels = []
    y_offset = 0

    for fmt in reversed(formats):  # reversed so first format is at top
        libs_in_fmt = grouped[fmt]
        libs = [l for l in all_libraries if l in libs_in_fmt]

        for i, lib in enumerate(reversed(libs)):
            y = y_offset + i * bar_height
            time_ms = libs_in_fmt[lib]
            color = LIBRARY_COLORS.get(lib, "#999999")
            bar = ax.barh(y, time_ms, height=bar_height * 0.85, color=color)
            ax.bar_label(bar, fmt="%.2f ms", padding=4, fontsize=8)
            y_positions.append(y)
            y_labels.append(lib)

        # Add format label as a group title above the group
        group_top = y_offset + (len(libs) - 1) * bar_height
        ax.annotate(
            fmt, xy=(0, group_top),
            xytext=(-10, 10), textcoords="offset points",
            ha="right", va="bottom", fontweight="bold", fontsize=10,
        )

        y_offset += len(libs) * bar_height + group_gap

    ax.set_yticks(y_positions)
    ax.set_yticklabels(y_labels)
    ax.set_xlabel("Time (ms) — lower is better")
    ax.set_title("Reading 4631 Packets (3.7mb)")

    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    print(f"Chart saved to {output_path}")


def main():
    parser = argparse.ArgumentParser(
        description="Run fpcap benchmark and produce a comparison chart.")
    parser.add_argument(
        "--executable",
        default="cmake-build-release-visual-studio/fpcap_benchmark.exe",
        help="Path to the benchmark executable (default: %(default)s)",
    )
    args = parser.parse_args()

    # Create output directory: benchmarks/<YYYY-MM-DD>/
    output_dir = os.path.join("benchmarks", date.today().isoformat())
    os.makedirs(output_dir, exist_ok=True)

    print(f"Running benchmark: {args.executable}")
    data = run_benchmark(args.executable)

    # Replace machine-specific fields with system info
    ctx = data.get("context", {})
    ctx.pop("host_name", None)
    ctx.pop("executable", None)
    ctx["os"] = _get_os_name()
    ctx["cpu_model"] = _get_cpu_model()
    ctx["ram_total"] = _get_ram_total()
    json_path = os.path.join(output_dir, "benchmark_results.json")
    with open(json_path, "w") as f:
        json.dump(data, f, indent=2)
    print(f"JSON saved to {json_path}")

    grouped = parse_benchmarks(data)
    if not grouped:
        print("Error: no benchmark results parsed.", file=sys.stderr)
        sys.exit(1)

    print(f"Parsed {sum(len(v) for v in grouped.values())} results "
          f"across {len(grouped)} format(s)")
    image_path = os.path.join(output_dir, "benchmark_results.png")
    plot_results(grouped, image_path)


if __name__ == "__main__":
    main()
