#!/usr/bin/env python3
"""Renames the files in a folder whose names are longer than FATX allows to the short names Mono looks for on xsdk.

Usage: short_names.py <folder>
mono/utils/mono-xsdk.c's mono_xsdk_short_file_name makes the same names: the name's start, '~', 8 hex digits of a
32-bit FNV-1a hash of the name in lower case, then the extension, 42 characters in all.
"""
import sys
from pathlib import Path

FILE_NAME_MAX = 42


def short_name(name):
    extension = name[name.rfind("."):] if "." in name else ""
    hash_value = 2166136261
    for byte in name.lower().encode("latin-1"):
        hash_value = ((hash_value ^ byte) * 16777619) & 0xFFFFFFFF
    if len(extension) > FILE_NAME_MAX - 10:
        extension = ""
    return f"{name[:FILE_NAME_MAX - len(extension) - 9]}~{hash_value:08x}{extension}"


def main():
    folder = Path(sys.argv[1])
    for path in sorted(folder.iterdir()):
        if path.is_file() and len(path.name) > FILE_NAME_MAX:
            target = path.with_name(short_name(path.name))
            path.rename(target)
            print(f"{path.name} -> {target.name}")


if __name__ == "__main__":
    main()
