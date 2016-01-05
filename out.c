
#include <stdio.h>
#include <stdlib.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1

#include "cisc.h"

int main()
{
  START_MACHINE;

  JUMP(CONTINUE);

  #include "char.lib"
  #include "io.lib"
  #include "math.lib"
  #include "string.lib"
  #include "system.lib"

 CONTINUE:
  L_if_else_1:
	applic
	CMP(R0, 1)
	JUMP_NE(L_else_1)
	1
	JUMP(L_if_end_1)
L_else_1: 
	2
L_if_end_1:

L_if_else_2:
	applic
	CMP(R0, 1)
	JUMP_NE(L_else_2)
	3
	JUMP(L_if_end_2)
L_else_2: 
	4
L_if_end_2:

L_if_else_3:
	5
	CMP(R0, 1)
	JUMP_NE(L_else_3)
	6
	JUMP(L_if_end_3)
L_else_3: 
	MOV(R0, T_VOID)
L_if_end_3:


  STOP_MACHINE;

  return 0;
}