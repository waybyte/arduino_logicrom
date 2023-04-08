# Copyright 2023 Waybyte Solutions
#
# SPDX-License-Identifier: MIT
#

from sys import argv
from os.path import getsize, join, dirname, basename, splitext
from zipfile import ZipFile
from zlib import crc32
from json import load, dump


def makebin(target, source):
    # fix bin size to 32 byte boundary
    binsz = getsize(source)
    f_binsz = (binsz + 0x1F) & ~0x1F
    f = open(source, "rb")
    f_bin = bytearray(f.read())
    f.close()
    f_bin += bytes(f_binsz - binsz)
    # Fix header size
    f_bin[4:8] = f_binsz.to_bytes(4, "little")
    # Fix checksum
    f_bin[8:0xC] = crc32(f_bin).to_bytes(4, "little")
    # write final binary
    f = open(target, "wb")
    f.write(f_bin)
    f.close()


def gen_fota_file(target, source):
    makebin(target, source)


def gen_release_file(mcu, flashsz, imagepath, source_bin, target_firm):
    build_dir = dirname(target_firm)
    progname = splitext(basename(target_firm))[0]
    # Generate image file
    print("Fix Firmware Binary")
    target_img = join(build_dir, progname + '.bin')
    makebin(target_img, source_bin)
    target_binsz = getsize(target_img)
    print("Updating download.json")
    # Load and update download.json
    f = open(join(imagepath, "download.json"), "r")
    download = load(f)
    f.close()
    for cmd in download:
        if cmd["command"] == "require" and cmd["name"] == "product":
            cmd["value"] = "arom-tiny" if mcu == "ASR1603" else "arom|arom-tiny"
        elif cmd["command"] == "require" and cmd["name"] == "version-bootrom":
            cmd["value"] = "2020.07.30" if mcu == "ASR1603" else "2019.01.15"
        elif cmd["command"] == "progress":
            cmd["value"] = target_binsz
        elif cmd["command"] == "flash":
            cmd["weight"] = target_binsz
            cmd["image"] = progname + '.bin'
    f = open(join(build_dir, "download.json"), "w")
    dump(download, f, indent=4)
    f.close()
    # Update partition info, might not be needed
    print("Updating partition_" + flashsz + ".bin")
    f = open(join(imagepath, "partition_" + flashsz + ".bin"), "rb")
    part = bytearray(f.read())
    f.close()
    # Update app size
    part[0x3B8:0x3BC] = target_binsz.to_bytes(4, "little")
    # update CRC
    part[0x888:] = crc32(part[:(len(part) - 4)]).to_bytes(4, "little")
    f = open(join(build_dir, "partition.bin"), "wb")
    f.write(part)
    f.close()
    # Generate zip file for flasher
    print("Creating final zip")
    firm = ZipFile(target_firm, "w")
    firm.write(join(build_dir, "download.json"), "download.json")
    firm.write(join(build_dir, "partition.bin"), "partition.bin")
    firm.write(join(imagepath, mcu, "flasher.img"), "flasher.img")
    firm.write(join(imagepath, "flashinfo_" + flashsz + ".bin"), "flashinfo.bin")
    firm.write(join(imagepath, mcu, "preboot.img"), "preboot.img")
    firm.write(target_img, progname + ".bin")


# arg1 operation
op = argv[1]
if op == "genzip":
    # arg2 - MCU
    mcu = argv[2].upper()
    # arg3 - flash size
    flashsz = argv[3]
    # arg4 - imagepath
    imagepath = argv[4]
    # arg5 - bin
    binfile = argv[5]
    # arg6 - target zip
    target = argv[6]
    gen_release_file(mcu, flashsz, imagepath, binfile, target)
elif op == "genfota":
    gen_fota_file(argv[3], argv[2])
