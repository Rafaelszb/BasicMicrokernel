.section .text
.global trap_entry
.align 4

trap_entry:
    # 1. Aloca o frame de 256 bytes na pilha da tarefa atual
    addi sp, sp, -256

    # 2. Salva todos os 32 registradores nas posições (0 a 31)
    sd x1,  0(sp)           # frame[0] -> ra
    
    # Calcula o SP antigo e põe no frame[1]
    addi x1, sp, 256        
    sd x1,  8(sp)           # frame[1] -> sp original
    
    # Restaura o x1 temporário
    ld x1,  0(sp)

    # Salva o resto do bloco
    sd x1,  0(sp)
    sd x3,  16(sp)
    sd x4,  24(sp)
    sd x5,  32(sp)
    sd x6,  40(sp)
    sd x7,  48(sp)
    sd x8,  56(sp)
    sd x9,  64(sp)
    sd x10, 72(sp)
    sd x11, 80(sp)
    sd x12, 88(sp)
    sd x13, 96(sp)
    sd x14, 104(sp)
    sd x15, 112(sp)
    sd x16, 120(sp)
    sd x17, 128(sp)
    sd x18, 136(sp)
    sd x19, 144(sp)
    sd x20, 152(sp)
    sd x21, 160(sp)
    sd x22, 168(sp)
    sd x23, 176(sp)
    sd x24, 184(sp)
    sd x25, 192(sp)
    sd x26, 200(sp)
    sd x27, 208(sp)
    sd x28, 216(sp)
    sd x29, 224(sp)
    sd x30, 232(sp)
    sd x31, 240(sp)

    # 3. Chama o trap_handler (void) passando o frame atual em a0
    mv a0, sp
    call trap_handler

    # 4. Restaura os registradores normais
    # NOTA: Se o seu scheduler rodou (passo 4 do C), os valores contidos 
    # aqui na memória já foram sobrescrevidos com os dados da Task 2!
    ld x1,  0(sp)
    ld x3,  16(sp)
    ld x4,  24(sp)
    ld x5,  32(sp)
    ld x6,  40(sp)
    ld x7,  48(sp)
    ld x8,  56(sp)
    ld x9,  64(sp)
    ld x10, 72(sp)
    ld x11, 80(sp)
    ld x12, 88(sp)
    ld x13, 96(sp)
    ld x14, 104(sp)
    ld x15, 112(sp)
    ld x16, 120(sp)
    ld x17, 128(sp)
    ld x18, 136(sp)
    ld x19, 144(sp)
    ld x20, 152(sp)
    ld x21, 160(sp)
    ld x22, 168(sp)
    ld x23, 176(sp)
    ld x24, 184(sp)
    ld x25, 192(sp)
    ld x26, 200(sp)
    ld x27, 208(sp)
    ld x28, 216(sp)
    ld x29, 224(sp)
    ld x30, 232(sp)
    ld x31, 240(sp)

    # 5. O CHAVEAMENTO FÍSICO: Carrega o SP modificado pelo passo 4 do seu C.
    # O registrador SP agora pula atômicamente para o endereço da pilha da Task 2!
    ld sp,  8(sp)

    # 6. Salta para a nova tarefa reabilitando interrupções
    sret
