ENTRY(HandlerReset)

_estack = 0x20004000;    /* end of 16K RAM */

_Min_Heap_Size = 0;      /* required amount of heap  */
_Min_Stack_Size = 0x80;  /* required amount of stack */

MEMORY
{
	FLASH (rx)      : ORIGIN = 0x00000000, LENGTH = 60K
	RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 16K
}

SECTIONS
{
	/* Program code - EXCLUDE overlay objects */
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.text.isr))
		EXCLUDE_FILE(*spectrum.o *fm.o *fmradio.o *aircopy.o *menu_render.o) *(.text)
		EXCLUDE_FILE(*spectrum.o *fm.o *fmradio.o *aircopy.o *menu_render.o) *(.text*)
		EXCLUDE_FILE(*spectrum.o *fm.o *fmradio.o *aircopy.o *menu_render.o) *(.rodata)
		EXCLUDE_FILE(*spectrum.o *fm.o *fmradio.o *aircopy.o *menu_render.o) *(.rodata*)
		*(.glue_7)
		*(.glue_7t)
		*(.eh_frame)

		KEEP(*(.fini))
		. = ALIGN(4);
		_etext = .;
	} >FLASH

	. = ALIGN(4);
	flash_data_start = .;

	.data :
	{
		. = ALIGN(4);
		sram_data_start = .;
		*(.sramtext)
		*(.srambss)
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_edata = .;
	} >RAM AT> FLASH

	sram_data_end = .;

	. = ALIGN(4);
	.bss :
	{
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} >RAM

	/* EEPROM-backed code overlays.
	   All sections share the same RAM VMA (only one loaded at a time).
	   LMA at 0xF0000 is virtual - NOT stored in flash binary.
	   Overlay loader copies from EEPROM to RAM at runtime. */
	. = ALIGN(4);
	_overlay_start = .;

	OVERLAY : NOCROSSREFS AT(0xF0000)
	{
		.ovly_spectrum
		{
			*spectrum.o(.text .text.* .rodata .rodata.* .data .data.*)
			. = ALIGN(4);
		}
		.ovly_fm
		{
			*fm.o(.text .text.* .rodata .rodata.* .data .data.*)
			*fmradio.o(.text .text.* .rodata .rodata.* .data .data.*)
			. = ALIGN(4);
		}
		.ovly_aircopy
		{
			*aircopy.o(.text .text.* .rodata .rodata.* .data .data.*)
			. = ALIGN(4);
		}
		.ovly_menu
		{
			*menu_render.o(.text .text.* .rodata .rodata.* .data .data.*)
			. = ALIGN(4);
		}
	} > RAM

	_overlay_end = .;

	_ovly_spectrum_size = SIZEOF(.ovly_spectrum);
	_ovly_fm_size = SIZEOF(.ovly_fm);
	_ovly_aircopy_size = SIZEOF(.ovly_aircopy);
	_ovly_menu_size = SIZEOF(.ovly_menu);

	/* Check that there is enough RAM for stack */
	._user_heap_stack :
	{
		. = ALIGN(4);
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
		. = ALIGN(4);
	} >RAM
}
