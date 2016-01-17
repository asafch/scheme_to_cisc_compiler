
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1
#define MEM_START 2
#define FREE_VAR_TAB_START (MEM_START + 68)
#define SYM_TAB_START (FREE_VAR_TAB_START + 46)

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
  printf("Ecxeption: applying closure on wrong number of argumens, given: %ld\n", FPARG(1));
  HALT

CONTINUE:
PUSH(IMM(1 + 68))
CALL(MALLOC) //allocate memory for constants
DROP(1)	long consts[68] = {T_VOID, T_NIL
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
, T_VECTOR, 6, MEM_START + 6, MEM_START + 9, MEM_START + 31, MEM_START + 42, MEM_START + 51, MEM_START + 58};

memcpy(M(mem) + MEM_START, consts, sizeof(long) * 68);
PUSH(IMM(46))
CALL(MALLOC) //allocate memory for all free variables in the program
DROP(1)
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
	MOV(R0, IMM(MEM_START + 60))

  PUSH(R0)
  CALL(WRITE_SOB)
  PUSH(IMM('\n'))
  CALL(PUTCHAR)
  DROP(2)
  STOP_MACHINE

  return 0;
}