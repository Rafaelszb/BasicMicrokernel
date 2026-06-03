.section .text
.global trap_entry
trap_entry:
addi sp, sp, -256
# TODO:
# Salvar x1..x31 na stack.
mv a0, sp
call trap_handler
# TODO:
# Restaurar x1..x31 da stack.
addi sp, sp, 256
sret
