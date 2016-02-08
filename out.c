
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1
#define MEM_START 2
#define FREE_VAR_TAB_START (MEM_START + 92)
#define SYM_TAB_START (FREE_VAR_TAB_START + 49)

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
  printf("Exception: applying closure on wrong number of arguments, given: %ld\n", FPARG(1));
  HALT

EXCEPTION_UNDEFINED_VARIABLE:
  printf("Exception: undefined variable\n");
  HALT

EXCEPTION_NOT_A_CHAR:
  printf("Exception: argument is not a character\n");
  HALT

EXCEPTION_NOT_A_CLOSURE:
  printf("Exception: argument is not a procedure\n");
  HALT

EXCEPTION_NOT_A_PAIR:
  printf("Exception: argument is not a pair\n");
  HALT

EXCEPTION_NOT_A_FRACTION:
  printf("Exception: argument is not a fraction\n");
  HALT

EXCEPTION_NOT_AN_INTEGER:
  printf("Exception: argument is not an integer\n");
  HALT

EXCEPTION_NOT_A_NUMBER:
  printf("Exception: argument is not a number\n");
  HALT

EXCEPTION_NOT_A_STRING:
  printf("Exception: argument is not a string\n");
  HALT

EXCEPTION_NOT_A_SYMBOL:
  printf("Exception: argument is not a symbol\n");
  HALT

EXCEPTION_NOT_A_VECTOR:
  printf("Exception: argument is not a vector\n");
  HALT

EXCEPTION_BAD_INDEX:
  printf("Exception: argument is not a valid index for access\n");
  HALT

CONTINUE:
PUSH(IMM(1 + 92))
CALL(MALLOC) //allocate memory for constants
DROP(1)
long consts[92] = {T_VOID, T_NIL
, T_BOOL, 0
, T_BOOL, 1
, T_INTEGER, 1234
, T_STRING, 1, 'a', T_SYMBOL, MEM_START + 8
, T_STRING, 1, 'b', T_SYMBOL, MEM_START + 13
, T_INTEGER, 1
, T_INTEGER, 2
, T_CHAR, 97
, T_STRING, 3, 'a', 'b', 'c', T_STRING, 6, 'l', 'a', 'm', 'b', 'd', 'a', T_SYMBOL, MEM_START + 29
, T_INTEGER, 3
, T_VECTOR, 3, MEM_START + 18, MEM_START + 20, MEM_START + 39, T_STRING, 1, 'c', T_SYMBOL, MEM_START + 46
, T_VECTOR, 3, MEM_START + 11, MEM_START + 16, MEM_START + 49, T_VECTOR, 3, MEM_START + 11, MEM_START + 4, MEM_START + 1, T_PAIR, MEM_START + 11, MEM_START + 16
, T_INTEGER, 0
, T_INTEGER, 234
, T_INTEGER, 97
, T_INTEGER, 10000
, T_INTEGER, 65
, T_CHAR, 65
, T_INTEGER, 98
, T_STRING, 2, 'a', 'b', T_SYMBOL, MEM_START + 78
, T_INTEGER, 7
, T_INTEGER, 4
, T_INTEGER, 6
, T_INTEGER, 5
};
memcpy(M(mem) + MEM_START, consts, sizeof(long) * 92);

PUSH(IMM(49))
CALL(MALLOC) //allocate memory for all free variables in the program
DROP(1)
long freevars[50] = {0
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
, T_UNDEFINED
};//memcpy from freevars + 1 because freevars[0]=0 for padding
memcpy(M(mem) + FREE_VAR_TAB_START, freevars + 1, sizeof(long) * 49);

//sym_tab initialization
long symbols[1 + 10] = {0
 	, MEM_START + 11, SYM_TAB_START + 2
	, MEM_START + 16, SYM_TAB_START + 4
	, MEM_START + 37, SYM_TAB_START + 6
	, MEM_START + 49, SYM_TAB_START + 8
	, MEM_START + 82, IMM(MEM_START + 1)
};
PUSH(IMM(10))
CALL(MALLOC) //allocate memory for the symbol linked list
DROP(1)
//in the following memcpy, the source is symbols + 1, because symbols[0] is just a padding 0
memcpy(M(mem) + SYM_TAB_START, symbols + 1, sizeof(long) * 10);
//mem[1] holds the address of the first link in the symbols linked list
MOV(ADDR(1), IMM(SYM_TAB_START))




//apply
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_apply))
MOV(IND(FREE_VAR_TAB_START + 1), R0)
JUMP(L_exit_apply)
L_apply:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_LT(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))         //the procedure
  CMP(IND(R1), IMM(T_CLOSURE))
  JUMP_NE(EXCEPTION_NOT_A_CLOSURE)
  MOV(R12, FPARG(1))          //R12 = n
  MOV(R11, SP)
  SUB(R11, IMM(4))
  SUB(R11, FPARG(1))        //R11 is the position on stack of the optional args list
  MOV(R2, STACK(R11))         //the arguments list
  ADD(R11, IMM(1))
  CMP(IND(R2), IMM(T_PAIR))
  JUMP_EQ(L_apply_has_optional_args)
  CMP(IND(R2), IMM(T_NIL))
  JUMP_EQ(L_apply_has_optional_args)
  JUMP(EXCEPTION_NOT_A_PAIR)
L_apply_has_optional_args:
  MOV(R4, IMM(0))           //list length counter
  MOV(R5, FPARG(-1))        //return address
  MOV(R6, FPARG(-2))        //old FP
  MOV(R7, R2)
L_apply_list_length:
  CMP(IND(R7), IMM(T_NIL))
  JUMP_EQ(L_apply_list_length_end)
  INCR(R4)
  MOV(R7, INDD(R7, 2))
  JUMP(L_apply_list_length)
L_apply_list_length_end:
  MOV(R7, SP)
  SUB(R7, IMM(6))                 //R7 is the first formal argument
  ADD(SP, R4)
  SUB(SP, IMM(3))                     //SP is where it should be after the frame is fixed, subtracting 2 for the procedure and optional args list
  //do not push the old FP back to the stack - it will be pushed at the entry to the procedure's body
  MOV(STARG(-1), R5)                  //set the old return address
  MOV(STARG(0), INDD(R1, 1))          //set the procedure's environment
  //do not yet set the corrected number of arguments - it can overwrite the first formal argument, if there is one
  //set the corrected number of arguments only after repositioning all formal arguments and unwrapping the optionals list
  MOV(R8, SP)
  SUB(R8, IMM(4))                     //R8 is the new position for the first formal argument (R7 is the old one)
  MOV(R9, IMM(0))                     //loop counter for repostioning formal arguments
  MOV(R10, R12)
  SUB(R10, IMM(2))                     //number of formal args to reposition
L_apply_reposition_formal_args:
  CMP(R9, R10)
  JUMP_EQ(L_apply_reposition_formal_args_end)
  MOV(R13, STACK(R7))
  MOV(STACK(R8), R13)
  DECR(R7)
  DECR(R8)
  INCR(R9)
  JUMP(L_apply_reposition_formal_args)
L_apply_reposition_formal_args_end:
  //all formal args were repositioned, and R8 is the stack position for unwrapping the optional args list
  MOV(STARG(1), R4)
  ADD(STARG(1), R12)
  SUB(STARG(1), IMM(2))               //subtract 2 for the procedure and optional args list, the number of args has been corrected
  MOV(R7, R2)                         //R7 is the list of args
L_apply_unwrap_args:
  CMP(IND(R7), IMM(T_NIL))
  JUMP_EQ(L_apply_unwrap_args_end)
  MOV(R9, INDD(R7, 1))
  MOV(STACK(R8), R9)
  DECR(R8)
  MOV(R7, INDD(R7, 2))
  JUMP(L_apply_unwrap_args)
L_apply_unwrap_args_end:
  MOV(FP, R6)              //set FP to its old value
  JUMPA(INDD(R1, 2))       //jump to closure's body
  // NOTE: the following POP(FP), RETURN are a product of the footer generator and will never be executed, since the frame has been overrun to accomodate the procedure that's being applied

	POP(FP)
  RETURN
L_exit_apply:




//<
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_less))
MOV(IND(FREE_VAR_TAB_START + 2), R0)
JUMP(L_exit_less)
L_less:
  PUSH(FP)
  MOV(FP, SP)
  MOV(R1, FPARG(1))                   //number of arguments
  CMP(R1, IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  CMP(R1, IMM(1))
  JUMP_EQ(L_less_true)
  MOV(R2, IMM(1))                     //loop counter
  MOV(R3, IMM(2))
  MOV(R4, FPARG(R3))
  INCR(R3)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_less_first_arg_is_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  MOV(R5, INDD(R4, 1))                      //previous value's numerator
  MOV(R6, INDD(R4, 2))                      //previous value's denominator
  JUMP(L_less_loop)
L_less_first_arg_is_integer:
  MOV(R5, INDD(R4, 1))
  MOV(R6, IMM(1))
  MOV(R8, IMM(1))
L_less_loop:
  CMP(R2, R1)
  JUMP_EQ(L_less_loop_end)
  MOV(R4, FPARG(R3))                        //R7 is the current arg to check
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_less_loop_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  MOV(R7, INDD(R4, 1))                      //current value's numerator
  MOV(R8, INDD(R4, 2))                      //current value's denominator
  MOV(R9, R5)
  MOV(R10, R6)
  MUL(R9, R8)
  MUL(R10, R7)
  CMP(R9, R10)
  JUMP_GE(L_less_false)
  JUMP(L_less_loop_after_check)
L_less_loop_integer:
  MOV(R7, INDD(R4, 1))
  MOV(R9, R5)
  MOV(R10, R6)
  MUL(R10, R7)
  CMP(R9, R10)
  JUMP_GE(L_less_false)
L_less_loop_after_check:
  INCR(R3)
  INCR(R2)
  MOV(R5, R7)
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_less_prev_is_integer)
  MOV(R6, R8)
L_less_prev_is_integer:
  JUMP(L_less_loop)
L_less_loop_end:
L_less_true:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_less_end)
L_less_false:
  MOV(R0, IMM(MEM_START + 2))
L_less_end:

	POP(FP)
  RETURN
L_exit_less:




//=
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_equals))
MOV(IND(FREE_VAR_TAB_START + 3), R0)
JUMP(L_exit_equals)
L_equals:
  PUSH(FP)
  MOV(FP, SP)
  MOV(R1, FPARG(1))                   //number of arguments
  CMP(R1, IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  CMP(R1, IMM(1))
  JUMP_EQ(L_equal_true)
  MOV(R2, IMM(1))                     //loop counter
  MOV(R3, IMM(2))
  MOV(R4, FPARG(R3))
  INCR(R3)
  MOV(R5, IND(R4))
L_equal_loop:
  CMP(R2, R1)
  JUMP_EQ(L_equal_loop_end)
  MOV(R6, FPARG(R3))
  CMP(IND(R6), IMM(T_INTEGER))
  JUMP_EQ(L_equal_loop_integer)
  CMP(IND(R6), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  CMP(IND(R6), R5)
  JUMP_NE(L_equal_false)
  MOV(R8, INDD(R6, 1))
  CMP(R8, INDD(R4, 1))
  JUMP_NE(L_equal_false)
  MOV(R8, INDD(R4, 2))
  CMP(R8, INDD(R2, 2))
  JUMP_NE(L_equal_false)
  JUMP(L_equal_loop_after_check)
L_equal_loop_integer:
  CMP(IND(R6), R5)
  JUMP_NE(L_equal_false)
  MOV(R7, INDD(R6, 1))
  CMP(INDD(R4, 1), R7)
  JUMP_NE(L_equal_false)
L_equal_loop_after_check:
  INCR(R3)
  INCR(R2)
  JUMP(L_equal_loop)
L_equal_loop_end:
L_equal_true:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_equal_end)
L_equal_false:
  MOV(R0, IMM(MEM_START + 2))
L_equal_end:

	POP(FP)
  RETURN
L_exit_equals:




//>
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_greater))
MOV(IND(FREE_VAR_TAB_START + 4), R0)
JUMP(L_exit_greater)
L_greater:
  PUSH(FP)
  MOV(FP, SP)
  MOV(R1, FPARG(1))                   //number of arguments
  CMP(R1, IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  CMP(R1, IMM(1))
  JUMP_EQ(L_greater_true)
  MOV(R2, IMM(1))                     //loop counter
  MOV(R3, IMM(2))
  MOV(R4, FPARG(R3))
  INCR(R3)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_greater_first_arg_is_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  MOV(R5, INDD(R4, 1))                      //previous value's numerator
  MOV(R6, INDD(R4, 2))                      //previous value's denominator
  JUMP(L_greater_loop)
L_greater_first_arg_is_integer:
  MOV(R5, INDD(R4, 1))
  MOV(R6, IMM(1))
  MOV(R8, IMM(1))
L_greater_loop:
  CMP(R2, R1)
  JUMP_EQ(L_greater_loop_end)
  MOV(R4, FPARG(R3))                        //R7 is the current arg to check
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_greater_loop_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  MOV(R7, INDD(R4, 1))                      //current value's numerator
  MOV(R8, INDD(R4, 2))                      //current value's denominator
  MOV(R9, R5)
  MOV(R10, R6)
  MUL(R9, R8)
  MUL(R10, R7)
  CMP(R9, R10)
  JUMP_LE(L_greater_false)
  JUMP(L_greater_loop_after_check)
L_greater_loop_integer:
  MOV(R7, INDD(R4, 1))
  MOV(R9, R5)
  MOV(R10, R6)
  MUL(R10, R7)
  CMP(R9, R10)
  JUMP_LE(L_greater_false)
L_greater_loop_after_check:
  INCR(R3)
  INCR(R2)
  MOV(R5, R7)
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_greater_prev_is_integer)
  MOV(R6, R8)
L_greater_prev_is_integer:
  JUMP(L_greater_loop)
L_greater_loop_end:
L_greater_true:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_greater_end)
L_greater_false:
  MOV(R0, IMM(MEM_START + 2))
L_greater_end:

	POP(FP)
  RETURN
L_exit_greater:




//+
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_plus))
MOV(IND(FREE_VAR_TAB_START + 5), R0)
JUMP(L_exit_plus)
L_plus:
  PUSH(FP)
  MOV(FP, SP)
  PUSH(IMM(3))
  CALL(MALLOC)
  DROP(1)
  MOV(IND(R0), IMM(T_FRACTION))
  MOV(INDD(R0, 1), IMM(0))                  //initial value
  MOV(INDD(R0, 2), IMM(0))
  MOV(R1, IMM(0))                           //loop counter
  MOV(R2, FPARG(1))                         //number of iterations
  MOV(R3, IMM(2))                           //FPARG index of the current argument
  MOV(R15, IMM(0))                          //a flag marking if addition was made
L_plus_loop:
  CMP(R1, R2)
  JUMP_EQ(L_plus_loop_end)
  MOV(R4, FPARG(R3))
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_plus_is_an_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  JUMP(L_plus_is_a_fraction)
L_plus_is_an_integer:
  CMP(R15, IMM(0))
  JUMP_NE(L_plus_integer_not_first_addition)
  MOV(INDD(R0, 2), IMM(1))
  MOV(R15, IMM(1))
L_plus_integer_not_first_addition:
  MOV(R5, INDD(R4, 1))                      //number to add
  MOV(R6, INDD(R0, 2))                      //accumulator's denominator
  MUL(R5, R6)
  ADD(INDD(R0, 1), R5)
  JUMP(L_plus_after_addition)
L_plus_is_a_fraction:
  CMP(R15, IMM(0))
  JUMP_NE(L_plus_fraction_not_first_addition)
  MOV(INDD(R0, 2), IMM(1))
  MOV(R15, IMM(1))
L_plus_fraction_not_first_addition:
  MOV(R5, INDD(R4, 1))
  MOV(R6, INDD(R4, 2))
  MOV(R7, INDD(R0, 1))
  MOV(R8, INDD(R0, 2))
  MUL(INDD(R0, 2), R6)
  MUL(R7, R6)
  MUL(R5, R8)
  ADD(R7, R5)
  MOV(INDD(R0, 1), R7)
L_plus_after_addition:
  INCR(R3)
  INCR(R1)
  PUSH(R0)
  PUSH(INDD(R0, 2))
  PUSH(INDD(R0, 1))
  CALL(GCD)
  DROP(2)
  MOV(R14, R0)
  POP(R0)
  DIV(INDD(R0, 1), R14)
  DIV(INDD(R0, 2), R14)
  JUMP(L_plus_loop)
L_plus_loop_end:
  CMP(INDD(R0, 2), IMM(0))
  JUMP_EQ(L_plus_no_addition)
  CMP(INDD(R0, 2), IMM(1))
  JUMP_EQ(L_plus_result_is_integer)
  JUMP(L_after_plus)
L_plus_no_addition:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), IMM(0))
  JUMP(L_after_plus)
L_plus_result_is_integer:
  MOV(R7, INDD(R0, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R7)
L_after_plus:

	POP(FP)
  RETURN
L_exit_plus:




///
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_div))
MOV(IND(FREE_VAR_TAB_START + 6), R0)
JUMP(L_exit_div)
L_div:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  PUSH(IMM(3))
  CALL(MALLOC)
  DROP(1)
  MOV(IND(R0), IMM(T_FRACTION))
  CMP(FPARG(1), IMM(1))
  JUMP_GT(L_div_two_or_more_args)
  MOV(R1, FPARG(2))
  MOV(R2, INDD(R1, 1))
  MOV(INDD(R0, 2), R2)
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_EQ(L_div_single_arg_integer)
  MOV(R2, INDD(R1, 2))
  MOV(INDD(R0, 1), R2)
L_div_single_arg_integer:
  MOV(INDD(R0, 1), IMM(1))
  JUMP(L_div_loop_end)
L_div_two_or_more_args:
  MOV(R1, IMM(1))                           //loop counter
  MOV(R2, FPARG(1))                         //number of iterations
  MOV(R3, IMM(2))                           //FPARG index of the current argument
  MOV(R4, FPARG(R3))
  INCR(R3)
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_div_first_arg_is_int)
  MOV(R5, INDD(R4, 1))
  MOV(INDD(R0, 2), R5)
  MOV(R5, INDD(R4, 2))
  MOV(INDD(R0, 1), R5)
  JUMP(L_div_loop)
L_div_first_arg_is_int:
  MOV(R4, INDD(R4, 1))
  MOV(INDD(R0, 1), R4)
  MOV(INDD(R0, 2), IMM(1))
L_div_loop:
  CMP(R1, R2)
  JUMP_EQ(L_div_loop_end)
  MOV(R4, FPARG(R3))
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_div_is_an_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  JUMP(L_div_is_a_fraction)
L_div_is_an_integer:
  MOV(R5, INDD(R4, 1))                      //divisor
  MUL(INDD(R0, 2), R5)
  JUMP(L_div_after_division)
L_div_is_a_fraction:
  MOV(R5, INDD(R4, 1))
  MOV(R6, INDD(R4, 2))
  MUL(INDD(R0, 1), R6)
  MUL(INDD(R0, 2), R5)
L_div_after_division:
  INCR(R3)
  INCR(R1)
  PUSH(R0)
  PUSH(INDD(R0, 2))
  PUSH(INDD(R0, 1))
  CALL(GCD)
  DROP(2)
  MOV(R14, R0)
  POP(R0)
  DIV(INDD(R0, 1), R14)
  DIV(INDD(R0, 2), R14)
  JUMP(L_div_loop)
L_div_loop_end:
  CMP(INDD(R0, 2), IMM(0))
  JUMP_GT(L_div_check_integer_result)
  MUL(INDD(R0, 1), IMM(-1))
  MUL(INDD(R0, 2), IMM(-1))
L_div_check_integer_result:
  CMP(INDD(R0, 2), IMM(1))
  JUMP_NE(L_after_div)
  MOV(R7, INDD(R0, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R7)
L_after_div:

	POP(FP)
  RETURN
L_exit_div:




//*
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_mul))
MOV(IND(FREE_VAR_TAB_START + 7), R0)
JUMP(L_exit_mul)
L_mul:
  PUSH(FP)
  MOV(FP, SP)
  PUSH(IMM(3))
  CALL(MALLOC)
  DROP(1)
  MOV(IND(R0), IMM(T_FRACTION))
  MOV(INDD(R0, 1), IMM(1))                  //initial value
  MOV(INDD(R0, 2), IMM(1))
  MOV(R1, IMM(0))                           //loop counter
  MOV(R2, FPARG(1))                         //number of iterations
  MOV(R3, IMM(2))                           //FPARG index of the current argument
L_mul_loop:
  CMP(R1, R2)
  JUMP_EQ(L_mul_loop_end)
  MOV(R4, FPARG(R3))
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_mul_is_an_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  JUMP(L_mul_is_a_fraction)
L_mul_is_an_integer:
  MOV(R5, INDD(R4, 1))                      //number to add
  MUL(INDD(R0, 1), R5)
  JUMP(L_mul_after_multiplication)
L_mul_is_a_fraction:
  MOV(R5, INDD(R4, 1))
  MOV(R6, INDD(R4, 2))
  MUL(INDD(R0, 1), R5)
  MUL(INDD(R0, 2), R6)
L_mul_after_multiplication:
  INCR(R3)
  INCR(R1)
  PUSH(R0)
  PUSH(INDD(R0, 2))
  PUSH(INDD(R0, 1))
  CALL(GCD)
  DROP(2)
  MOV(R14, R0)
  POP(R0)
  DIV(INDD(R0, 1), R14)
  DIV(INDD(R0, 2), R14)
  JUMP(L_mul_loop)
L_mul_loop_end:
  CMP(INDD(R0, 2), IMM(1))
  JUMP_NE(L_after_mul)
  MOV(R7, INDD(R0, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R7)
L_after_mul:

	POP(FP)
  RETURN
L_exit_mul:




//-
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_minus))
MOV(IND(FREE_VAR_TAB_START + 8), R0)
JUMP(L_exit_minus)
L_minus:
  PUSH(FP)
  MOV(FP, SP)
  MOV(R1, FPARG(1))
  CMP(R1, IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  PUSH(IMM(3))
  CALL(MALLOC)
  DROP(1)
  MOV(IND(R0), IMM(T_FRACTION))
  MOV(INDD(R0, 1), IMM(0))                  //initial value
  MOV(INDD(R0, 2), IMM(0))
  CMP(R1, IMM(1))
  JUMP_NE(L_minus_two_args_or_more)
  MOV(R1, FPARG(2))
  MOV(R2, INDD(R1, 1))
  MOV(INDD(R0, 1), R2)
  MUL(INDD(R0, 1), IMM(-1))
  MOV(INDD(R0, 2), IMM(1))
  CMP(INDD(R1, 0), IMM(T_INTEGER))
  JUMP_EQ(L_minus_loop_end)
  MOV(R2, INDD(R1, 2))
  MOV(INDD(R0, 2), R2)
  JUMP(L_minus_loop_end)
L_minus_two_args_or_more:
  MOV(R1, IMM(0))                           //loop counter
  MOV(R2, FPARG(1))                         //number of iterations
  MOV(R3, IMM(2))                           //FPARG index of the current argument
  MOV(R15, IMM(0))                          //a flag marking if addition was made
L_minus_loop:
  CMP(R1, R2)
  JUMP_EQ(L_minus_loop_end)
  MOV(R4, FPARG(R3))
  CMP(IND(R4), IMM(T_INTEGER))
  JUMP_EQ(L_minus_is_an_integer)
  CMP(IND(R4), IMM(T_FRACTION))
  JUMP_NE(EXCEPTION_NOT_A_NUMBER)
  JUMP(L_minus_is_a_fraction)
L_minus_is_an_integer:
  CMP(R15, IMM(0))
  JUMP_NE(L_minus_integer_not_first_subtraction)
  MOV(R13, INDD(R4, 1))
  MOV(INDD(R0, 1), R13)
  MOV(INDD(R0, 2), IMM(1))
  MOV(R15, IMM(1))
  INCR(R1)
  INCR(R3)
  JUMP(L_minus_loop)
L_minus_integer_not_first_subtraction:
  MOV(R5, INDD(R4, 1))                      //number to subtract
  MOV(R6, INDD(R0, 2))                      //accumulator's denominator
  MUL(R5, R6)
  SUB(INDD(R0, 1), R5)
  JUMP(L_minus_after_addition)
L_minus_is_a_fraction:
  CMP(R15, IMM(0))
  JUMP_NE(L_minus_fraction_not_first_subtraction)
  MOV(R13, INDD(R4, 1))
  MOV(INDD(R0, 1), R13)
  MOV(R13, INDD(R4, 2))
  MOV(INDD(R0, 2), R13)
  INCR(R1)
  INCR(R3)
  MOV(R15, IMM(1))
  JUMP(L_minus_loop)
L_minus_fraction_not_first_subtraction:
  MOV(R5, INDD(R4, 1))
  MOV(R6, INDD(R4, 2))
  MOV(R7, INDD(R0, 1))
  MOV(R8, INDD(R0, 2))
  MUL(R7, R6)
  MUL(R5, R8)
  SUB(R7, R5)
  MOV(INDD(R0, 1), R7)
  MUL(INDD(R0, 2), R6)
L_minus_after_addition:
  INCR(R3)
  INCR(R1)
  PUSH(R0)
  PUSH(INDD(R0, 2))
  PUSH(INDD(R0, 1))
  CALL(GCD)
  DROP(2)
  MOV(R14, R0)
  POP(R0)
  DIV(INDD(R0, 1), R14)
  DIV(INDD(R0, 2), R14)
  JUMP(L_minus_loop)
L_minus_loop_end:
  CMP(INDD(R0, 2), IMM(0))
  JUMP_EQ(L_minus_no_subtraction)
  CMP(INDD(R0, 2), IMM(1))
  JUMP_EQ(L_minus_result_is_integer)
  JUMP(L_after_minus)
L_minus_no_subtraction:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), IMM(0))
  JUMP(L_after_minus)
L_minus_result_is_integer:
  MOV(R7, INDD(R0, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R7)
L_after_minus:

	POP(FP)
  RETURN
L_exit_minus:




//boolean?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_boolean))
MOV(IND(FREE_VAR_TAB_START + 9), R0)
JUMP(L_exit_boolean)
L_boolean:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_BOOL))
  JUMP_NE(L_not_bool)
L_is_bool:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_bool)
L_not_bool:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_bool:
	POP(FP)
  RETURN
L_exit_boolean:




//car
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_car))
MOV(IND(FREE_VAR_TAB_START + 10), R0)
JUMP(L_exit_car)
L_car:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_PAIR))
  JUMP_NE(EXCEPTION_NOT_A_PAIR)
  MOV(R0, INDD(R0, 1))
	POP(FP)
  RETURN
L_exit_car:




//cdr
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_cdr))
MOV(IND(FREE_VAR_TAB_START + 11), R0)
JUMP(L_exit_cdr)
L_cdr:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_PAIR))
  JUMP_NE(EXCEPTION_NOT_A_PAIR)
  MOV(R0, INDD(R0, 2))
	POP(FP)
  RETURN
L_exit_cdr:




//char->integer
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_chartointeger))
MOV(IND(FREE_VAR_TAB_START + 12), R0)
JUMP(L_exit_chartointeger)
L_chartointeger:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(INDD(R1, 0), IMM(T_CHAR))
  JUMP_NE(EXCEPTION_NOT_A_CHAR)
  MOV(R1, INDD(R1, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R1)

	POP(FP)
  RETURN
L_exit_chartointeger:




//char?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_char))
MOV(IND(FREE_VAR_TAB_START + 13), R0)
JUMP(L_exit_char)
L_char:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_CHAR))
  JUMP_NE(L_not_a_char)
L_is_char:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_char)
L_not_a_char:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_char:
	POP(FP)
  RETURN
L_exit_char:




//cons
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_cons))
MOV(IND(FREE_VAR_TAB_START + 14), R0)
JUMP(L_exit_cons)
L_cons:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  PUSH(IMM(3))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_PAIR))
  MOV(R1, FPARG(2))
  MOV(INDD(R0, 1), R1)        //car
  MOV(R1, FPARG(3))
  MOV(INDD(R0, 2), R1)        //cdr

	POP(FP)
  RETURN
L_exit_cons:




//denominator
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_denominator))
MOV(IND(FREE_VAR_TAB_START + 15), R0)
JUMP(L_exit_denominator)
L_denominator:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_FRACTION))
  JUMP_EQ(L_is_fraction_for_denominator)
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_A_FRACTION)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), IMM(1))
  JUMP(L_denominator_end)
L_is_fraction_for_denominator:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(R1, INDD(R1, 2))
  MOV(INDD(R0, 1), R1)
L_denominator_end:
	POP(FP)
  RETURN
L_exit_denominator:




//eq?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_eq))
MOV(IND(FREE_VAR_TAB_START + 16), R0)
JUMP(L_exit_eq)
L_eq:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  MOV(R2, FPARG(3))
  MOV(R3, IND(R1))
  MOV(R4, IND(R2))
  CMP(R3, R4)            // compare type tags
  JUMP_NE(L_eq_false)
  CMP(R4, IMM(T_VOID))
  JUMP_EQ(L_eq_true)
  CMP(R4, IMM(T_NIL))
  JUMP_EQ(L_eq_true)
  CMP(R4, IMM(T_BOOL))
  JUMP_EQ(L_eq_bool)
  CMP(R4, IMM(T_CHAR))
  JUMP_EQ(L_eq_char)
  CMP(R4, IMM(T_INTEGER))
  JUMP_EQ(L_eq_integer)
  CMP(R4, IMM(T_FRACTION))
  JUMP_EQ(L_eq_fraction)
  CMP(R4, IMM(T_STRING))
  JUMP_EQ(L_eq_string)
  CMP(R4, IMM(T_SYMBOL))
  JUMP_EQ(L_eq_symbol)
  CMP(R4, IMM(T_PAIR))
  JUMP_EQ(L_eq_pair)
  CMP(R4, IMM(T_VECTOR))
  JUMP_EQ(L_eq_vector)
L_eq_bool:
  MOV(R3, INDD(R1, 1))
  MOV(R4, INDD(R2, 1))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_char:
  MOV(R3, INDD(R1, 1))
  MOV(R4, INDD(R2, 1))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_integer:
  MOV(R3, INDD(R1, 1))
  MOV(R4, INDD(R2, 1))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_fraction:
  MOV(R3, INDD(R1, 1))
  MOV(R4, INDD(R2, 1))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  MOV(R3, INDD(R1, 2))
  MOV(R4, INDD(R2, 2))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_string:
  CMP(R1, R2)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_symbol:
  MOV(R3, INDD(R1, 1))
  MOV(R4, INDD(R2, 1))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_pair:
  CMP(R1, R2)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_vector:
  CMP(R1, R2)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_closure:
  MOV(R3, INDD(R1, 1))
  MOV(R4, INDD(R2, 1))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  MOV(R3, INDD(R1, 2))
  MOV(R4, INDD(R2, 2))
  CMP(R3, R4)
  JUMP_NE(L_eq_false)
  JUMP(L_eq_true)
L_eq_true:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_eq_end)
L_eq_false:
  MOV(R0, IMM(MEM_START + 2))
L_eq_end:
  
	POP(FP)
  RETURN
L_exit_eq:




//integer?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_integer))
MOV(IND(FREE_VAR_TAB_START + 17), R0)
JUMP(L_exit_integer)
L_integer:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_INTEGER))
  JUMP_NE(L_not_integer)
L_is_integer:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_integer)
L_not_integer:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_integer:
	POP(FP)
  RETURN
L_exit_integer:




//integer->char
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_integertochar))
MOV(IND(FREE_VAR_TAB_START + 18), R0)
JUMP(L_exit_integertochar)
L_integertochar:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(INDD(R1, 0), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R1, INDD(R1, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_CHAR))
  MOV(INDD(R0, 1), R1)

	POP(FP)
  RETURN
L_exit_integertochar:




//make-string
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_make_string))
MOV(IND(FREE_VAR_TAB_START + 20), R0)
JUMP(L_exit_make_string)
L_make_string:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_GT(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  CMP(FPARG(1), IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R1, INDD(R1, 1))
  ADD(R1, IMM(2))
  PUSH(R1)
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_STRING))
  SUB(R1, IMM(2))
  MOV(INDD(R0, 1), R1)        //string length
  MOV(R4, IMM(0))             //loop counter
  MOV(R5, IMM(2))             //new val displacement
  CMP(FPARG(1), IMM(1))
  JUMP_EQ(L_make_string_default_value)
  MOV(R2, FPARG(3))
  CMP(INDD(R2, 0), IMM(T_CHAR))
  JUMP_NE(EXCEPTION_NOT_A_CHAR)
  MOV(R2, INDD(R2, 1))
  JUMP(L_make_string_populate)
L_make_string_default_value:
  MOV(R2, IMM(0))
L_make_string_populate:
  CMP(R4, R1)
  JUMP_EQ(L_make_string_populate_end)
  MOV(INDD(R0, R5), R2)
  INCR(R4)
  INCR(R5)
  JUMP(L_make_string_populate)
L_make_string_populate_end:

	POP(FP)
  RETURN
L_exit_make_string:




//make-vector
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_make_vector))
MOV(IND(FREE_VAR_TAB_START + 21), R0)
JUMP(L_exit_make_vector)
L_make_vector:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_GT(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  CMP(FPARG(1), IMM(0))
  JUMP_EQ(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R1, INDD(R1, 1))
  ADD(R1, IMM(2))
  PUSH(R1)
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_VECTOR))
  SUB(R1, IMM(2))
  MOV(INDD(R0, 1), R1)        //vector length
  MOV(R4, IMM(0))             //loop counter
  MOV(R5, IMM(2))             //new val displacement
  CMP(FPARG(1), IMM(1))
  JUMP_EQ(L_make_vector_default_value)
  MOV(R2, FPARG(3))
  JUMP(L_make_vector_populate)
L_make_vector_default_value:
  PUSH(R0)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(R2, R0)
  POP(R0)
  MOV(INDD(R2, 0), IMM(T_INTEGER))
  MOV(INDD(R2, 1), IMM(0))
L_make_vector_populate:
  CMP(R4, R1)
  JUMP_EQ(L_make_vector_populate_end)
  MOV(INDD(R0, R5), R2)
  INCR(R4)
  INCR(R5)
  JUMP(L_make_vector_populate)
L_make_vector_populate_end:

	POP(FP)
  RETURN
L_exit_make_vector:




//null?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_null))
MOV(IND(FREE_VAR_TAB_START + 24), R0)
JUMP(L_exit_null)
L_null:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_NIL))
  JUMP_NE(L_not_null)
L_is_null:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_null)
L_not_null:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_null:
	POP(FP)
  RETURN
L_exit_null:




//number?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_number))
MOV(IND(FREE_VAR_TAB_START + 25), R0)
JUMP(L_exit_number)
L_number:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_INTEGER))
  JUMP_EQ(L_is_number)
  CMP(IND(R0), IMM(T_FRACTION))
  JUMP_NE(L_not_number)
L_is_number:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_number)
L_not_number:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_number:
	POP(FP)
  RETURN
L_exit_number:




//numerator
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_numerator))
MOV(IND(FREE_VAR_TAB_START + 26), R0)
JUMP(L_exit_numerator)
L_numerator:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_FRACTION))
  JUMP_EQ(L_is_fraction_for_numerator)
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_A_FRACTION)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(R1, INDD(R1, 1))
  MOV(INDD(R0, 1), R1)
  JUMP(L_numerator_end)
L_is_fraction_for_numerator:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(R1, INDD(R1, 1))
  MOV(INDD(R0, 1), R1)
L_numerator_end:
	POP(FP)
  RETURN
L_exit_numerator:




//pair?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_pair))
MOV(IND(FREE_VAR_TAB_START + 27), R0)
JUMP(L_exit_pair)
L_pair:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_PAIR))
  JUMP_NE(L_not_a_pair)
L_is_pair:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_pair)
L_not_a_pair:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_pair:
	POP(FP)
  RETURN
L_exit_pair:




//procedure?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_procedure))
MOV(IND(FREE_VAR_TAB_START + 28), R0)
JUMP(L_exit_procedure)
L_procedure:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_CLOSURE))
  JUMP_NE(L_not_a_procedure)
L_is_procedure:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_procedure)
L_not_a_procedure:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_procedure:
	POP(FP)
  RETURN
L_exit_procedure:




//rational?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_rational))
MOV(IND(FREE_VAR_TAB_START + 29), R0)
JUMP(L_exit_rational)
L_rational:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_FRACTION))
  JUMP_EQ(L_is_fraction)
  CMP(IND(R0), IMM(T_INTEGER))
  JUMP_NE(L_not_a_fraction)
L_is_fraction:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_fraction)
L_not_a_fraction:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_fraction:
	POP(FP)
  RETURN
L_exit_rational:




//remainder
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_remainder))
MOV(IND(FREE_VAR_TAB_START + 30), R0)
JUMP(L_exit_remainder)
L_remainder:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R2, FPARG(3))
  CMP(IND(R2), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R1, INDD(R1, 1))
  MOV(R2, INDD(R2, 1))
  REM(R1, R2)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R1)
L_remainder_end:
	POP(FP)
  RETURN
L_exit_remainder:




//set-car!
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_set_car))
MOV(IND(FREE_VAR_TAB_START + 31), R0)
JUMP(L_exit_set_car)
L_set_car:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_PAIR))
  JUMP_NE(EXCEPTION_NOT_A_PAIR)
  MOV(R1, FPARG(3))
  MOV(INDD(R0, 1), R1)
  MOV(R0, IMM(MEM_START + 0))

	POP(FP)
  RETURN
L_exit_set_car:




//set-cdr!
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_set_cdr))
MOV(IND(FREE_VAR_TAB_START + 32), R0)
JUMP(L_exit_set_cdr)
L_set_cdr:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_PAIR))
  JUMP_NE(EXCEPTION_NOT_A_PAIR)
  MOV(R1, FPARG(3))
  MOV(INDD(R0, 2), R1)
  MOV(R0, IMM(MEM_START + 0))

	POP(FP)
  RETURN
L_exit_set_cdr:




//string-length
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_string_length))
MOV(IND(FREE_VAR_TAB_START + 33), R0)
JUMP(L_exit_string_length)
L_string_length:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_STRING))
  JUMP_NE(EXCEPTION_NOT_A_STRING)
  MOV(R1, INDD(R1, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R1)
L_string_length_end:
	POP(FP)
  RETURN
L_exit_string_length:




//string-ref
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_string_ref))
MOV(IND(FREE_VAR_TAB_START + 34), R0)
JUMP(L_exit_string_ref)
L_string_ref:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_STRING))
  JUMP_NE(EXCEPTION_NOT_A_STRING)
  MOV(R1, FPARG(3))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R1, INDD(R1, 1))
  MOV(R2, INDD(R0, 1))
  CMP(R1, 0)
  JUMP_LT(EXCEPTION_BAD_INDEX)
  CMP(R1, R2)
  JUMP_GE(EXCEPTION_BAD_INDEX)
  ADD(R1, IMM(2))
  MOV(R2, INDD(R0, R1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_CHAR))
  MOV(INDD(R0, 1), R2)

	POP(FP)
  RETURN
L_exit_string_ref:




//string-set!
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_string_set))
MOV(IND(FREE_VAR_TAB_START + 35), R0)
JUMP(L_exit_string_set)
L_string_set:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(3))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_STRING))
  JUMP_NE(EXCEPTION_NOT_A_STRING)
  MOV(R1, FPARG(3))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R2, FPARG(4))
  MOV(R1, INDD(R1, 1))
  MOV(R3, INDD(R0, 1))
  CMP(R1, 0)
  JUMP_LT(EXCEPTION_BAD_INDEX)
  CMP(R1, R3)
  JUMP_GE(EXCEPTION_BAD_INDEX)
  ADD(R1, IMM(2))
  CMP(INDD(R2, 0), IMM(T_CHAR))
  JUMP_NE(EXCEPTION_NOT_A_CHAR)
  MOV(R2, INDD(R2, 1))
  MOV(INDD(R0, R1), R2)
  MOV(R0, IMM(MEM_START + 0))

	POP(FP)
  RETURN
L_exit_string_set:




//string->symbol
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_stringtosymbol))
MOV(IND(FREE_VAR_TAB_START + 36), R0)
JUMP(L_exit_stringtosymbol)
L_stringtosymbol:
  PUSH(FP)
  MOV(FP, SP)
  MOV(R15, IMM(0))
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_STRING))
  JUMP_NE(EXCEPTION_NOT_A_STRING)
  MOV(R2, IND(1))                            //curr
  CMP(IND(R2), IMM(T_NIL))
  JUMP_NE(L_stringtosymbol_not_first_link)
  MOV(R15, IMM(1))
  JUMP(L_stringtosymbol_not_found)
L_stringtosymbol_not_first_link:
  MOV(R3, INDD(R2, 1))                        //next
L_stringtosymbol_search_loop:
  MOV(R9, IND(R2))
  MOV(R9, INDD(R9, 1))
  CMP(R9, R1)                                 //R1 is the input string, R9 is the string of the current link
  JUMP_EQ(L_stringtosymbol_found)
  MOV(R4, INDD(R1, 1))                        //input string length
  MOV(R5, INDD(R9, 1))                        //curr symbol's string representation length
  CMP(R4, R5)
  JUMP_NE(L_stringtosymbol_next_link)
  MOV(R6, IMM(2))
L_stringtosymbol_deep_comparison_loop:
  CMP(R4, IMM(0))
  JUMP_EQ(L_stringtosymbol_deep_comparison_loop_end)
  MOV(R7, INDD(R1, R6))
  CMP(R7, INDD(R9, R6))
  JUMP_NE(L_stringtosymbol_next_link)
  INCR(R6)
  DECR(R4)
  JUMP(L_stringtosymbol_deep_comparison_loop)
L_stringtosymbol_deep_comparison_loop_end:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_SYMBOL))
  MOV(INDD(R0, 1), R9)
  JUMP(L_stringtosymbol_end)
L_stringtosymbol_next_link:
  MOV(R2, R3)
  CMP(IND(R2), IMM(T_NIL))
  JUMP_EQ(L_stringtosymbol_not_found)
  MOV(R3, INDD(R3, 1))
  JUMP(L_stringtosymbol_search_loop)
L_stringtosymbol_found:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_SYMBOL))
  MOV(INDD(R0, 1), R9)
  JUMP(L_stringtosymbol_end)
L_stringtosymbol_not_found:
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(R4, R0)
  MOV(INDD(R4, 0), IMM(T_SYMBOL))
  MOV(INDD(R4, 1), R1)
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(R5, R0)
  MOV(INDD(R5, 0), R4)
  MOV(INDD(R5, 1), IMM(MEM_START + 1))
  MOV(INDD(R2, 1), R5)
  CMP(R15, IMM(1))
  JUMP_NE(L_stringtosymbol_not_first_link2)
  MOV(ADDR(1), R0)
L_stringtosymbol_not_first_link2:
  MOV(R0, R4)
L_stringtosymbol_end:

	POP(FP)
  RETURN
L_exit_stringtosymbol:




//string?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_string))
MOV(IND(FREE_VAR_TAB_START + 37), R0)
JUMP(L_exit_string)
L_string:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_STRING))
  JUMP_NE(L_not_a_string)
L_is_string:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_string)
L_not_a_string:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_string:
	POP(FP)
  RETURN
L_exit_string:




//symbol?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_symbol))
MOV(IND(FREE_VAR_TAB_START + 38), R0)
JUMP(L_exit_symbol)
L_symbol:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_SYMBOL))
  JUMP_NE(L_not_a_symbol)
L_is_symbol:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_symbol)
L_not_a_symbol:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_symbol:
	POP(FP)
  RETURN
L_exit_symbol:




//symbol->string
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_symboltostring))
MOV(IND(FREE_VAR_TAB_START + 39), R0)
JUMP(L_exit_symboltostring)
L_symboltostring:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_SYMBOL))
  JUMP_NE(EXCEPTION_NOT_A_SYMBOL)
  MOV(R0, INDD(R0, 1))

	POP(FP)
  RETURN
L_exit_symboltostring:




//vector
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_vector))
MOV(IND(FREE_VAR_TAB_START + 40), R0)
JUMP(L_exit_vector)
L_vector:
  PUSH(FP)
  MOV(FP, SP)
  MOV(R1, FPARG(1))
  MOV(R2, FPARG(1))         //number of args, number of iterations in the loop
  ADD(R1, IMM(2))
  PUSH(R1)
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_VECTOR))
  MOV(INDD(R0, 1), R2)
  MOV(R3, IMM(0))          //arg number
  MOV(R4, IMM(2))          //FPARG displacement
L_insert_to_vector:
  CMP(R3, R2)              //loop entry condition
  JUMP_EQ(L_insertion_end)
  MOV(R5, FPARG(R4))       //temp
  MOV(INDD(R0, R4), R5)
  INCR(R3)
  INCR(R4)
  JUMP(L_insert_to_vector)

L_insertion_end:
L_vector_end:
	POP(FP)
  RETURN
L_exit_vector:




//vector-length
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_vector_length))
MOV(IND(FREE_VAR_TAB_START + 41), R0)
JUMP(L_exit_vector_length)
L_vector_length:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R1, FPARG(2))
  CMP(IND(R1), IMM(T_VECTOR))
  JUMP_NE(EXCEPTION_NOT_A_VECTOR)
  MOV(R1, INDD(R1, 1))
  PUSH(IMM(2))
  CALL(MALLOC)
  DROP(1)
  MOV(INDD(R0, 0), IMM(T_INTEGER))
  MOV(INDD(R0, 1), R1)
L_vector_length_end:
	POP(FP)
  RETURN
L_exit_vector_length:




//vector-ref
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_vector_ref))
MOV(IND(FREE_VAR_TAB_START + 42), R0)
JUMP(L_exit_vector_ref)
L_vector_ref:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(2))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_VECTOR))
  JUMP_NE(EXCEPTION_NOT_A_VECTOR)
  MOV(R1, FPARG(3))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R1, INDD(R1, 1))
  MOV(R2, INDD(R0, 1))
  CMP(R1, 0)
  JUMP_LT(EXCEPTION_BAD_INDEX)
  CMP(R1, R2)
  JUMP_GE(EXCEPTION_BAD_INDEX)
  ADD(R1, IMM(2))
  MOV(R0, INDD(R0, R1))

	POP(FP)
  RETURN
L_exit_vector_ref:




//vector-set!
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_vector_set))
MOV(IND(FREE_VAR_TAB_START + 43), R0)
JUMP(L_exit_vector_set)
L_vector_set:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(3))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_VECTOR))
  JUMP_NE(EXCEPTION_NOT_A_VECTOR)
  MOV(R1, FPARG(3))
  CMP(IND(R1), IMM(T_INTEGER))
  JUMP_NE(EXCEPTION_NOT_AN_INTEGER)
  MOV(R2, FPARG(4))
  MOV(R1, INDD(R1, 1))
  MOV(R3, INDD(R0, 1))
  CMP(R1, 0)
  JUMP_LT(EXCEPTION_BAD_INDEX)
  CMP(R1, R3)
  JUMP_GE(EXCEPTION_BAD_INDEX)
  ADD(R1, IMM(2))
  MOV(INDD(R0, R1), R2)
  MOV(R0, IMM(MEM_START + 0))

	POP(FP)
  RETURN
L_exit_vector_set:




//vector?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_isVector))
MOV(IND(FREE_VAR_TAB_START + 44), R0)
JUMP(L_exit_isVector)
L_isVector:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_VECTOR))
  JUMP_NE(L_not_a_vector)
L_is_vector:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_vector)
L_not_a_vector:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_vector:
	POP(FP)
  RETURN
L_exit_isVector:




//zero?
PUSH(IMM(3))
CALL(MALLOC)
DROP(1)
MOV(INDD(R0, 0), IMM(T_CLOSURE))
MOV(INDD(R0, 1), IMM(0))
MOV(INDD(R0, 2), LABEL(L_zero))
MOV(IND(FREE_VAR_TAB_START + 45), R0)
JUMP(L_exit_zero)
L_zero:
  PUSH(FP)
  MOV(FP, SP)
  CMP(FPARG(1), IMM(1))
  JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
  MOV(R0, FPARG(2))
  CMP(IND(R0), IMM(T_INTEGER))
  JUMP_EQ(L_check_if_zero)
  CMP(IND(R0), IMM(T_FRACTION))
  JUMP_EQ(L_check_if_zero)
  JUMP(EXCEPTION_NOT_A_NUMBER)
L_check_if_zero:
  CMP(INDD(R0, 1), IMM(0))
  JUMP_NE(L_not_a_zero)
L_is_zero:
  MOV(R0, IMM(MEM_START + 4))
  JUMP(L_after_is_zero)
L_not_a_zero:
  MOV(R0, IMM(MEM_START + 2))
L_after_is_zero:
	POP(FP)
  RETURN
L_exit_zero:





// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- PROGRAM ENTRY POINT *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-



  PUSH(IMM(0))
  PUSH(IMM(0))
  PUSH(IMM(0))
  PUSH(FP)
  MOV(FP, SP)



  L_simple_env_expansion_7:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_7:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_7)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_7)
L_simple_env_expand_end_7:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_7:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_7)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_7)
L_simple_param_copy_end_7:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_7))
	JUMP(L_lambda_simple_end_7)
L_lambda_simple_7:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(2))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_if_else_41:
L_applic_90:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_90:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_41)
MOV(R0, FPARG(1 + 2))
	JUMP(L_if_end_41)
L_else_41: 
L_applic_tp_10:
L_applic_88:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
L_applic_89:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 11))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_89:
	PUSH(R0)
	PUSH(IMM(2))
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
L_lapplic_end_88:
	PUSH(R0)
L_applic_87:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_87:
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 14))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_10:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_10)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_10)
L_applic_tp_drop_frame_end_10:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_10:
L_if_end_41:
	POP(FP)
	RETURN
L_lambda_simple_end_7:
	MOV(IND(FREE_VAR_TAB_START + 46), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_simple_env_expansion_8:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_8:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_8)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_8)
L_simple_env_expand_end_8:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_8:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_8)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_8)
L_simple_param_copy_end_8:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_8))
	JUMP(L_lambda_simple_end_8)
L_lambda_simple_8:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(2))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_if_else_42:
L_applic_94:
L_applic_95:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 11))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_95:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_94:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_42)
L_applic_tp_12:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_12:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_12)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_12)
L_applic_tp_drop_frame_end_12:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_12:
	JUMP(L_if_end_42)
L_else_42: 
L_applic_tp_11:
L_applic_92:
L_applic_93:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 11))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_93:
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 47))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_92:
	PUSH(R0)
L_applic_91:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_91:
	PUSH(R0)
	PUSH(IMM(2))
MOV(R0, FPARG(0 + 2))
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_11:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_11)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_11)
L_applic_tp_drop_frame_end_11:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_11:
L_if_end_42:
	POP(FP)
	RETURN
L_lambda_simple_end_8:
	MOV(IND(FREE_VAR_TAB_START + 47), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_opt_env_expansion_4:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_opt_env_expand_4:
	CMP(R3, 0)
	JUMP_EQ(L_opt_env_expand_end_4)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_opt_env_expand_4)
L_opt_env_expand_end_4:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_opt_param_copy_4:
	CMP(R4, R5)
	JUMP_EQ(L_opt_param_copy_end_4)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_opt_param_copy_4)
L_opt_param_copy_end_4:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_opt_4))
	JUMP(L_lambda_opt_end_4)
L_lambda_opt_4:
	MOV(R10, STARG(1))
	CMP(R10, IMM(0))
	JUMP_GT(L_opt_after_push_nil_4)
	JUMP_LT(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
	MOV(R1, SP)
	MOV(R2, R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(R4, R3)
	INCR(R3)
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R5, IMM(0))
L_opt_push_nil_4:
	CMP(R5, R4)
	JUMP_EQ(L_opt_push_nil_end_4)
	MOV(R13, IMM(1))
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	INCR(R5)
	JUMP(L_opt_push_nil_4)
L_opt_push_nil_end_4:
	CMP(R4, IMM(0))
	JUMP_NE(L_lambda_opt_not_variadic_4)
	MOV(R13, IMM(1))
L_lambda_opt_not_variadic_4:
	MOV(STACK(R1), IMM(MEM_START + 1))
	INCR(SP)
	CMP(R13, IMM(1))
	JUMP_EQ(L_lambda_opt_no_frame_drop_4)
L_opt_after_push_nil_4:
	MOV(R1, IMM(MEM_START + 1))
	MOV(R2, STARG(1))
L_opt_pack_args_4:
	CMP(R2, IMM(0))
	JUMP_EQ(L_opt_after_pack_args_4)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_PAIR))
	MOV(R3, R2)
	INCR(R3)
	MOV(INDD(R0, 1), STARG(R3))
	MOV(INDD(R0, 2), R1)
	MOV(R1, R0)
	DECR(R2)
	JUMP(L_opt_pack_args_4)
L_opt_after_pack_args_4:
	MOV(STARG(R3), R1)
	MOV(R4, STARG(1))
	MOV(R5, STARG(1))
	MOV(STARG(1), IMM (1))
	INCR(R4)
	DECR(R5)
	SUB(R5, IMM(0))
	MOV(R7, IMM(0))
	ADD(R7, IMM(4))
	MOV(R6, IMM(0))
L_lambda_opt_drop_frame_4:
	CMP(R6, R7)
	JUMP_EQ(L_lambda_opt_drop_frame_end_4)
	MOV(R8, STARG(R3))
	MOV(STARG(R4), R8)
	INCR(R6)
	DECR(R4)
	DECR(R3)
	JUMP(L_lambda_opt_drop_frame_4)
L_lambda_opt_drop_frame_end_4:
	DROP(R5)
L_lambda_opt_no_frame_drop_4:
	PUSH(FP)
	MOV(FP, SP)
L_if_else_43:
L_applic_98:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_98:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_43)
MOV(R0, FPARG(0 + 2))
	JUMP(L_if_end_43)
L_else_43: 
L_if_else_44:
L_applic_96:
L_applic_97:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 11))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_97:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_96:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_44)
L_applic_tp_14:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_14:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_14)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_14)
L_applic_tp_drop_frame_end_14:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_14:
	JUMP(L_if_end_44)
L_else_44: 
L_applic_tp_13:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	MOV(R0, INDD(FREE_VAR_TAB_START, 46))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 47))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_13:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_13)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_13)
L_applic_tp_drop_frame_end_13:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_13:
L_if_end_44:
L_if_end_43:
	POP(FP)
	RETURN
L_lambda_opt_end_4:
	MOV(IND(FREE_VAR_TAB_START + 0), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_simple_env_expansion_9:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_9:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_9)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_9)
L_simple_env_expand_end_9:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_9:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_9)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_9)
L_simple_param_copy_end_9:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_9))
	JUMP(L_lambda_simple_end_9)
L_lambda_simple_9:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(1))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_if_else_45:
L_applic_99:
	MOV(R0, IMM(MEM_START + 2))
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 16))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_99:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_45)
	MOV(R0, IMM(MEM_START + 4))
	JUMP(L_if_end_45)
L_else_45: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_45:
	POP(FP)
	RETURN
L_lambda_simple_end_9:
	MOV(IND(FREE_VAR_TAB_START + 23), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_opt_env_expansion_5:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_opt_env_expand_5:
	CMP(R3, 0)
	JUMP_EQ(L_opt_env_expand_end_5)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_opt_env_expand_5)
L_opt_env_expand_end_5:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_opt_param_copy_5:
	CMP(R4, R5)
	JUMP_EQ(L_opt_param_copy_end_5)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_opt_param_copy_5)
L_opt_param_copy_end_5:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_opt_5))
	JUMP(L_lambda_opt_end_5)
L_lambda_opt_5:
	MOV(R10, STARG(1))
	CMP(R10, IMM(0))
	JUMP_GT(L_opt_after_push_nil_5)
	JUMP_LT(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
	MOV(R1, SP)
	MOV(R2, R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(R4, R3)
	INCR(R3)
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R5, IMM(0))
L_opt_push_nil_5:
	CMP(R5, R4)
	JUMP_EQ(L_opt_push_nil_end_5)
	MOV(R13, IMM(1))
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	INCR(R5)
	JUMP(L_opt_push_nil_5)
L_opt_push_nil_end_5:
	CMP(R4, IMM(0))
	JUMP_NE(L_lambda_opt_not_variadic_5)
	MOV(R13, IMM(1))
L_lambda_opt_not_variadic_5:
	MOV(STACK(R1), IMM(MEM_START + 1))
	INCR(SP)
	CMP(R13, IMM(1))
	JUMP_EQ(L_lambda_opt_no_frame_drop_5)
L_opt_after_push_nil_5:
	MOV(R1, IMM(MEM_START + 1))
	MOV(R2, STARG(1))
L_opt_pack_args_5:
	CMP(R2, IMM(0))
	JUMP_EQ(L_opt_after_pack_args_5)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_PAIR))
	MOV(R3, R2)
	INCR(R3)
	MOV(INDD(R0, 1), STARG(R3))
	MOV(INDD(R0, 2), R1)
	MOV(R1, R0)
	DECR(R2)
	JUMP(L_opt_pack_args_5)
L_opt_after_pack_args_5:
	MOV(STARG(R3), R1)
	MOV(R4, STARG(1))
	MOV(R5, STARG(1))
	MOV(STARG(1), IMM (1))
	INCR(R4)
	DECR(R5)
	SUB(R5, IMM(0))
	MOV(R7, IMM(0))
	ADD(R7, IMM(4))
	MOV(R6, IMM(0))
L_lambda_opt_drop_frame_5:
	CMP(R6, R7)
	JUMP_EQ(L_lambda_opt_drop_frame_end_5)
	MOV(R8, STARG(R3))
	MOV(STARG(R4), R8)
	INCR(R6)
	DECR(R4)
	DECR(R3)
	JUMP(L_lambda_opt_drop_frame_5)
L_lambda_opt_drop_frame_end_5:
	DROP(R5)
L_lambda_opt_no_frame_drop_5:
	PUSH(FP)
	MOV(FP, SP)
MOV(R0, FPARG(0 + 2))
	POP(FP)
	RETURN
L_lambda_opt_end_5:
	MOV(IND(FREE_VAR_TAB_START + 19), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_simple_env_expansion_10:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_10:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_10)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_10)
L_simple_env_expand_end_10:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_10:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_10)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_10)
L_simple_param_copy_end_10:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_10))
	JUMP(L_lambda_simple_end_10)
L_lambda_simple_10:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(2))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_if_else_46:
L_applic_104:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_104:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_46)
	MOV(R0, IMM(MEM_START + 1))
	JUMP(L_if_end_46)
L_else_46: 
L_applic_tp_15:
L_applic_102:
L_applic_103:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 11))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_103:
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 48))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_102:
	PUSH(R0)
L_applic_100:
L_applic_101:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_101:
	PUSH(R0)
	PUSH(IMM(1))
MOV(R0, FPARG(0 + 2))
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_100:
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 14))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_15:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_15)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_15)
L_applic_tp_drop_frame_end_15:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_15:
L_if_end_46:
	POP(FP)
	RETURN
L_lambda_simple_end_10:
	MOV(IND(FREE_VAR_TAB_START + 48), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_opt_env_expansion_6:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_opt_env_expand_6:
	CMP(R3, 0)
	JUMP_EQ(L_opt_env_expand_end_6)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_opt_env_expand_6)
L_opt_env_expand_end_6:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_opt_param_copy_6:
	CMP(R4, R5)
	JUMP_EQ(L_opt_param_copy_end_6)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_opt_param_copy_6)
L_opt_param_copy_end_6:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_opt_6))
	JUMP(L_lambda_opt_end_6)
L_lambda_opt_6:
	MOV(R10, STARG(1))
	CMP(R10, IMM(1))
	JUMP_GT(L_opt_after_push_nil_6)
	JUMP_LT(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
	MOV(R1, SP)
	MOV(R2, R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R3, STACK(R2))
	MOV(R4, R3)
	INCR(R3)
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	MOV(R5, IMM(0))
L_opt_push_nil_6:
	CMP(R5, R4)
	JUMP_EQ(L_opt_push_nil_end_6)
	MOV(R13, IMM(1))
	MOV(R3, STACK(R2))
	MOV(STACK(R1), R3)
	DECR(R1)
	DECR(R2)
	INCR(R5)
	JUMP(L_opt_push_nil_6)
L_opt_push_nil_end_6:
	CMP(R4, IMM(0))
	JUMP_NE(L_lambda_opt_not_variadic_6)
	MOV(R13, IMM(1))
L_lambda_opt_not_variadic_6:
	MOV(STACK(R1), IMM(MEM_START + 1))
	INCR(SP)
	CMP(R13, IMM(1))
	JUMP_EQ(L_lambda_opt_no_frame_drop_6)
L_opt_after_push_nil_6:
	MOV(R1, IMM(MEM_START + 1))
	MOV(R2, STARG(1))
L_opt_pack_args_6:
	CMP(R2, IMM(1))
	JUMP_EQ(L_opt_after_pack_args_6)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_PAIR))
	MOV(R3, R2)
	INCR(R3)
	MOV(INDD(R0, 1), STARG(R3))
	MOV(INDD(R0, 2), R1)
	MOV(R1, R0)
	DECR(R2)
	JUMP(L_opt_pack_args_6)
L_opt_after_pack_args_6:
	MOV(STARG(R3), R1)
	MOV(R4, STARG(1))
	MOV(R5, STARG(1))
	MOV(STARG(1), IMM (2))
	INCR(R4)
	DECR(R5)
	SUB(R5, IMM(1))
	MOV(R7, IMM(1))
	ADD(R7, IMM(4))
	MOV(R6, IMM(0))
L_lambda_opt_drop_frame_6:
	CMP(R6, R7)
	JUMP_EQ(L_lambda_opt_drop_frame_end_6)
	MOV(R8, STARG(R3))
	MOV(STARG(R4), R8)
	INCR(R6)
	DECR(R4)
	DECR(R3)
	JUMP(L_lambda_opt_drop_frame_6)
L_lambda_opt_drop_frame_end_6:
	DROP(R5)
L_lambda_opt_no_frame_drop_6:
	PUSH(FP)
	MOV(FP, SP)
L_if_else_47:
L_applic_110:
L_applic_111:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_111:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_110:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_47)
	MOV(R0, IMM(MEM_START + 1))
	JUMP(L_if_end_47)
L_else_47: 
L_applic_tp_16:
L_applic_109:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	MOV(R0, INDD(FREE_VAR_TAB_START, 11))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 48))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_109:
	PUSH(R0)
L_applic_107:
L_applic_108:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 48))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_108:
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 1))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_107:
	PUSH(R0)
	PUSH(IMM(2))
L_simple_env_expansion_11:
	PUSH(IMM(2))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_11:
	CMP(R3, 1)
	JUMP_EQ(L_simple_env_expand_end_11)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_11)
L_simple_env_expand_end_11:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_11:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_11)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_11)
L_simple_param_copy_end_11:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_11))
	JUMP(L_lambda_simple_end_11)
L_lambda_simple_11:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(2))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_applic_tp_17:
L_applic_105:
L_applic_106:
MOV(R0, FPARG(1 + 2))
	PUSH(R0)
	MOV(R0, FPARG(0))
	MOV(R0, INDD(R0, 0))
	MOV(R0, INDD(R0, 0))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 14))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_106:
	PUSH(R0)
	MOV(R0, INDD(FREE_VAR_TAB_START, 22))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 1))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_105:
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 14))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_17:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_17)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_17)
L_applic_tp_drop_frame_end_17:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_17:
	POP(FP)
	RETURN
L_lambda_simple_end_11:
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_16:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_16)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_16)
L_applic_tp_drop_frame_end_16:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_16:
L_if_end_47:
	POP(FP)
	RETURN
L_lambda_opt_end_6:
	MOV(IND(FREE_VAR_TAB_START + 22), R0)
	MOV(R0, IMM(MEM_START + 0))


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  L_if_else_48:
L_applic_180:
	MOV(R0, IMM(MEM_START + 4))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 9))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_180:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_48)
L_if_else_49:
L_applic_179:
	MOV(R0, IMM(MEM_START + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 9))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_179:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_49)
L_if_else_50:
L_applic_177:
L_applic_178:
	MOV(R0, IMM(MEM_START + 6))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 9))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_178:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_177:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_50)
L_if_else_51:
L_applic_175:
L_applic_176:
	MOV(R0, IMM(MEM_START + 11))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 9))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_176:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_175:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_51)
L_if_else_52:
L_applic_174:
	MOV(R0, IMM(MEM_START + 16))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 38))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_174:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_52)
L_if_else_53:
L_applic_173:
	MOV(R0, INDD(FREE_VAR_TAB_START, 28))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 28))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_173:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_53)
L_if_else_54:
L_applic_170:
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
L_applic_171:
L_applic_172:
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 14))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_172:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 10))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_171:
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_170:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_54)
L_if_else_55:
L_applic_169:
	MOV(R0, IMM(MEM_START + 6))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 17))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_169:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_55)
L_if_else_56:
L_applic_168:
	MOV(R0, IMM(MEM_START + 22))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 13))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_168:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_56)
L_if_else_57:
L_applic_167:
	MOV(R0, IMM(MEM_START + 1))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 24))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_167:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_57)
L_if_else_58:
L_applic_166:
	MOV(R0, IMM(MEM_START + 24))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 37))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_166:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_58)
L_if_else_59:
L_applic_165:
	MOV(R0, IMM(MEM_START + 37))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 38))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_165:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_59)
L_if_else_60:
L_applic_164:
	MOV(R0, IMM(MEM_START + 41))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 44))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_164:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_60)
L_if_else_61:
L_applic_162:
L_applic_163:
	MOV(R0, IMM(MEM_START + 6))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 44))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_163:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_162:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_61)
L_if_else_62:
L_applic_160:
L_applic_161:
	MOV(R0, IMM(MEM_START + 51))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 37))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_161:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_160:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_62)
L_if_else_63:
L_applic_158:
L_applic_159:
	MOV(R0, IMM(MEM_START + 6))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 37))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_159:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_158:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_63)
L_if_else_64:
L_applic_156:
L_applic_157:
	MOV(R0, IMM(MEM_START + 56))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 41))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_157:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_156:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_64)
L_if_else_65:
L_applic_155:
	MOV(R0, IMM(MEM_START + 61))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 27))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_155:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_65)
L_if_else_66:
L_applic_153:
L_applic_154:
	MOV(R0, IMM(MEM_START + 1))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 27))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_154:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_153:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_66)
L_if_else_67:
L_applic_152:
	MOV(R0, IMM(MEM_START + 64))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 45))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_152:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_67)
L_if_else_68:
L_applic_150:
L_applic_151:
	MOV(R0, IMM(MEM_START + 66))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 45))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_151:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 23))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_150:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_68)
L_if_else_69:
L_applic_147:
L_applic_148:
L_applic_149:
	MOV(R0, IMM(MEM_START + 64))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 24))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 34))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_149:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 12))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_148:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 68))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_147:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_69)
L_if_else_70:
L_applic_144:
	MOV(R0, IMM(MEM_START + 70))
	PUSH(R0)
	PUSH(IMM(1))
L_simple_env_expansion_15:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_15:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_15)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_15)
L_simple_env_expand_end_15:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_15:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_15)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_15)
L_simple_param_copy_end_15:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_15))
	JUMP(L_lambda_simple_end_15)
L_lambda_simple_15:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(1))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_applic_tp_21:
L_applic_145:
L_applic_146:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 20))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_146:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 33))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_145:
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_21:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_21)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_21)
L_applic_tp_drop_frame_end_21:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_21:
	POP(FP)
	RETURN
L_lambda_simple_end_15:
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_144:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_70)
L_if_else_71:
L_applic_141:
	MOV(R0, IMM(MEM_START + 70))
	PUSH(R0)
	PUSH(IMM(1))
L_simple_env_expansion_14:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_14:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_14)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_14)
L_simple_env_expand_end_14:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_14:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_14)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_14)
L_simple_param_copy_end_14:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_14))
	JUMP(L_lambda_simple_end_14)
L_lambda_simple_14:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(1))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_applic_tp_20:
L_applic_142:
L_applic_143:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 21))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_143:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 41))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_142:
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_20:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_20)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_20)
L_applic_tp_drop_frame_end_20:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_20:
	POP(FP)
	RETURN
L_lambda_simple_end_14:
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_141:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_71)
L_if_else_72:
L_applic_139:
	MOV(R0, IMM(MEM_START + 51))
	PUSH(R0)
	PUSH(IMM(1))
L_simple_env_expansion_13:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_13:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_13)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_13)
L_simple_env_expand_end_13:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_13:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_13)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_13)
L_simple_param_copy_end_13:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_13))
	JUMP(L_lambda_simple_end_13)
L_lambda_simple_13:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(1))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_applic_tp_19:
L_applic_140:
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 42))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_140:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 49))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 16))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_19:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_19)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_19)
L_applic_tp_drop_frame_end_19:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_19:
	POP(FP)
	RETURN
L_lambda_simple_end_13:
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_139:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_72)
L_if_else_73:
L_applic_137:
L_applic_138:
	MOV(R0, IMM(MEM_START + 74))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 12))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_138:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 72))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_137:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_73)
L_if_else_74:
L_applic_130:
L_applic_136:
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 20))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_136:
	PUSH(R0)
	PUSH(IMM(1))
L_simple_env_expansion_12:
	PUSH(IMM(1))
	CALL(MALLOC)
	MOV(R1, R0)
	DROP(1)
	MOV(R2, FPARG(0))
	MOV(R3, IMM(0))
	MOV(R4, IMM(1))
L_simple_env_expand_12:
	CMP(R3, 0)
	JUMP_EQ(L_simple_env_expand_end_12)
	MOV(R5, INDD(R2, R3))
	MOV(INDD(R1, R4), R5)
	INCR(R3)
	INCR(R4)
	JUMP(L_simple_env_expand_12)
L_simple_env_expand_end_12:
	PUSH(FPARG(1))
	CALL(MALLOC)
	MOV(R3, R0)
	DROP(1)
	MOV(R4, IMM(2))
	MOV(R5, FPARG(1))
	ADD(R5, IMM(2))
L_simple_param_copy_12:
	CMP(R4, R5)
	JUMP_EQ(L_simple_param_copy_end_12)
	MOV(R9, R4)
	SUB(R9, IMM(2))
	MOV(R6, FPARG(R4))
	MOV(INDD(R3, R9), R6)
	INCR(R4)
	JUMP(L_simple_param_copy_12)
L_simple_param_copy_end_12:
	MOV(INDD(R1, 0), R3)
	PUSH(IMM(3))
	CALL(MALLOC)
	DROP(1)
	MOV(INDD(R0, 0), IMM(T_CLOSURE))
	MOV(INDD(R0, 1), R1)
	MOV(INDD(R0, 2), LABEL(L_lambda_simple_12))
	JUMP(L_lambda_simple_end_12)
L_lambda_simple_12:
	PUSH(FP)
	MOV(FP, SP)
	MOV(R15, FPARG(1))
	CMP(R15, IMM(1))
	JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)
L_applic_134:
L_applic_135:
	MOV(R0, IMM(MEM_START + 68))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 18))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_135:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 64))
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(3))
	MOV(R0, INDD(FREE_VAR_TAB_START, 35))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_134:

L_applic_132:
L_applic_133:
	MOV(R0, IMM(MEM_START + 76))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 18))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_133:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(3))
	MOV(R0, INDD(FREE_VAR_TAB_START, 35))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_132:

L_applic_tp_18:
L_applic_131:
MOV(R0, FPARG(0 + 2))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 36))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_131:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 82))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 16))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	MOV(R1, STACK(FP - 1))
	MOV(R2, STACK(FP - 2))
	MOV(R3, FP)
	MOV(R4, FP)
	SUB(R4, FPARG(1))
	SUB(R4, IMM(4))
	MOV(R5, IMM(0))
	MOV(R6, STARG(0))
	ADD(R6, IMM(2))
	MOV(R14, STARG(0))
	ADD(R14, FPARG(1))
	ADD(R14, IMM(1))
	MOV(SP, R4)
L_applic_tp_drop_frame_18:
	CMP(R5, R6)
	JUMP_EQ(L_applic_tp_drop_frame_end_18)
	MOV(R7, STACK(R3))
	PUSH(R7)
	INCR(R3)
	INCR(R5)
	JUMP(L_applic_tp_drop_frame_18)
L_applic_tp_drop_frame_end_18:
	PUSH(R2)
	MOV(FP, R1)
	JUMPA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_applic_tp_end_18:

	POP(FP)
	RETURN
L_lambda_simple_end_12:
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_130:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_74)
L_if_else_75:
L_applic_128:
L_applic_129:
	MOV(R0, IMM(MEM_START + 86))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 84))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 30))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_129:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_128:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_75)
L_if_else_76:
L_applic_126:
L_applic_127:
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	PUSH(IMM(3))
	MOV(R0, INDD(FREE_VAR_TAB_START, 7))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_127:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 88))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_126:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_76)
L_if_else_77:
L_applic_124:
L_applic_125:
	PUSH(IMM(0))
	MOV(R0, INDD(FREE_VAR_TAB_START, 7))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_125:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_124:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_77)
L_if_else_78:
L_applic_122:
L_applic_123:
	MOV(R0, IMM(MEM_START + 66))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 7))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_123:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 66))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_122:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_78)
L_if_else_79:
L_applic_120:
L_applic_121:
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	PUSH(IMM(3))
	MOV(R0, INDD(FREE_VAR_TAB_START, 5))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_121:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 88))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_120:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_79)
L_if_else_80:
L_applic_118:
L_applic_119:
	PUSH(IMM(0))
	MOV(R0, INDD(FREE_VAR_TAB_START, 5))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_119:
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 45))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_118:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_80)
L_if_else_81:
L_applic_116:
L_applic_117:
	MOV(R0, IMM(MEM_START + 66))
	PUSH(R0)
	PUSH(IMM(1))
	MOV(R0, INDD(FREE_VAR_TAB_START, 5))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_117:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 66))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_116:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_81)
L_if_else_82:
L_applic_114:
L_applic_115:
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 88))
	PUSH(R0)
	PUSH(IMM(3))
	MOV(R0, INDD(FREE_VAR_TAB_START, 8))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_115:
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	PUSH(IMM(2))
	MOV(R0, INDD(FREE_VAR_TAB_START, 3))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_114:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_82)
L_if_else_83:
L_applic_113:
	MOV(R0, IMM(MEM_START + 90))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 86))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	PUSH(IMM(5))
	MOV(R0, INDD(FREE_VAR_TAB_START, 2))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_113:
	CMP(R0, IMM(MEM_START + 2))
	JUMP_EQ(L_else_83)
L_applic_112:
	MOV(R0, IMM(MEM_START + 18))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 20))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 39))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 86))
	PUSH(R0)
	MOV(R0, IMM(MEM_START + 90))
	PUSH(R0)
	PUSH(IMM(5))
	MOV(R0, INDD(FREE_VAR_TAB_START, 4))
	CMP(R0, T_UNDEFINED)
	JUMP_EQ(EXCEPTION_UNDEFINED_VARIABLE)
	CMP(IND(R0), IMM(T_CLOSURE))
	JUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)
	PUSH(INDD(R0, 1))
	CALLA(INDD(R0, 2))
	POP(R1)
	POP(R1)
	DROP(R1)
L_lapplic_end_112:
	JUMP(L_if_end_83)
L_else_83: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_83:
	JUMP(L_if_end_82)
L_else_82: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_82:
	JUMP(L_if_end_81)
L_else_81: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_81:
	JUMP(L_if_end_80)
L_else_80: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_80:
	JUMP(L_if_end_79)
L_else_79: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_79:
	JUMP(L_if_end_78)
L_else_78: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_78:
	JUMP(L_if_end_77)
L_else_77: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_77:
	JUMP(L_if_end_76)
L_else_76: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_76:
	JUMP(L_if_end_75)
L_else_75: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_75:
	JUMP(L_if_end_74)
L_else_74: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_74:
	JUMP(L_if_end_73)
L_else_73: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_73:
	JUMP(L_if_end_72)
L_else_72: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_72:
	JUMP(L_if_end_71)
L_else_71: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_71:
	JUMP(L_if_end_70)
L_else_70: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_70:
	JUMP(L_if_end_69)
L_else_69: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_69:
	JUMP(L_if_end_68)
L_else_68: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_68:
	JUMP(L_if_end_67)
L_else_67: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_67:
	JUMP(L_if_end_66)
L_else_66: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_66:
	JUMP(L_if_end_65)
L_else_65: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_65:
	JUMP(L_if_end_64)
L_else_64: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_64:
	JUMP(L_if_end_63)
L_else_63: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_63:
	JUMP(L_if_end_62)
L_else_62: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_62:
	JUMP(L_if_end_61)
L_else_61: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_61:
	JUMP(L_if_end_60)
L_else_60: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_60:
	JUMP(L_if_end_59)
L_else_59: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_59:
	JUMP(L_if_end_58)
L_else_58: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_58:
	JUMP(L_if_end_57)
L_else_57: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_57:
	JUMP(L_if_end_56)
L_else_56: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_56:
	JUMP(L_if_end_55)
L_else_55: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_55:
	JUMP(L_if_end_54)
L_else_54: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_54:
	JUMP(L_if_end_53)
L_else_53: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_53:
	JUMP(L_if_end_52)
L_else_52: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_52:
	JUMP(L_if_end_51)
L_else_51: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_51:
	JUMP(L_if_end_50)
L_else_50: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_50:
	JUMP(L_if_end_49)
L_else_49: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_49:
	JUMP(L_if_end_48)
L_else_48: 
	MOV(R0, IMM(MEM_START + 2))
L_if_end_48:


  PUSH(R0)
  CALL(WRITE_SOB_IF_NOT_VOID)
  DROP(1)

  
  STOP_MACHINE

  return 0;
}