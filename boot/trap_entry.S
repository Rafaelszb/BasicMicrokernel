.section .text
.global trap_entry
trap_entry:
addi sp, sp, -256
# TODO:
# Salvar x1..x31 na stack.

    sd x1,  8(sp)       # ra (Return Address)
    # x2 é o SP, salvamos o valor dele ANTES do trap (sp + 256)
    addi t0, sp, 256
    sd t0,  16(sp)      
    sd x3,  24(sp)      # gp (Global Pointer)
    sd x4,  32(sp)      # tp (Thread Pointer)
    sd x5,  40(sp)      # t0
    sd x6,  48(sp)      # t1
    sd x7,  56(sp)      # t2
    sd x8,  64(sp)      # s0 
    sd x9,  72(sp)      # s1
    sd x10, 80(sp)      # a0 
    sd x11, 88(sp)      # a1
    sd x12, 96(sp)      # a2
    sd x13, 104(sp)     # a3
    sd x14, 112(sp)     # a4
    sd x15, 120(sp)     # a5
    sd x16, 128(sp)     # a6
    sd x17, 136(sp)     # a7
    sd x18, 144(sp)     # s2
    sd x19, 152(sp)     # s3
    sd x20, 160(sp)     # s4
    sd x21, 168(sp)     # s5
    sd x22, 176(sp)     # s6
    sd x23, 184(sp)     # s7
    sd x24, 192(sp)     # s8
    sd x25, 200(sp)     # s9
    sd x26, 208(sp)     # s10
    sd x27, 216(sp)     # s11
    sd x28, 224(sp)     # t3
    sd x29, 232(sp)     # t4
    sd x30, 240(sp)     # t5
    sd x31, 248(sp)     # t6

mv a0, sp
call trap_handler
# TODO:
# Restaurar x1..x31 da stack.
    ld x1,  8(sp)
    # Não restauramos x2 (sp) daqui diretamente, faremos isso no final com addi
    ld x3,  24(sp)
    ld x4,  32(sp)
    ld x5,  40(sp)
    ld x6,  48(sp)
    ld x7,  56(sp)
    ld x8,  64(sp)
    ld x9,  72(sp)
    ld x10, 80(sp)
    ld x11, 88(sp)
    ld x12, 96(sp)
    ld x13, 104(sp)
    ld x14, 112(sp)
    ld x15, 120(sp)
    ld x16, 128(sp)
    ld x17, 136(sp)
    ld x18, 144(sp)
    ld x19, 152(sp)
    ld x20, 160(sp)
    ld x21, 168(sp)
    ld x22, 176(sp)
    ld x23, 184(sp)
    ld x24, 192(sp)
    ld x25, 200(sp)
    ld x26, 208(sp)
    ld x27, 216(sp)
    ld x28, 224(sp)
    ld x29, 232(sp)
    ld x30, 240(sp)
    ld x31, 248(sp)

    # 5. Desaloca o espaço da pilha, voltando o SP ao que era antes do trap
    addi sp, sp, 256
addi sp, sp, 256
sret
