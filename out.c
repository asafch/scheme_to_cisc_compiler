
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1
#define MEM_START 2
#define FREE_VAR_TAB_START (MEM_START + 50)

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
  printf("Exception: trying to apply a non-procedure\n");
  HALT

EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS:
  printf("Ecxeption: applying closure on wrong number of argumens, given: %ld\n", FPARG(1));
  HALT

CONTINUE:
	PUSH(IMM(50))
	CALL(MALLOC)
	DROP(1)
	long consts[50] = {T_VOID, T_NIL
, T_BOOL, 0
, T_BOOL, 1
, T_STRING, 1, 'B', T_INTEGER, 3
, T_STRING, 5, 'q', 'u', 'o', 't', 'e', T_SYMBOL, MEM_START + 11
, T_STRING, 4, 'c', 'a', 'k', 'e', T_SYMBOL, MEM_START + 20
, T_PAIR, MEM_START + 26, MEM_START + 1
, T_PAIR, MEM_START + 18, MEM_START + 28
, T_STRING, 6, 'c', 'o', 'o', 'k', 'i', 'e', T_SYMBOL, MEM_START + 34
, T_VECTOR, 4, MEM_START + 6, MEM_START + 9, MEM_START + 31, MEM_START + 42};

	memcpy(M(mem) + 1, consts, sizeof(long) * 50);
printf("%p:%p:%ld", machine->mem ,&machine->mem, machine->mem[0]);
	MOV(R0, IMM(MEM_START + 44))

  PUSH(R0)
  CALL(WRITE_SOB)
  PUSH(IMM('\n'))
  CALL(PUTCHAR)
  DROP(2)
  STOP_MACHINE

  return 0;
}
