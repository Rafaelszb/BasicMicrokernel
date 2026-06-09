.section .text
.global trap_entry
.align 8                # Garante o alinhamento correto exigido pelo stvec

trap_entry:
    # 1. Aloca espaço na stack para os 31 registradores (31 * 8 bytes = 248 bytes)
    # Arredondamos para 256 bytes para manter o alinhamento de 16 bytes da stack do RISC-V.
    addi sp, sp, -256

    # 2. Salva os registradores gerais (x1 a x31) na stack
    sd x1,  0(sp)
    # x2 é o próprio 'sp', salvamos o valor que ele tinha ANTES do decremento (-256)
    addi x1, sp, 256
    sd x1,  8(sp)        
    sd x3,  16(sp)
    sd x4,  24(sp)
    sd x5,  32(sp)
    sd x6,  40(sp)
    sd x7,  48(sp)
    sd x8,  56(sp)
    sd x9,  64(sp)
    sd x10, 72(sp)       # a0
    sd x11, 80(sp)       # a1
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

    # 3. Passa o ponteiro da stack (o frame) como primeiro argumento (a0) para o C
    mv a0, sp
    
    # Chama o gerenciador de traps em C
    # Nota: Dentro do seu trap_handler(), você deve chamar o schedule_from_trap(frame)
    call trap_handler

    # 4. Restaura os registradores gerais da stack
    # Nota: Se o scheduler trocou de tarefa, os dados contidos aqui dentro
    # já serão os da NOVA tarefa apontada pelo frame!
    ld x1,  0(sp)
    # Ignoramos o x2 (sp) aqui para não quebrar o ponteiro da stack atual de forma prematura.
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

    # 5. Desaloca o espaço da stack
    addi sp, sp, 256

    # Retorna do modo Supervisor para o modo anterior (User/U-mode) 
    # carregando o novo 'sepc' alterado pelo scheduler.
    sret
