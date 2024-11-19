/* birdnest */
    #include "simple_vm.h"

    /**
     * Set E flag - Equal
     */
    void __ste(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags |= 0x08;
    }
    /**
     * Set G flag - Greater
     */
    void __stg(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags |= 0x04;
    }
    /**
     * Set H flag - Higher
     */
    void __sth(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags |= 0x02;
    }
    /**
     * Set L flag - Lower
     */
    void __stl(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags |= 0x01;
    }
    /**
     * Clear E flag - Equal
     */
    void __cle(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags &= 0x07;
    }
    /**
     * Clear G flag - Greater
     */
    void __clg(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags &= 0x0c;
    }
    /**
     * Clear H flag - Higher
     */
    void __clh(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags &= 0x0d;
    }
    /**
     * Clear L flag - Lower
     */
    void __cll(VM *vm, Opcode opcode, Args a1, Args a2) {
        vm $flags &= 0x0e;
    }

    /**
     * Move data between registers and memory
     */
    void __mov(VM *vm, Opcode opcode, Args a1, Args a2) {
        /**
         * Function used to handle the `mov` instruction
         * @param vm Virtual Machine
         * @param opcode Opcode
         * @param a1 Argument 1
         * @param a2 Argument 2
         * @return void
         */
        int16 dst; // Declare a variable for the destination register or memory location

        // The destination is the second argument (a1), cast to a 16-bit integer
        dst = $2 a1; 

        // Check if the flags indicate that the operation involves a "higher" or "lower" action.
        // If both are set, it indicates an invalid operation, so we raise an error.
        if (higher(vm) && lower(vm))
            error(vm, ErrInstr); // If both flags are set, it's an invalid instruction (error)

        // Switch statement to handle different types of `mov` operations based on the opcode
        switch (opcode) {
            /* mov ax      ; 1000 */
            case 0x08:
                /**
                 * 0x08 means the instruction is for moving data to or from the AX register.
                 * The instruction takes 1 argument, which is 2 bytes long (16 bits).
                 * Depending on the flags (higher or lower), it moves the argument to the AX register.
                 */
                if (higher(vm))
                    vm $ax = (((Reg)a1 << 8) | (vm $ax & 0xFF)); // Move the higher byte of a1 to AX
                else if (lower(vm))
                    vm $ax = ((Reg)a1 | (vm $ax & 0xFF00)); // Move the lower byte of a1 to AX
                else
                    vm $ax = (Reg)a1; // Directly move the 16-bit value into AX
                break;

            /* mov bx      ; 1001 */
            case 0x09:
                /**
                 * 0x09 means the instruction is for moving data to or from the BX register.
                 * Similar logic to the AX register handling.
                 */
                if (higher(vm))
                    vm $bx = (((Reg)a1 << 8) | (vm $bx & 0xFF)); // Move the higher byte of a1 to BX
                else if (lower(vm))
                    vm $bx = ((Reg)a1 | (vm $bx & 0xFF00)); // Move the lower byte of a1 to BX
                else
                    vm $bx = (Reg)a1; // Directly move the 16-bit value into BX
                break;

            /* mov cx      ; 1010 */
            case 0x0a:
                /**
                 * 0x0a means the instruction is for moving data to or from the CX register.
                 * Similar logic to the AX and BX registers.
                 */
                if (higher(vm))
                    vm $cx = (((Reg)a1 << 8) | (vm $cx & 0xFF)); // Move the higher byte of a1 to CX
                else if (lower(vm))
                    vm $cx = ((Reg)a1 | (vm $cx & 0xFF00)); // Move the lower byte of a1 to CX
                else
                    vm $cx = (Reg)a1; // Directly move the 16-bit value into CX
                break;

            /* mov dx      ; 1011 */
            case 0x0b:
                /**
                 * 0x0b means the instruction is for moving data to or from the DX register.
                 * Similar logic to the AX, BX, and CX registers.
                 */
                if (higher(vm))
                    vm $dx = (((Reg)a1 << 8) | (vm $dx & 0xFF)); // Move the higher byte of a1 to DX
                else if (lower(vm))
                    vm $dx = ((Reg)a1 | (vm $dx & 0xFF00)); // Move the lower byte of a1 to DX
                else
                    vm $dx = (Reg)a1; // Directly move the 16-bit value into DX
                break;

            /* mov sp      ; 1100 */
            case 0x0c:
                /**
                 * 0x0c means the instruction is for moving data to the SP (Stack Pointer).
                 * This instruction moves the value in the destination to the stack pointer register.
                 */
                vm $sp = (Reg)dst; // Move the value of dst (a1) into the SP register
                break;

            /* mov [addr]      ; 1111 */
            case 0x0f:
                /**
                 * 0x0f indicates moving a value into memory at the address specified.
                 * However, the code to handle memory addressing is not implemented here.
                 * This is typically for an indirect memory access.
                 */
                /* (xx) - Logic for memory addressing should go here */
                break;

            // If the opcode doesn't match any known values, we raise an error.
            default:
                error(vm, ErrInstr); // Invalid instruction, raise error
                break;
        }

        return;
    }


void execinstr(VM* vm, Program *p) {
    /**
     * Function used to execute an instruction
     * @param vm Virtual Machine
     * @param p Program
     * @return void
     */
    Args a1, a2;
    int16 size;

    a1=a2 = 0;
    size = map(*p);

    /**
     * swtich case to map the size of the instruction and the arguments
     * It will extract the arguments from the program and execute the instruction
     */
    switch (size) {
        /**
         * SIze 1 means the instruction has only the opcode with 0 arguments
         * Made for instructions like nop, hlt
         */
        case 1:
            break;

        case 2:
        /**
         * Size 2 means the instruction has the opcode and 1 argument (1 byte long (8 bits))
         * Simple instructions like mov ax, imm8
         * So the argument is located immediately after the opcode at the address p+1
         */
            a1 = *(p+1);
            break;

        case 3:
        /**
         * Size 3 means the instruction has 1 argument, 2 bytes long (16 bits)
         * Two bytes are read, starting at (p+1);
         * - p+1 holds the low byte of the argument
         * - p+2 holds the high byte of the argument
         * Then the two bytes are combined to form the argument into a single 16-bit value using bitwise operations
         */
            a1 = (
                (((int16)*(p+2) & 0xff) << 8)
                    | ((int16)*(p+1) & 0xff)
            );

            break;

        case 5:
        /**
         * Size 5 means the instruction has 2 arguments, each 2 bytes long (16 bits)
         * a1 - The first argument is read from the bytes starting at (p+1) and (p+2), combining them into a single 16-bit value
         * a2 - The second argument is read from the bytes starting at (p+3) and (p+4), combining them into a single 16-bit value
         */
            a1 = (
                (((int16)*(p+2) & 0xff) << 8)
                    | ((int16)*(p+1) & 0xff)
            );

            a2 = (
                (((int16)*(p+4) & 0xff) << 8)
                    | ((int16)*(p+3) & 0xff)
            );

            break;

        default:
        /**
         * If the size of the instruction is not 1, 2, 3 or 5, it will segfault
         */
            segfault(vm);
            break;
    }

    /**
     * Executes the appropriate operation by calling the corresponding handler function
     */
    switch (*p) {
        /**
         * mov and variants
         * These additional opcodes are used to move data between registers and memory
         */
        case mov: 
        case 0x09 ... 0x0f:
            __mov(vm, (Opcode)*p, a1, a2);
            break;

        case nop: // No operation
            break;
        
        case hlt: // Halt the system
            error(vm, SysHlt);
            break;

        // Straight forward instructions
        case ste:    __ste(vm, (Opcode)*p, a1, a2); break;
        case stg:    __stg(vm, (Opcode)*p, a1, a2); break;
        case sth:    __sth(vm, (Opcode)*p, a1, a2); break;
        case stl:    __stl(vm, (Opcode)*p, a1, a2); break;
        case cle:    __cle(vm, (Opcode)*p, a1, a2); break;
        case clg:    __clg(vm, (Opcode)*p, a1, a2); break;
        case clh:    __clh(vm, (Opcode)*p, a1, a2); break;
        case cll:    __cll(vm, (Opcode)*p, a1, a2); break;
    }

    return;
}

void execute(VM *vm) {
    /**
     * Function used to execute a program
     * @param vm Virtual Machine
     * @return void
     */
    int32 brkaddr;
    Program *pp;
    int16 size;
    Instruction ip;

    /**
     * Check if the virtual machine is initialized
     */
    assert(vm && *vm->m);
    size = 0;
    /**
     * brkaddr = memory + break
     * pp = memory
     */
    brkaddr = ((int32)vm->m + vm->b);
    pp = (Program *)&vm->m;

    /* instr1 arg1 instr2 instr3 */
    /* mov ax,0x05; nop; hlt; */
    /* 0x01 0x00 0x05 */
    /* 0x02 */
    /* 0x03 */

    // pp -> 3
    // brk -> 5
    // brkaddr = 0x01 0x00 0x05
    //           0x02 0x03 X

    /**
     * do-while loop to execute the program
     * while the program pointer is not equal to hlt
     * it increase the program pointer by the size of the instruction
     * and if the pp is greater than the brkaddr, it will segfault
     * if not, it will map the size of the instruction and execute it
     */
    do {
        vm $ip += size;
        pp += size;

       if ((int32)pp > brkaddr)
             segfault(vm);
        size = map(*pp);
        execinstr(vm, pp);
    } while (*pp != (Opcode)hlt);

    return;
}

void error(VM* vm, Errorcode e) {
    /**
     * Function used to handle errors
     * @param vm Virtual Machine
     * @param e Errorcode
     * @return void
     */
    int8 exitcode;

    exitcode = -1;
    switch(e) {
        case ErrSegv:
            fprintf(stderr, "%s\n", "VM Segmentation fault");
            break;

        case ErrInstr:
            fprintf(stderr, "%s\n", "VM Illegal instruction");
            break;

        /* (xx) */
        case SysHlt:
            fprintf(stderr, "%s\n", "System halted");
            exitcode = 0;
            printf("ax = %.04hx\n", $i vm $ax);
            if (equal(vm))
                printf("E flag set\n");
            if (gt(vm))
                printf("GT flag set\n");

            break;

        default:
            break;
    }
    if (vm)
        free(vm);

    exit($i exitcode);
}

int8 map(Opcode o) {
    /**
     * Function used to map the size of an instruction
     * @param o Opcode
     * @return int8
     */
    int8 n, ret;
    IM *p;

    ret = 0;
    for (n=IMs, p=instrmap; n; n--, p++)
        if (p->o == o) {
            ret = p->s;
            break;
        }
    
    return ret;
}

VM *virtualmachine() {
    /**
     * This function creates a virtual machine adapted to the program and initializes the memory with only zeros
     * @return VM
     */
    VM *p;
    int16 size;

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p) {
        errno = ErrMem;
        return (VM *)0;
    }
    zero($1 p, size);

     return p;
}

Program *exampleprogram(VM *vm, ...) {
    /**
     * Function used to create a program from a list of instructions
     * @param vm Virtual Machine
     * @param ... List of instructions
     * @return Program
     * @see Program
     */
    Program *p, *instr;
    va_list ap;
    Opcode opc;
    int8 size;

    // Begin at the start of the memory
    va_start(ap, vm);
    p = vm->m;

    do {
        instr = va_arg(ap, Program*);
        opc = (Opcode)*instr;
        size = map(opc);
        copy($1 p, $1 instr, size);
        p += size;
        vm->b += size;
    } while(opc != hlt);
    va_end(ap); // End of the list

    return p;
}

Instruction *i0(Opcode op) {
    /**
     * Function used to create an instruction with no arguments
     * @param op Opcode
     * @return Instruction
     * @see Instruction
     */
    Instruction *i;
    int8 size;

    size = map(op);
    i = (Instruction *)malloc(size);
    assert(i);
    zero($1 i, size);
    i->o = op;

    return i;
}

Instruction *i1(Opcode op, Args a1) {
    /**
     * Function used to create an instruction with one argument
     * @param op Opcode
     * @param a1 Argument 1
     * @return Instruction
     * @see Instruction
     */
    Instruction *i;
    int8 size;

    size = map(op);
    i = (Instruction *)malloc(size);
    assert(i);
    zero($1 i, size);
    i->o = op;
    i->a[0] = a1;

    return i;
}

Instruction *i2(Opcode op, Args a1, Args a2) {
    /**
     * Function used to create an instruction with two arguments
     * @param op Opcode
     * @param a1 Argument 1
     * @param a2 Argument 2
     * @return Instruction
     * @see Instruction
     */
    Instruction *i;
    int8 size;

    size = map(op);
    i = (Instruction *)malloc(size);
    assert(i);
    zero($1 i, size);
    i->o = op;
    i->a[0] = a1;
    i->a[1] = a2;

    return i;
}

Program *i(Instruction *i) {
    /**
     * Function used to create a program from an instruction
     * @param i Instruction
     * @return Program
     * @see Instruction
     */
    Program *p;
    int8 size;

    size = map(i->o);
    p = (Program *)malloc($i size);
    assert(p);
    copy(p, $1 i, 1);
    copy((p+1), $1 i->a, (size-1));

    return p;
}

int main(int argc, char *argv[]) {
    Program *prog;
    VM *vm;

    /**
     * VM creates an instance of a virtual machine able to execute a simple program
     */
    vm = virtualmachine();

    /**
     * Example program :
     * mov ax, 0x04 - mov 0x04 into ax
     * ste - set E flag
     * hlt - halt the system
     */
    prog = exampleprogram(vm,
        i(i1(mov, 0x04)), i(i0(ste)), i(i0(hlt))
    );
    printf("vm   = %p (sz: %ld)\n", vm, sizeof(struct s_vm));

    printf("prog = %p\n", prog);

    execute(vm);

    printhex($1 prog, (map(mov)+map(nop)+map(hlt)), ' ');

    return 0;
}

#pragma GCC diagnostic pop