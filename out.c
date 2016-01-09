
#include <stdio.h>
#include <stdlib.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1
#define MEM_START 1

#include "cisc.h"

int main()
{

  START_MACHINE
  JUMP(CONTINUE)

  #include "char.lib"
  #include "io.lib"
  #include "math.lib"
  #include "scheme.lib"
  #include "string.lib"
  #include "system.lib"

EXCPETION_APPLYING_NON_PROCEDURE:
  SHOW("Trying to apply a non-procedure: ", R0)
  HALT

CONTINUE:
	PUSH(IMM(12))
	CALL(MALLOC)
	DROP(1)
	MOV(ADDR(MEM_START), IMM(T_VOID))
	MOV(ADDR(MEM_START + 1), IMM(T_NIL))
	MOV(ADDR(MEM_START + 2), IMM(T_BOOL))
	MOV(ADDR(MEM_START + 3), IMM(0))
	MOV(ADDR(MEM_START + 4), IMM(T_BOOL))
	MOV(ADDR(MEM_START + 5), IMM(1))
	MOV(ADDR(MEM_START + 6), IMM(T_INTEGER))
	MOV(ADDR(MEM_START + 7), IMM(1))
	MOV(ADDR(MEM_START + 8), IMM(T_INTEGER))
	MOV(ADDR(MEM_START + 9), IMM(2))
	MOV(ADDR(MEM_START + 10), IMM(T_INTEGER))
	MOV(ADDR(MEM_START + 11), IMM(3))

L_applic_2:
	MOV(R0, IMM(MEM_START + 6))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 8))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 10))
	PUSH(R0)
	PUSH(IMM(3))
L_simple_env_expansion_2:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_2:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_2)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_2)
L_simple_env_expand_end_2:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_2:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_2)
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R4), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_2)
L_simple_param_copy_end_2:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_2))
	JUMP(L_lambda_simple_end_2)
L_lambda_simple_2:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R0, FPARG(0 + 2))

MOV(R0, FPARG(1 + 2))

	POP(FP)
	RETURN
L_lambda_simple_end_2:
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_2:

  PUSH(R0)
  CALL(WRITE_SOB)
  PUSH(IMM('\n'))
  CALL(PUTCHAR)
  DROP(2)
  STOP_MACHINE

  return 0;
}