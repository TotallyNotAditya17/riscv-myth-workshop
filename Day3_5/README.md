# Day 3-5 Labs: Digital Logic with TL-Verilog & RISC-V CPU Microarchitecture

This directory contains the Transaction-Level Verilog (TL-Verilog) implementations, Makerchip sandboxes, and verification waveforms developed during Days 3, 4, and 5 of the **RISC-V MYTH Workshop**.

---

## 📑 Table of Contents
- [Day 3: Digital Logic with TL-Verilog](#day-3-digital-logic-with-tl-verilog)
  - [Combinational Calculator](#combinational-calculator)
  - [Sequential Calculator](#sequential-calculator)
  - [2-Cycle Pipelined Calculator](#2-cycle-pipelined-calculator)
  - [Cycle Calculator with Validity](#cycle-calculator-with-validity)
- [Day 4: Basic RISC-V CPU Microarchitecture](#day-4-basic-risc-v-cpu-microarchitecture)
  - [Instruction Fetch (IF)](#instruction-fetch-if)
  - [Instruction Decode (ID)](#instruction-decode-id)
  - [Register File Read & Write](#register-file-read--write)
  - [ALU & Execute Stage](#alu--execute-stage)
  - [Control Logic & Branch Handling](#control-logic--branch-handling)
- [Day 5: Pipelined RISC-V Core & Hazard Resolution](#day-5-pipelined-risc-v-core--hazard-resolution)
  - [Pipelined CPU Datapath](#pipelined-cpu-datapath)
  - [Load / Store Data Memory Interface](#load--store-data-memory-interface)
  - [Final Verification & Test Case](#final-verification--test-case)

---

# Day 3: Digital Logic with TL-Verilog

## Combinational Calculator
A 4-operation ALU calculator supporting addition, subtraction, multiplication, and division based on a 3-bit operation selector:
* File: [`combinational_calculator.tlv`](combinational_calculator.tlv)

![Combinational Calculator](../Images/Combinational_Calculator.png)

---

## Sequential Calculator
A sequential accumulator that stores previous results and performs operations incrementally on incoming operands across clock cycles:
* File: [`sequential_calculator.tlv`](sequential_calculator.tlv)

![Sequential Calculator](../Images/Sequential_Calculator.png)

---

## 2-Cycle Pipelined Calculator
Splitting calculator computation across pipeline stages using TL-Verilog `@1` and `@2` timing abstraction:
* File: [`cycle_calculator.tlv`](cycle_calculator.tlv)

![Cycle Calculator](../Images/Cycle_Calculator.png)

---

## Cycle Calculator with Validity
Implements condition-based transaction validity (`?$valid`) to manage clock gating and clean data propagation:
* File: [`cycle_calculator_validity.tlv`](cycle_calculator_validity.tlv)
* File with Single Value Memory Recall: [`calculator_singleValueMemory.tlv`](calculator_singleValueMemory.tlv)
* Complete Calculator Implementation: [`calculator_solutions.tlv`](calculator_solutions.tlv)

![Cycle Calculator with Validity](../Images/Cycle_Calculator_validity.png)

---

# Day 4: Basic RISC-V CPU Microarchitecture

Implementation of the fundamental 3-stage single-cycle RV32I datapath components:

## Instruction Fetch (IF)
Program Counter logic generating sequential addresses (`PC + 4`) and interfacing with Instruction Memory (IMem):

![Instruction Fetch](../Images/Fetch.png)

---

## Instruction Decode (ID)
Decoding instruction categories (R, I, S, B, U, J) and generating immediate values, destination register indexes, and source operand registers:

![Instruction Decode](../Images/Decode.png)

---

## Register File Read & Write
Dual-read, single-write (2R1W) 32-entry register file with synchronous write enable:

### Register File Read
![Register File Read](../Images/Register_File_Read.png)

### Register File Write
![Register File Write](../Images/Register_File_Write.png)

---

## ALU & Execute Stage
Arithmetic Logic Unit supporting arithmetic operations (ADD, SUB), logical operations (AND, OR, XOR), shifts (SLL, SRL, SRA), and set-less-than comparisons (SLT, SLTU):

![ALU Execution](../Images/ALU.png)

---

## Control Logic & Branch Handling
Branch target calculation and conditional evaluation (`BEQ`, `BNE`, `BLT`, `BGE`, `BLTU`, `BGEU`):

![Control Logic & Branches](../Images/Control_Logic.png)

---

# Day 5: Pipelined RISC-V Core & Hazard Resolution

Implementation of the full **5-Stage Pipelined RV32I Processor** in [`risc-v_solutions.tlv`](risc-v_solutions.tlv):

## Pipelined CPU Datapath
Pipelined into `@1` (Fetch), `@2` (Decode), `@3` (Execute), `@4` (Memory), and `@5` (Write-Back) with 2-source data forwarding logic from the write-back stage to prevent Read-After-Write (RAW) data hazard stalls:

![Pipelining CPU](../Images/Pipelining_CPU.png)

---

## Load / Store Data Memory Interface
Integration of Data Memory (DMem) with load (`LW`) and store (`SW`) instruction decoding and alignment:

![Load and Store](../Images/Load_Store.png)

---

## Final Verification & Test Case
The core executes an assembly testbench program calculating the summation of integers 1 to 9 and stores the result (`45` / `0x2d`) into register `x10`. The verification assert statement passes:

```text
*passed = |cpu/xreg[10]>>5$value == (1+2+3+4+5+6+7+8+9);
```

![Final Simulation Verification](../Images/Final.png)

---

## 👨‍💻 Author
- **Aditya Nanda** - [GitHub Profile](https://github.com/TotallyNotAditya17)

