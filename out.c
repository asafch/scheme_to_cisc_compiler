
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
	PUSH(IMM(24))
	CALL(MALLOC)
	DROP(1)
	MOV(ADDR(MEM_START), IMM(T_VOID))
	MOV(ADDR(MEM_START + 1), IMM(T_NIL))
	MOV(ADDR(MEM_START + 2), IMM(T_BOOL))
	MOV(ADDR(MEM_START + 3), IMM(0))
	MOV(ADDR(MEM_START + 4), IMM(T_BOOL))
	MOV(ADDR(MEM_START + 5), IMM(1))
	MOV(ADDR(MEM_START + 6), IMM(T_STRING))
	MOV(ADDR(MEM_START + 6 + 1), IMM(17))
	MOV(ADDR(MEM_START + 6 + 2), IMM('s'))
	MOV(ADDR(MEM_START + 6 + 3), IMM('t'))
	MOV(ADDR(MEM_START + 6 + 4), IMM('r'))
	MOV(ADDR(MEM_START + 6 + 5), IMM('i'))
	MOV(ADDR(MEM_START + 6 + 6), IMM('n'))
	MOV(ADDR(MEM_START + 6 + 7), IMM('g'))
	MOV(ADDR(MEM_START + 6 + 8), IMM('_'))
	MOV(ADDR(MEM_START + 6 + 9), IMM('f'))
	MOV(ADDR(MEM_START + 6 + 10), IMM('o'))
	MOV(ADDR(MEM_START + 6 + 11), IMM('r'))
	MOV(ADDR(MEM_START + 6 + 12), IMM('_'))
	MOV(ADDR(MEM_START + 6 + 13), IMM('s'))
	MOV(ADDR(MEM_START + 6 + 14), IMM('h'))
	MOV(ADDR(MEM_START + 6 + 15), IMM('a'))
	MOV(ADDR(MEM_START + 6 + 16), IMM('h'))
	MOV(ADDR(MEM_START + 6 + 17), IMM('a'))
	MOV(ADDR(MEM_START + 6 + 18), IMM('f'))

	MOV(R0, IMM(MEM_START + 6))

  PUSH(R0)
  CALL(WRITE_SOB)
  PUSH(IMM('\n'))
  CALL(PUTCHAR)
  DROP(2)
  STOP_MACHINE

  return 0;
}