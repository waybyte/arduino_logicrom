OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(appimg_enter)
EXTERN(appimg_enter)
EXTERN(appimg_exit)
EXTERN(appimg_get_param)
EXTERN(appimg_set_param)
PROVIDE(appimg_get_param = 0);
PROVIDE(appimg_set_param = 0);
strtol = _strtol;
strtoll = _strtoll;
strtoul = _strtoul;
strtoull = _strtoll;
strtok_r = _strtok_r;
_strtol_r = __strtol_r;
_strtoll_r = __strtoll_r;
_strtoul_r = __strtoul_r;
_strtoull_r = __strtoull_r;
_strtod_l = __strtod_l;
_strerror_r = __strerror_r;
_svfprintf_r = __svfprintf_r;
_vsnprintf_r = __vsnprintf_r;
_fflush_r = __fflush_r;
__ssvfiscanf_r = ___ssvfiscanf_r;
_vfprintf_r = __vfprintf_r;
_svfiprintf_r = __svfiprintf_r;
__ssvfscanf_r = ___ssvfscanf_r;
__sflush_r = ___sflush_r;
__sfvwrite_r = ___sfvwrite_r;
_fclose_r = __fclose_r;
_strerror_r = __strerror_r;
_strtod_l = __strtod_l;
_tzset_unlocked_r = __tzset_unlocked_r;
__tzcalc_limits = ___tzcalc_limits;
gmtime_r = _gmtime_r;
MEMORY {
    flash (rx): ORIGIN = 0x60230000, LENGTH = 0x100000
    ram (rwx): ORIGIN = 0x80000000 +0xec0000, LENGTH = 0x100000
}
SECTIONS {
    . = ORIGIN(flash);
    __flash_start = .;
    .imageheader : {
        LONG(0x41505032) LONG(0) LONG(0) KEEP(*(.rodata.core_stub.header))
        LONG(2) LONG(__corestub_load_start - __flash_start) LONG(__corestub_end - __corestub_start) LONG(__corestub_start)
        LONG(4) LONG(__text_load_start - __flash_start) LONG(__text_end - __text_start) LONG(__text_start)
        LONG(4) LONG(__init_array_load_start - __flash_start) LONG(__init_array_end - __init_array_start) LONG(__init_array_start)
        LONG(1) LONG(__data_load_start - __flash_start) LONG(__data_end - __data_start) LONG(__data_start)
        LONG(3) LONG(0) LONG(__bss_end - __bss_start) LONG(__bss_start)
        LONG(0) LONG(0) LONG(0) LONG(0)
    } > flash
    ASSERT(SIZEOF(.imageheader) == 128, "invalid app image header")
    .initdata ALIGN(0x20) : {
        KEEP(*(.appinit));
    } > flash
    . = ORIGIN(ram);
    __ram_start = .;
    .corestub ALIGN(0x20) : {
        __corestub_start = .;
        *(.text.core_stub.*)
        __corestub_end = .;
        . = ALIGN(0x20);
        __corestub_load_start = LOADADDR(.corestub);
    } AT>flash
    .data ALIGN(0x20) : {
        __data_start = .;
        *(.data_begin .data_begin.* .data .data.* .gnu.linkonce.d.* .got.plt .got .shdata .data_end .data_end.*)
        __data_end = .;
        . = ALIGN(0x20);
        __data_load_start = LOADADDR(.data);
    } AT>flash
    .bss ALIGN(0x20) : {
        __bss_start = .;
        *(.bss_begin .bss_begin.* .shbss .bss .bss.* COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.* .gnu.linkonce.b.* .bss_end .bss_end.*)
        __bss_end = .;
        . = ALIGN(0x20);
    }
    . = LOADADDR(.corestub) + SIZEOF(.corestub) + SIZEOF(.data);
    .text ALIGN(0x20) : {
        __text_start = .;
  KEEP(*(.appinit));
        *(.text .text.* .gnu.linkonce.t.* .plt .rdata .rodata .rodata.* .constdata .constdata.* .gnu.linkonce.r.* .glue_7 .glue_7t .vfp11_veneer .v4_bx vtable)
        KEEP(*(.eh_frame*))
        __text_end = .;
        . = ALIGN(0x20);
        __text_load_start = LOADADDR(.text);
    } > flash
    .ll ALIGN(0x20) : {
        PROVIDE_HIDDEN (__ll_entry_start = .);
        KEEP( *(SORT(.ll_entry_cmdlist*)) );
        PROVIDE_HIDDEN (__ll_entry_end = .);
    } > flash
    .ARM.extab ALIGN(0x20) : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > flash
    __exidx_start = .;
    .ARM.exidx ALIGN(0x20) : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > flash
    __exidx_end = .;
    .init_array ALIGN(0x20) : {
        __init_array_start = .;
        KEEP(*(.init_array .init_array.*))
        __init_array_end = .;
        . = ALIGN(0x20);
        __init_array_load_start = LOADADDR(.init_array);
    } > flash
    __flash_end = ADDR(.init_array) + SIZEOF(.init_array);
    __ram_end = ADDR(.bss) + SIZEOF(.bss);
    ASSERT(__flash_end <= ORIGIN(flash) + LENGTH(flash), "FLASH overflow")
    ASSERT(__ram_end <= ORIGIN(ram) + LENGTH(ram), "RAM overflow")
    .comment 0 : { *(.comment) }
    /DISCARD/ : { *(.dynstr*) }
    /DISCARD/ : { *(.dynamic*) }
    /DISCARD/ : { *(.plt*) }
    /DISCARD/ : { *(.interp*) }
    /DISCARD/ : { *(.gnu*) }
}
