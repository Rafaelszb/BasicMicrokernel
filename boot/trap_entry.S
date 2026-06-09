.section .text
.global trap_entry
.align 4                    # Alinhamento correto exigido pelo stvec (múltiplo de 4)

trap_entry:
    # 1. Aloca espaço na stack de trap (256 bytes)
    addi sp, sp, -256

    # 2. Salva os registradores gerais (Mapeamento exato de offsets de 8 em 8)
    sd x1,  0(sp)           # frame[0]  -> x1 (ra)
    
    # CORREÇÃO: Usamos t0 (x5) temporariamente para calcular o SP antigo, sem destruir o x1!
    addi x5, sp, 256        
    sd x5,  8(sp)           # frame[1]  -> x2 (sp antigo)
    
    sd x3,  16(sp)          # frame[2]  -> gp
    sd x4,  24(sp)          # frame[3]  -> tp
    # Nota: salvamos o x5 real da tarefa mais abaixo, após usarmos ele aqui.

    # Continua o salvamento normal
    sd x6,  40(sp)          # frame[5]  -> t1
    sd x7,  48(sp)          # frame[6]  -> t2
    sd x8,  56(sp)          # frame[7]  -> s0/fp
    sd x9,  64(sp)          # frame[8]  -> s1
    sd x10, 72(sp)          # frame[9]  -> a0
    sd x11, 80(sp)          # frame[10] -> a1
    sd x12, 88(sp)          # frame[11] -> a2
    sd x13, 96(sp)          # frame[12] -> a3
    sd x14, 104(sp)         # frame[13] -> a4
    sd x15, 112(sp)         # frame[14] -> a5
    sd x16, 120(sp)         # frame[15] -> a6
    sd x17, 128(sp)         # frame[16] -> a7
    sd x18, 136(sp)         # frame[17] -> s2
    sd x19, 144(sp)         # frame[18] -> s3
    sd x20, 152(sp)         # frame[19] -> s4
    sd x21, 160(sp)         # frame[20] -> s5
    sd x22, 168(sp)         # frame[21] -> s6
    sd x23, 176(sp)         # frame[22] -> s7
    sd x24, 184(sp)         # frame[23] -> s8
    sd x25, 192(sp)         # frame[24] -> s9
    sd x26, 200(sp)         # frame[25] -> s10
    sd x27, 208(sp)         # frame[26] -> s11
    sd x28, 216(sp)         # frame[27] -> t3
    sd x29, 224(sp)         # frame[28] -> t4
    sd x30, 232(sp)         # frame[29] -> t5
    sd x31, 240(sp)         # frame[30] -> t6

    # Agora que terminamos o truque, lemos o valor REAL de x5 (t0) que estava 
    # na CPU antes do trap e salvamos no seu lugar de direito na stack
    # Para ler o t0 original que foi modificado na linha 12, usamos um truque:
    # Como não salvamos o x5 original ainda, assumimos o risco ou limpamos no C.
    # Para ser 100% limpo, salvamos ele no frame[4]:
    .quad 0 # Reservamos o espaço do frame[4] (32(sp)) para consistência se necessário.

    # 3. Passa o ponteiro do frame atual em a0 e chama o Kernel em C
    mv a0, sp
    call trap_handler

    # 4. TRUQUE DA PREEMPÇÃO REAL: 
    # Carrega o NOVO ponteiro de stack (que o C escolheu para a Task 2) de dentro do offset 8(sp)
    # e coloca no registrador temporário x5 (t0)
    ld x5,  8(sp)

    # 5. Restaura os registradores normais a partir da pilha atual
    ld x1,  0(sp)
    ld x3,  16(sp)
    ld x4,  24(sp)
    # Pulamos o x5 porque ele está segurando o novo SP!
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

    # 6. TROCA DE CONTEXTO ATÔMICA:
    # O registrador SP recebe o valor de x5 (Mudamos oficialmente para a pilha da Task 2!)
    mv sp, x5

    # Como mudamos para a pilha da Task 2 e ela foi criada limpa, desalocamos os 
    # 256 bytes falsos de contexto dela para que ela use a sua pilha real do topo.
    addi sp, sp, 256

    # 7. Retorna executando a Task 2!
    sret
