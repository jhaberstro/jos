global outb
global outw
global outl
global inb
global inw
global inl

section .text

outb:
    push ebp
    mov ebp, esp
    
    mov dx, [ebp + 8]      ; port
    mov al, [ebp + 12]     ; value
    out dx, al

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]      ; port
    mov ax, [ebp + 12]     ; value
    out dx, ax

    pop ebp
    ret

outl:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]      ; port
    mov eax, [ebp + 12]    ; value
    out dx, eax

    pop ebp
    ret

inb:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    in al, dx

    pop ebp
    ret

inw:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    in ax, dx

    pop ebp
    ret

inl:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8]
    in eax, dx

    pop ebp
    ret

