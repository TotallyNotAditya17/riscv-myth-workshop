# Day 1 & Day 2: RISC-V ISA, GNU Compiler Toolchain & ABI Verification

This section covers the foundational lab exercises for Day 1 and Day 2 of the **RISC-V MYTH Workshop**, including C program compilation, assembly disassembly, Spike simulation, ABI function calling conventions, and verilated RTL execution with Picorv32.

---

## 📑 Table of Contents
- [Day 1: Introduction to RISC-V ISA & Toolchain](#day-1-introduction-to-risc-v-isa--toolchain)
  - [Lab 1: C Program Compilation & Disassembly](#lab-1-c-program-compilation--disassembly)
  - [Lab 1 Part 2: Spike Simulation & Register Debugging](#lab-1-part-2-spike-simulation--register-debugging)
  - [Lab 2: Integer Representation & Limits](#lab-2-integer-representation--limits)
- [Day 2: Application Binary Interface (ABI) & Verification Flow](#day-2-application-binary-interface-abi--verification-flow)
  - [Lab 3: ABI Function Calls via Assembly](#lab-3-abi-function-calls-via-assembly)
  - [Lab 4: Picorv32 Microprocessor Simulation](#lab-4-picorv32-microprocessor-simulation)

---

# Day 1: Introduction to RISC-V ISA & Toolchain

## Lab 1: C Program Compilation & Disassembly

### C Source Code (`Lab1/sum1ton.c`)
```c
#include <stdio.h>

int main() {
    int i, sum = 0, n = 9;
    for (i = 1; i <= n; i++) {
        sum += i;
    }
    printf("The sum of numbers from 1 to %d is %d\n", n, sum);
    return 0;
}
```

### 1. Native Compilation & Execution
```bash
gcc sum1ton.c -o sum1ton
./sum1ton
```
*Expected Output:* `The sum of numbers from 1 to 9 is 45`

### 2. RISC-V Cross-Compilation & Optimization Flags
```bash
# Compile using O1 optimization
riscv64-unknown-elf-gcc -O1 -mabi=lp64 -march=rv64i -o sum1ton_O1.o sum1ton.c

# Disassemble with objdump
riscv64-unknown-elf-objdump -d sum1ton_O1.o > sum1ton_O1_d.txt

# Compile using Ofast optimization
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o sum1ton_Ofast.o sum1ton.c

# Disassemble Ofast binary
riscv64-unknown-elf-objdump -d sum1ton_Ofast.o > sum1ton_Ofast_d.txt
```

### Disassembly Verification Output
Inspection of the `main` section in the disassembled output confirms instruction reduction between `-O1` and `-Ofast`:

![Objdump Disassembly](../Images/disassemble.png)

---

## Lab 1 Part 2: Spike Simulation & Register Debugging

### Running & Debugging on Spike ISA Simulator
```bash
# Execute using Spike with proxy kernel (pk)
spike pk sum1ton_O1.o

# Launch Spike in interactive debug mode
spike -d pk sum1ton_O1.o
```

### Debugger Session
```text
(spike) until pc 0 10184
(spike) reg 0 a0
(spike) reg 0 a1
(spike) reg 0 a2
(spike) q
```

Inspection of register contents (`a0`, `a1`, `a2`) before and after instruction execution:

![Spike Debugger](../Images/spike_debug.png)

---

## Lab 2: Integer Representation & Limits

### 1. Unsigned 64-bit Integer Boundaries (`Lab2/unshighlow.c`)
```c
#include <stdio.h>
#include <math.h>

int main() {
    unsigned long long int max = (unsigned long long int)(pow(2, 64) - 1);
    unsigned long long int maxover = (unsigned long long int)(pow(2, 127));
    unsigned long long int min = (unsigned long long int)(0);
    unsigned long long int minover = (unsigned long long int)(pow(2, 64) * -1);

    printf("Highest number represented by unsigned long long int is %llu\n", max);
    printf("Overflow test above limit: %llu\n", maxover);
    printf("Lowest number represented by unsigned long long int is %llu\n", min);
    printf("Underflow test below limit: %llu\n", minover);
    return 0;
}
```

```bash
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o unshighlow_Ofast.o unshighlow.c
spike pk unshighlow_Ofast.o
```

### 2. Signed 64-bit Integer Boundaries (`Lab2/signhighlow.c`)
```c
#include <stdio.h>
#include <math.h>

int main() {
    long long int max = (long long int)(pow(2, 63) - 1);
    long long int maxover = (long long int)(pow(2, 127));
    long long int min = (long long int)(pow(2, 63) * -1);
    long long int minover = (long long int)(pow(2, 127) * -1);

    printf("Highest positive number represented by signed long long int is %lld\n", max);
    printf("Overflow test above limit: %lld\n", maxover);
    printf("Lowest negative number represented by signed long long int is %lld\n", min);
    printf("Underflow test below limit: %lld\n", minover);
    return 0;
}
```

```bash
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o signhighlow_Ofast.o signhighlow.c
spike pk signhighlow_Ofast.o
```

---

# Day 2: Application Binary Interface (ABI) & Verification Flow

## ABI Calling Conventions
The Application Binary Interface dictates register usage conventions between application routines and system libraries:

![RISC-V ABI Calling Conventions](../Images/calling_convetion.png)

---

## Lab 3: ABI Function Calls via Assembly

### Main Program (`Lab3/1to9_custom.c`)
```c
#include <stdio.h>

extern int load(int x, int y);

int main() {
    int result = 0;
    int count = 9;
    result = load(0x0, count + 1);
    printf("Sum of numbers from 1 to %d is %d\n", count, result);
    return 0;
}
```

![Main Program](../Images/main_ABI.png)

### Assembly Routine (`Lab3/load.S`)
```asm
.section .text
.global load
.type load, @function

load:
    add   a4, a0, zero   // Initialize sum accumulator a4 with 0
    add   a2, a0, a1     // Load terminal count (10) into a2
    add   a3, a0, zero   // Initialize loop index a3 with 0
loop:
    add   a4, a3, a4     // Incremental sum: a4 = a4 + a3
    addi  a3, a3, 1      // Increment loop counter: a3 = a3 + 1
    blt   a3, a2, loop   // If a3 < a2, branch back to loop
    add   a0, a4, zero   // Move return result into a0
    ret                  // Return to caller
```

![Assembly Load Routine](../Images/load_ABI.png)

### Compilation & Simulation Output
```bash
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o 1to9_custom_Ofast.o 1to9_custom.c load.S
spike pk 1to9_custom_Ofast.o
```

![ABI Output](../Images/compile_ABI.png)

---

## Lab 4: Picorv32 Microprocessor Simulation

In this lab, the compiled C and assembly code is converted to `.hex` format and executed on the synthesizable **Picorv32** Verilog core using Icarus Verilog (`iverilog`).

### Script Execution (`Lab4/rv32im.sh`)
```bash
cd Lab4
chmod 755 rv32im.sh
./rv32im.sh
```

The script performs:
1. Assembly compilation of `start.S` and `load.S`.
2. Compilation of `1to9_custom.c` and `syscalls.c`.
3. Linking using `riscv.ld` into `firmware.elf`.
4. Hex conversion via `hex8tohex32.py`.
5. Verilog simulation of `testbench.v` with `picorv32.v`.
6. Output verification confirming computation of `45`.

---

## 👨‍💻 Author
- **Aditya Nanda** - [GitHub Profile](https://github.com/TotallyNotAditya17)

