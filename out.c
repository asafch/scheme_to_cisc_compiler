
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1
#define MEM_START 2
#define FREE_VAR_TAB_START (MEM_START + 70)
#define SYM_TAB_START (FREE_VAR_TAB_START + 48)

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
  #include "debug.h"

EXCPETION_APPLYING_NON_PROCEDURE:
  printf("Exception: trying to apply a non-procedure\n");
  HALT

EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS:
  printf("Exception: applying closure on wrong number of argumens, given: %ld\n", FPARG(1));
  HALT

EXCEPTION_UNDEFINED_VARIABLE:
  printf("Exception: undefined variable\n");
  HALT

CONTINUE:
PUSH(IMM(1 + 70))
CALL(MALLOC) //allocate memory for constants
DROP(1)
long consts[70] = {T_VOID, T_NIL
, T_BOOL, 0
, T_BOOL, 1
, T_STRING, 1, 'B', T_INTEGER, 3
, T_STRING, 5, 'q', 'u', 'o', 't', 'e', T_SYMBOL, MEM_START + 11
, T_STRING, 4, 'c', 'a', 'k', 'e', T_SYMBOL, MEM_START + 20
, T_PAIR, MEM_START + 26, MEM_START + 1
, T_PAIR, MEM_START + 18, MEM_START + 28
, T_STRING, 6, 'c', 'o', 'o', 'k', 'i', 'e', T_SYMBOL, MEM_START + 34
, T_STRING, 5, 'd', 'o', 'n', 'u', 't', T_SYMBOL, MEM_START + 44
, T_STRING, 3, 'p', 'i', 'e', T_SYMBOL, MEM_START + 53
, T_VECTOR, 6, MEM_START + 6, MEM_START + 9, MEM_START + 31, MEM_START + 42, MEM_START + 51, MEM_START + 58, T_INTEGER, 5
};
memcpy(M(mem) + MEM_START, consts, sizeof(long) * 70);

PUSH(IMM(48))
CALL(MALLOC) //allocate memory for all free variables in the program
DROP(1)
long freevars[49] = {0
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
, T_UNDEFINED
};//memcpy from freevars + 1 because freevars[0]=0 for padding
memcpy(M(mem) + FREE_VAR_TAB_START, freevars + 1, sizeof(long) * 48);

//sym_tab initialization
long symbols[1 + 10] = {0
 	, MEM_START + 18, SYM_TAB_START + 2
	, MEM_START + 26, SYM_TAB_START + 4
	, MEM_START + 42, SYM_TAB_START + 6
	, MEM_START + 51, SYM_TAB_START + 8
	, MEM_START + 58, T_NIL
};
PUSH(IMM(10))
CALL(MALLOC) //allocate memory for the symbol linked list
DROP(1)
//in the following memcpy, the source is symbols + 1, because symbols[0] is just a padding 0
memcpy(M(mem) + SYM_TAB_START, symbols + 1, sizeof(long) * 10);
//mem[1] holds the address of the first link in the symbols linked list
MOV(ADDR(1), IMM(FREE_VAR_TAB_START + 48))




//append
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_append))
MOV(IND(FREE_VAR_TAB_START + 0), R0)
JUMP(L_exit_append)
L_append:
  PUSH(FP)
  MOV(FP, SP)
	POP(FP)
  RETURN
L_exit_append:


	MOV(R0, IMM(MEM_START + 60))
L_simple_env_expansion_1:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_1:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_1)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_1)
L_simple_env_expand_end_1:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_1:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_1)
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R4), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_1)
L_simple_param_copy_end_1:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_1))
	JUMP(L_lambda_simple_end_1)
L_lambda_simple_1:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(1))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
	MOV(R0, FPARG(0 + 2))
	POP(FP)
	RETURN
L_lambda_simple_end_1:
	MOV(IND(FREE_VAR_TAB_START + 46), R0)
L_applic_1:
	MOV(R0, IMM(MEM_START + 68))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 46))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_1:
	MOV(IND(FREE_VAR_TAB_START + 47), R0)
L_applic_2:
	MOV(R0, INDD(FREE_VAR_TAB_START, 47))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 46))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
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
