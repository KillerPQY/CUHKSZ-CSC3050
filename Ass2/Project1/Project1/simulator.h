#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <fstream>
#include <string>

// syscall
void SYSCALL(std::ifstream& infile, std::ofstream& outfile);

// R type instructions
void ADD(int* rs, int* rt, int* rd, int shamt);
void ADDU(int* rs, int* rt, int* rd, int shamt);
void AND(int* rs, int* rt, int* rd, int shamt);
void DIV(int* rs, int* rt, int* rd, int shamt);
void DIVU(int* rs, int* rt, int* rd, int shamt);
void JALR(int* rs, int* rt, int* rd, int shamt);
void JR(int* rs, int* rt, int* rd, int shamt);
void MFHI(int* rs, int* rt, int* rd, int shamt);
void MFLO(int* rs, int* rt, int* rd, int shamt);
void MTHI(int* rs, int* rt, int* rd, int shamt);
void MTLO(int* rs, int* rt, int* rd, int shamt);
void MULT(int* rs, int* rt, int* rd, int shamt);
void MULTU(int* rs, int* rt, int* rd, int shamt);
void NOR(int* rs, int* rt, int* rd, int shamt);
void OR(int* rs, int* rt, int* rd, int shamt);
void SLL(int* rs, int* rt, int* rd, int shamt);
void SLLV(int* rs, int* rt, int* rd, int shamt);
void SLT(int* rs, int* rt, int* rd, int shamt);
void SLTU(int* rs, int* rt, int* rd, int shamt);
void SRA(int* rs, int* rt, int* rd, int shamt);
void SRAV(int* rs, int* rt, int* rd, int shamt);
void SRL(int* rs, int* rt, int* rd, int shamt);
void SRLV(int* rs, int* rt, int* rd, int shamt);
void SUB(int* rs, int* rt, int* rd, int shamt);
void SUBU(int* rs, int* rt, int* rd, int shamt);
void XOR(int* rs, int* rt, int* rd, int shamt);

// I type instructions
void ADDI(int* rs, int* rt, int imm);
void ADDIU(int* rs, int* rt, int imm);
void ANDI(int* rs, int* rt, int imm);
void BEQ(int* rs, int* rt, int imm);
void BGEZ(int* rs, int* rt, int imm); 
void BGTZ(int* rs, int* rt, int imm);
void BLEZ(int* rs, int* rt, int imm);
void BLTZ(int* rs, int* rt, int imm);
void BNE(int* rs, int* rt, int imm);
void LB(int* rs, int* rt, int imm);
void LBU(int* rs, int* rt, int imm);
void LH(int* rs, int* rt, int imm);
void LHU(int* rs, int* rt, int imm);
void LUI(int* rs, int* rt, int imm);
void LW(int* rs, int* rt, int imm);
void ORI(int* rs, int* rt, int imm);
void SB(int* rs, int* rt, int imm);
void SLTI(int* rs, int* rt, int imm);
void SLTIU(int* rs, int* rt, int imm);
void SH(int* rs, int* rt, int imm);
void SW(int* rs, int* rt, int imm);
void XORI(int* rs, int* rt, int imm);
void LWL(int* rs, int* rt, int imm);
void LWR(int* rs, int* rt, int imm);
void SWL(int* rs, int* rt, int imm);
void SWR(int* rs, int* rt, int imm);

// J type instructions
void J(int target);
void JAL(int target);

// convert binary string instruction into int
unsigned int binToint(std::string line);

// select instruction from R
void Rins(unsigned int codes);
// select instruction from I
void Iins(int op, unsigned int codes, int type);
// select instruction from J
void Jins(int op, unsigned int codes);

// simulate the execution of the instruction
int* simulator(int* p, std::ifstream& infile, std::ofstream& outfile);

// store .text part into the simulative memory
void storeText(std::ifstream& infile, int* p);

#endif