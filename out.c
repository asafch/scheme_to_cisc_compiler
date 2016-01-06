
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
PUSH(IMM(31))
CALL(MALLOC)
DROP(1)
MOV(ADDR(MEM_START), IMM(T_VOID))
MOV(ADDR(MEM_START + 1), IMM(T_NIL))
MOV(ADDR(MEM_START + 2), IMM(T_BOOL))
MOV(ADDR(MEM_START + 3), IMM(0))
MOV(ADDR(MEM_START + 4), IMM(T_BOOL))
MOV(ADDR(MEM_START + 5), IMM(1))
MOV(ADDR(MEM_START + 6), IMM(T_SYMBOL))
MOV(ADDR(MEM_START + 6 + 1), IMM('a'))
MOV(ADDR(MEM_START + 6 + 2), IMM('a'))
MOV(ADDR(MEM_START + 6 + 3), IMM('1'))
MOV(ADDR(MEM_START + 6 + 4), IMM('2'))
MOV(ADDR(MEM_START + 6 + 5), IMM('3'))
MOV(ADDR(MEM_START + 6 + 6), IMM('4'))
MOV(ADDR(MEM_START + 6 + 7), IMM('5'))
MOV(ADDR(MEM_START + 6 + 8), IMM('6'))
MOV(ADDR(MEM_START + 6 + 9), IMM('7'))
MOV(ADDR(MEM_START + 6 + 10), IMM('8'))
MOV(ADDR(MEM_START + 6 + 11), IMM('9'))
MOV(ADDR(MEM_START + 6 + 12), IMM('0'))
MOV(ADDR(MEM_START + 6 + 13), IMM('!'))
MOV(ADDR(MEM_START + 6 + 14), IMM('$'))
MOV(ADDR(MEM_START + 6 + 15), IMM('^'))
MOV(ADDR(MEM_START + 6 + 16), IMM('*'))
MOV(ADDR(MEM_START + 6 + 17), IMM('-'))
MOV(ADDR(MEM_START + 6 + 18), IMM('_'))
MOV(ADDR(MEM_START + 6 + 19), IMM('='))
MOV(ADDR(MEM_START + 6 + 20), IMM('+'))
MOV(ADDR(MEM_START + 6 + 21), IMM('<'))
MOV(ADDR(MEM_START + 6 + 22), IMM('>'))
MOV(ADDR(MEM_START + 6 + 23), IMM('/'))
MOV(ADDR(MEM_START + 6 + 24), IMM('?'))
MOV(ADDR(MEM_START + 6 + 25), IMM('\0'))

	MOV(R0, IMM(MEM_START + 6))

  PUSH(R0)
  CALL(WRITE_SOB)
  DROP(1)
  PUSH(IMM('\n'))
  CALL(PUTCHAR)
  DROP(1)
  STOP_MACHINE

  return 0;
}
