/*
 ============================================================================
 Name        : Arithmetic Machine
 Author      : UBC Launchpad
 Version     : 1.0
 Copyright   :
 Description : Stack-based virtual machine that performs simple arithmetic.
 ============================================================================
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE   256                                 // maximum number of values on the stack
#define PUSH(vm, v)  (vm->stack[++vm->sp] = v)           // push value onto stack
#define POP(vm)      (vm->stack[vm->sp--])               // get value from top of stack
#define NCODE(vm)    (vm->code[vm->pc++])                // get next bytecode

/* all opcodes that will be implemented. */
enum opcodes {
    HALT      = 0x00, // halt
    /* in case you can't get DCONST working, you can still try things using these: */
    DCONST_M1 = 0x0A, // push -1.0 onto stack
    DCONST_0  = 0x0B, // push 0.0 onto stack
    DCONST_1  = 0x0C, // push 1.0 onto stack
    DCONST_2  = 0x0D, // push 2.0 onto stack
    /* make sure you consider endianness */
    DCONST    = 0x0F, // push next 8 bytes onto stack as double constant
    /* arithmetic operations */
    ADD       = 0x60, // add two doubles
    SUB       = 0x61, // subtract two doubles
    MUL       = 0x62, // multiply two doubles
    DIV       = 0x64, // divide two doubles
    NEG       = 0x70, // negate an double (e.g. if -1.0 is on the stack, NEG will turn it to 1.0 on the stack)

    NOP       = 0xF0, // do nothing
    /* in a real VM, we'd use a function call for print, rather than having a special opcode */
    PRINT     = 0xF2, // pops and prints top of stack
    /* store and read from registers */
    ST1       = 0xF4, // pops top of stack and stores it in r1
    LD1       = 0xF5, // load global from r1
    ST2       = 0xF6, // pops top of stack and stores it in r2
    LD2       = 0xF7, // load global from r2
	/* added operations */
	MOV 	= 0x90, // moves value in one register to another. Register numbers will be specified in next byte.
};

/* defining our virtual machine */
typedef struct {
	double r1, r2;      // registers
	char* code;         // pointer to bytecode
	double* stack;      // stack
	int pc;             // program counter
	int sp;             // stack pointer
} VM;

VM* newVM(char* code /* pointer to bytecode */ ) {
    VM* vm = malloc(sizeof(VM));
    vm->code = code; 
    vm->pc = 0;
    vm->sp = -1;
    vm->r1 = vm->r2 = 0;    // init registers to 0
    vm->stack = malloc(sizeof(double) * STACK_SIZE);
    return vm;
}

void delVM(VM* vm){
        free(vm->stack);
        free(vm);
}

int run(VM* vm){
    for (;;) {
        unsigned char opcode = NCODE(vm);        // store next bytecode in `opcode'
        double a, b, v;                          // use these to store intermediate values when implementing opcodes below
        switch (opcode) {   // decode
        case HALT: return EXIT_SUCCESS;  // exit successfully
        case NOP: break;    // pass
        case DCONST_M1:     // push -1.0 onto stack
            // TODO: implement this.
		v = -1;
		PUSH(vm, v);
            break;
        case DCONST_0:      // push 0.0 onto stack
		v = 0;
		PUSH(vm, v);
            // TODO: implement this.
            break;
        case DCONST_1:      // push 1.0 onto stack
            // TODO: implement this.
		v = 1;
		PUSH(vm, v);
            break;
        case DCONST_2:      // push 2.0 onto stack
            // TODO: implement this.
		v = 2;
		PUSH(vm, v);
            break;
        case DCONST:        // reads next 8 bytes of opcode as a double, and stores it on the stack.
            // HINT: use memcpy to read next 8 bytes of code as a double. make sure you consider endianness.
		memcpy((void*) &v, vm->code, 8);
		u_int16_t  checkEndianness = 0x0102;
		if (*((char *) &checkEndianness) == 0x02) //if the first byte stored is 0x02, then I'm on a little endian machine!
		{
		// flip the bytes. unsure why this is needed, seeing as memcpy doesn't change the byte order, but the question hints at it so I think this is what it wants?
			char * bytes = &v;
			for(int i = 0; i < 8; i++)
			{
				char temp = bytes[i];
				bytes[i] = bytes[7-i];
				bytes[7-i] = temp;
			}
		}
		PUSH(vm, v);
            break;
        case ADD:           // add two doubles from top of stack and push result back onto stack
            b = POP(vm);
            a = POP(vm);
            PUSH(vm, a + b);
            break;
        case MUL:           // multiply two doubles from top of stack and push result back onto stack
            // TODO: implement this.
		b = POP(vm);
		a = POP(vm);
		PUSH(vm, a*b);
            break;
        case SUB:           // subtract two doubles from top of stack and push result back onto stack
            // TODO: implement this.
		b = POP(vm);
		a = POP(vm);
		PUSH(vm, a-b);
            break;
        case DIV:          // divide two doubles from top of stack and push result back onto stack
            //TODO: implement this.
            // HINT: make sure to deal with the division by zero case.
		b = POP(vm);
		a = POP(vm);
		if (b == 0) {printf("DivisionZeroError: %f, %f\n", b, a); return EXIT_FAILURE;}
		PUSH(vm, a/b);
            break;
        case NEG:                         // negates top of stack
            //TODO: implement this.
		b = POP(vm);
		PUSH(vm, (double) (b*-1));
            break;
        case LD1:          // put value from r1 on top of stack
            // TODO: implement this.
		v = vm->r1;
		PUSH(vm, v);
            break;
        case ST1:                         // store top of stack in r1
            // TODO: implement this.
		vm->r1 = POP(vm);
            break;
        case LD2:           // put value from r2 on top of stack
            // TODO: implement this.
            // HINT: should be similar to LD1.
		v = vm->r1;
		PUSH(vm, v);
            break;
        case ST2:                         // store top of stack in r2
            // TODO: implement this.
            // HINT: should be similar to ST1.
		vm->r2 = POP(vm);
            break;
        case PRINT:                       // print top of stack, (and discard value afterwards.)
            // TODO: implement this.
		printf("%f\n", POP(vm));
            break;
	case MOV:; // reads next byte as 0xXY where X is the number of the src register and Y is the dst register. e.g., 0x6012 would say
		  // move value in register 1 to register 2
		unsigned char byte = NCODE(vm);
		switch(byte) {
			case 0x12:
				vm->r2 = vm->r1;
				break;
			case 0x21:
				vm->r1 = vm->r2;
				break;
			default:
				printf("InvalidArgumentError: Operation %x received invalid argument %x\n", opcode, byte);
				return EXIT_FAILURE;
		}
		break;
        default:
            printf("InvalidOpcodeError: %x\n", opcode);  // terminate program at unknown opcode and show error.
            return EXIT_FAILURE;
        }

    }
    return EXIT_FAILURE;
}

int main(void) {
	/* in a real VM, we'd read bytecode from a file, but for brevity's sake we'll read
	from an array.
	*/
	// simple example: push 2 onto stack, push 1 onto stack, subtract them, print the result, exit (should print 1.0)
	char bytecode[] = { DCONST_2,
		DCONST_1,
		SUB,
		PRINT,
		HALT };
	VM* vm = newVM(bytecode /* program to execute */ );
	int exit_status = run(vm);
	printf("Exited vm with code: %d\n", exit_status);
	delVM(vm);
	char testArithmetic[] = {
		DCONST_1,
		DCONST_2,
		ADD,
		PRINT, //should print 3
		DCONST_1,
		DCONST_2,
		DIV,
		PRINT, //should print 1/2
		DCONST_1,
		DCONST_2,
		MUL,
		PRINT, //should print 2
		DCONST_1,
		DCONST_2,
		NEG,
		PRINT, //should print -2
		HALT
	};
	vm = newVM(testArithmetic);
	exit_status = run(vm);
	printf("Exited vm with code: %d\n", exit_status);
	delVM(vm);
	return exit_status;
};
