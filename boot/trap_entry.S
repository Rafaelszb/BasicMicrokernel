.section .text
.global trap_entry
.align 4

trap_entry:
    # 1. Aloca imediatamente o frame de 256 bytes na pilha da própria tarefa.
    # O RISC-V exige que sp seja atualizado antes de escrever na memória.
    addi sp, sp, -256

    # 2. Salva o x1 (ra) original na primeira posição para liberar o registrador
    sd x1,  0(sp)           # frame[0] -> ra

    # 3. PULO DO GATO: Como 'sp' decrementou 256, o SP original era (sp + 256).
    # Usamos o próprio x1 (que já salvamos na linha de cima) para fazer essa matemática.
    addi x1, sp, 256        
    sd x1,  8(sp)           # frame[1] -> salva o SP antigo na posição exata x2

    # 4. Restaura o valor original de x1 (ra) a partir da memória
    ld x1,  0(sp)

    # 5. Salva todos os outros registradores sequencialmente (Mapeamento direto de 0 a 31)
    sd x1,  0(sp)           # frame[0]  -> x1 (ra) real da tarefa
    # O frame[1] (sp) já foi preenchido com segurança no passo 3!
    sd x3,  16(sp)          # frame[2]  -> gp
    sd x4,  24(sp)          # frame[3]  -> tp
    sd x5,  32(sp)          # frame[4]  -> t0
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

    # 6. Passa o ponteiro do frame (sp atual) em a0 para o C
    mv a0, sp
    call trap_handler

    # --- RESTAURAÇÃO DE CONTEXTO ---
    # Se o seu scheduler em C trocou de tarefa mudando os valores do ponteiro 'frame',
    # o 'sp' atualizado já estará apontando para a estrutura da Task 2!

    # Restaura o RA da tarefa destino
    ld x1,  0(sp)

    # Restaura todos os registradores gerais
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

    # 7. Restaura o SP da nova tarefa de forma atômica
    ld sp,  8(sp)

    # Retorna executando a Task 2!
    sret
