
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
MOV(ADDR(MEM_START + 7), IMM(4))
MOV(ADDR(MEM_START + 8), IMM(T_INTEGER))
MOV(ADDR(MEM_START + 9), IMM(-1))
MOV(ADDR(MEM_START + 10), IMM(T_INTEGER))
MOV(ADDR(MEM_START + 11), IMM(2801))

	MOV(R0, IMM(MEM_START + 6))
	MOV(R0, IMM(MEM_START + 8))
	MOV(R0, IMM(MEM_START + 10))

  PUSH(R0)
  CALL(WRITE_SOB)
  DROP(1)
  PUSH(IMM('\n'))
  CALL(PUTCHAR)
  DROP(1)
  STOP_MACHINE

  return 0;
}