# Virtual Machine Project

This project implements a basic **Virtual Machine (VM)** that can execute a set of low-level instructions, with a focus on the **`mov`** instruction for data movement between registers and memory. The VM is designed to simulate a simple CPU architecture and provides functionality for various operations commonly found in assembly languages.

## Features

- **Registers**: Simulates 16-bit registers (`AX`, `BX`, `CX`, `DX`, `SP`, `IP`, and `FLAGS`).
- **Instruction Set**: Implements several basic operations, such as `MOV`, `NOP`, and `HLT`.
- **Memory Management**: Handles a simple memory model with read and write operations.
- **Flag Handling**: Uses the status flags (`Zero`, `Carry`, `Greater-than`, `Equal`, etc.) to control logic flow.
- **Program Execution**: A simple program loader and executor that processes bytecode instructions.

## Instruction Set

The instruction set defines several opcodes, including but not limited to:

- `mov`: Move data between registers and memory.
- `nop`: No operation (does nothing).
- `hlt`: Halt the program execution.
- `ste`, `stg`, `stl`, `sth`: Instructions for setting and clearing specific flags or register parts.
- `clg`, `clh`, `cll`, `clg`: Clear certain flags or parts of registers.

### Instruction Format

Each instruction is represented by an opcode, followed by optional arguments. For example:

- `MOV` can take a 16-bit argument and transfer it to a specified register, such as `AX`, `BX`, `CX`, etc.
  
The instruction format can vary based on the operation, such as 1-byte (for simple instructions like `NOP`) to 3-bytes (for operations like `MOV` with arguments).

## Error Handling

The VM uses several error codes to handle exceptional cases:

- `ErrMem`: Memory error.
- `ErrSegv`: Segmentation fault.
- `ErrInstr`: Invalid instruction.
- `SysHlt`: System halt.

Error handling is done using a simple error function that stops execution when an error is encountered.

## Example Program

An example program is provided in the code, which demonstrates the basic usage of the VM. The program loads and executes a set of instructions, such as moving data into registers and performing simple operations.

To use the example program, call the `exampleprogram` function, which sets up a basic program and executes it on the VM.

## How it Works

1. **Instruction Decoding**: The VM reads an instruction from memory, decodes its size (1, 2, or 3 bytes), and extracts the arguments.
2. **Instruction Execution**: Based on the decoded instruction, the VM performs the corresponding operation, such as moving data between registers or halting the execution.
3. **Flag Handling**: The VM checks the status flags (e.g., `Zero`, `Carry`) to determine the result of certain operations and controls the flow of the program.
4. **Memory Management**: The VM uses a simple memory model with read and write operations to store and retrieve data.