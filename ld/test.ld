MEMORY {
	RAM (rw) : ORIGIN = 0x20000000,  LENGTH = 20K
	ROM (rx) : ORIGIN = 0x08000000,  LENGTH = 64K
	}

SECTIONS {
	.vectors : {

	KEEP 	(*(.i_vectors))
		_eVectors = .;
		} > ROM

	.text :  {
		*(.text)
		*(.text*)
		*(.code)
		*(.code*)
		} > ROM

	.rodata : {
		*(.rodata)
		*(.rodata*)
		_sFlashData = .;

		} > ROM


	.bss : {
	       	_sbss = .;
		 *(.bss)
		 *(.bss*)
		_ebss = .;
		 } > RAM
	.data : AT (_sFlashData) {
		_sRamData = .;
		*(.data)
		_eRamData = .;
		} > RAM

	}
