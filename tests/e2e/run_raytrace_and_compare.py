#!/usr/bin/env python3
"""
Helper invoked by CTest to run the compiled raytracer end-to-end and assert
that the produced PNG matches a known-good baseline.
"""

from __future__ import annotations

import argparse
import hashlib
import re
import subprocess
import sys
import textwrap
import time
from pathlib import Path


def sha256sum(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(8192), b""):
            digest.update(chunk)
    return digest.hexdigest()


def parse_runtime(stdout: str) -> float | None:
    match = re.search(r"Total time:\s+([0-9.]+)\s+seconds", stdout)
    if match:
        return float(match.group(1))
    return None


def run(binary: Path, scene: Path, output: Path) -> tuple[float | None, str, str]:
    if output.exists():
        output.unlink()
    output.parent.mkdir(parents=True, exist_ok=True)

    command = [str(binary), str(scene), str(output)]
    start = time.perf_counter()
    proc = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=False,
    )
    wall_time = time.perf_counter() - start

    if proc.returncode != 0:
        message = textwrap.dedent(
            f"""
            Raytracer process failed with exit code {proc.returncode}
            Command: {' '.join(command)}
            ---- STDOUT ----
            {proc.stdout}
            ---- STDERR ----
            {proc.stderr}
            """
        ).strip()
        raise RuntimeError(message)

    runtime = parse_runtime(proc.stdout) or wall_time
    return runtime, proc.stdout.strip(), proc.stderr.strip()


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", required=True, type=Path, help="Path to raytracer executable")
    parser.add_argument("--scene", required=True, type=Path, help="Scene JSON file")
    parser.add_argument("--expected", required=True, type=Path, help="Expected PNG to compare against")
    parser.add_argument("--output", required=True, type=Path, help="Temporary PNG output location")
    parser.add_argument(
        "--allow-mismatch",
        action="store_true",
        help="Do not fail when hashes differ (useful when regenerating baselines)",
    )
    return parser


def main(argv: list[str]) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)

    for path in (args.binary, args.scene, args.expected):
        if not path.exists():
            parser.error(f"{path} does not exist")

    runtime_s, stdout, stderr = run(args.binary, args.scene, args.output)

    actual_hash = sha256sum(args.output)
    expected_hash = sha256sum(args.expected)

    if actual_hash != expected_hash and not args.allow_mismatch:
        print(stdout, file=sys.stdout)
        if stderr:
            print(stderr, file=sys.stderr)
        print(
            textwrap.dedent(
                f"""
                Hash mismatch detected for scene {args.scene}.
                expected: {expected_hash}
                actual:   {actual_hash}
                temporary output kept at: {args.output}
                """
            ).strip(),
            file=sys.stderr,
        )
        return 1

    metrics = {
        "scene": str(args.scene),
        "sha256": actual_hash,
        "render_time_seconds": runtime_s,
    }
    print(f"[metrics] {metrics}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
