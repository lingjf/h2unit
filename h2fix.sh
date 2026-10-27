#!/bin/bash

# h2unit patches code pages at runtime. macOS never permits a direct W+X
# transition, so __TEXT must advertise write in its maximum protection.
# Patch the Mach-O before signing: changing the header afterwards invalidates
# the code signature, and Apple Silicon kills the process at launch.

set -e

if test "$(uname)" != 'Darwin'; then
    exit 0
fi

if test -z "$1"; then
    echo "usage: $0 <mach-o executable>" >&2
    exit 2
fi

/usr/bin/python3 - "$1" <<'PY'
import struct
import sys

path = sys.argv[1]
with open(path, "rb") as f:
    data = bytearray(f.read())

LC_SEGMENT = 0x1
LC_SEGMENT_64 = 0x19
VM_PROT_WRITE = 0x2
changed = False


def patch_macho(offset, size):
    global changed
    if offset < 0 or size < 28 or offset + size > len(data):
        raise ValueError("invalid Mach-O slice")

    magic_le = struct.unpack_from("<I", data, offset)[0]
    magic_be = struct.unpack_from(">I", data, offset)[0]
    if magic_le in (0xFEEDFACE, 0xFEEDFACF):
        endian = "<"
        magic = magic_le
    elif magic_be in (0xFEEDFACE, 0xFEEDFACF):
        endian = ">"
        magic = magic_be
    else:
        raise ValueError("not a Mach-O image")

    is_64 = magic == 0xFEEDFACF
    header_size = 32 if is_64 else 28
    ncmds, sizeofcmds = struct.unpack_from(endian + "II", data, offset + 16)
    command = offset + header_size
    command_end = command + sizeofcmds
    if command_end > offset + size:
        raise ValueError("truncated Mach-O load commands")

    for _ in range(ncmds):
        if command + 8 > command_end:
            raise ValueError("truncated Mach-O load command")
        cmd, cmdsize = struct.unpack_from(endian + "II", data, command)
        if cmdsize < 8 or command + cmdsize > command_end:
            raise ValueError("invalid Mach-O load command size")
        if cmd == (LC_SEGMENT_64 if is_64 else LC_SEGMENT):
            segment_name = bytes(data[command + 8:command + 24]).split(b"\0", 1)[0]
            if segment_name == b"__TEXT":
                maxprot_offset = command + (56 if is_64 else 40)
                maxprot = struct.unpack_from(endian + "I", data, maxprot_offset)[0]
                writable_maxprot = maxprot | VM_PROT_WRITE
                if writable_maxprot != maxprot:
                    struct.pack_into(endian + "I", data, maxprot_offset, writable_maxprot)
                    changed = True
        command += cmdsize


magic = bytes(data[:4])
if magic in (b"\xca\xfe\xba\xbe", b"\xca\xfe\xba\xbf"):
    is_64 = magic == b"\xca\xfe\xba\xbf"
    nfat = struct.unpack_from(">I", data, 4)[0]
    entry_size = 32 if is_64 else 20
    for index in range(nfat):
        entry = 8 + index * entry_size
        if is_64:
            offset, size = struct.unpack_from(">QQ", data, entry + 8)
        else:
            offset, size = struct.unpack_from(">II", data, entry + 8)
        patch_macho(offset, size)
else:
    patch_macho(0, len(data))

if changed:
    with open(path, "r+b") as f:
        f.write(data)
PY

# Apple Silicon requires a valid signature after the Mach-O header change.
codesign --force --sign - "$1"
