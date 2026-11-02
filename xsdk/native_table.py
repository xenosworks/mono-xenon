#!/usr/bin/env python3
"""Writes mono/native/pal-xsdk-table.c: every SystemNative_* function the xsdk build of System.Native defines, by name.

Usage: native_table.py <source>... > mono/native/pal-xsdk-table.c
Run it from the repository's root with mono/native/Makefile.am's xsdk sources whenever they change; a function left
out of the table is one P/Invoke can't find.
"""
import re
import sys

# A definition, in corefx's style or Mono's (the return type on a line of its own): the name, its parameters, a brace.
DEFINITION = re.compile(r"\b(SystemNative_\w+)\s*\([^;{)]*\)\s*\{")


def main():
    names = set()
    for path in sys.argv[1:]:
        with open(path, encoding="utf-8", errors="replace") as source:
            names.update(match.group(1) for match in DEFINITION.finditer(source.read()))
    names = sorted(names)
    sources = ", ".join(path.replace("\\", "/").split("/")[-1] for path in sys.argv[1:])
    lines = [
        "/* Written by xsdk/native_table.py from " + sources + ": System.Native by name, for P/Invoke on xsdk. */",
        "",
        "#include <stddef.h>",
        "#include <string.h>",
        "",
        "#include \"mono/utils/mono-xsdk.h\"",
        "",
        "/* Declared alike, as only their addresses are taken here. */",
    ]
    lines += [f"extern void {name} (void);" for name in names]
    lines += ["", "static const struct {", "\tconst char *name;", "\tvoid (*address) (void);", "} functions [] = {"]
    lines += [f'\t{{ "{name}", {name} }},' for name in names]
    lines += [
        "};",
        "",
        "void *",
        "mono_xsdk_native_lookup (const char *name)",
        "{",
        "\tfor (size_t i = 0; i < sizeof (functions) / sizeof (functions [0]); i++) {",
        "\t\tif (strcmp (functions [i].name, name) == 0)",
        "\t\t\treturn (void *)functions [i].address;",
        "\t}",
        "\treturn NULL;",
        "}",
    ]
    sys.stdout.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
