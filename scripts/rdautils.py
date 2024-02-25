# Copyright 2024 Waybyte Solutions
#
# SPDX-License-Identifier: MIT
#

from sys import argv
from platform import system
from subprocess import Popen
from os.path import join, getsize, split
from os import remove
from zlib import crc32
from shutil import copyfile

lod_header = bytes("""#$mode=flsh_spi32m
#$sectormap=(16 x 4k, 57 x 64k, 96 x 4k)
#$base=0x08000000
#$spacesize=0x08000000
#$XCV_MODEL=xcv_8955
#$PA_MODEL=pasw_rda6625
#$FLSH_MODEL=flsh_spi32m
#$FLASH_SIZE=0x400000
#$RAM_SIZE=0x00400000
#$RAM_PHY_SIZE=0x00400000
#$RAM_EXTAPP_SIZE=0
#$FLASH_EXTAPP_SIZE=0x40000
#$CALIB_BASE=0x3FA000
#$FACT_SETTINGS_BASE=0x3F4000
#$CODE_BASE=0x00000000
#$USER_DATA_BASE=0x3F6000
#$USER_DATA_SIZE=0x4000
#$BOOT_SECTOR=0x10000
#$PM_MODEL=pmu_8955
#$FM_MODEL=rdafm_8955
""", 'utf8')


def gen_lod_file(source_bin, target_lod):
    # fix bin size to sector boundary (0x10000)
    binsz = getsize(source_bin)
    f_binsz = (binsz + 0xFFFF) & ~0xFFFF
    # print("Binary size: %d" % binsz)
    f = open(source_bin, "rb")
    f_bin = bytearray(f.read())
    f.close()
    f_bin += bytes(b'\xff' * (f_binsz - binsz))
    # Fix header size
    f_bin[4:8] = f_binsz.to_bytes(4, "little")
    # Fix checksum
    f_bin[8:0xC] = crc32(f_bin).to_bytes(4, "little")
    # write final binary
    f = open(source_bin, "wb")
    f.write(f_bin)
    f.close()

    # write final lod file
    f = open(target_lod, "wb")
    f.write(lod_header)
    address = 0x8250000
    for i in range(int(f_binsz / 4)):
        if address % 0x10000 == 0:
            f.write(bytes('@%08x\n' % address, "utf8"))
        x = i*4
        f.write(bytes('%08x\n' % int.from_bytes(f_bin[x:x+4], 'little'), "utf8"))
        address = address + 4
    f.close()


def gen_fota_file(empty_lod, genfotacmd, source_lod, target_bin):
    if "darwin" in system():
        print("Fota generate is only available on Windows/Linux.\n")
        return

    (source_path, source_name) = split(source_lod)
    tmp_lod = join(source_path, 'tmp_' + source_name)
    copyfile(empty_lod, tmp_lod)
    f_tmp = open(tmp_lod, "ab")
    with open(source_lod, "rb") as src:
        src.seek(len(lod_header))
        f_tmp.write(src.read())
        src.close()
    f_tmp.close()
    print(genfotacmd)
    cmd = " ".join([
        genfotacmd,
        '0x24F000',
        empty_lod,
        tmp_lod,
        target_bin
    ])
    Popen(cmd).wait()
    remove(tmp_lod)

# arg1 operation
op = argv[1]
if op == "genlod":
    # arg2 - bin
    binfile = argv[2]
    # arg3 - target lod
    target = argv[3]
    gen_lod_file(binfile, target)
elif op == "genfota":
    gen_fota_file(argv[2], argv[3], argv[4], argv[5])
