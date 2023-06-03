#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <set>
#include <map>
#include "simulator.h"
#include "datasolver.h"
#include <bitset>
using namespace std;

typedef void (*R_instruction) (int*, int*, int*, int);
typedef void (*I_instruction) (int*, int*, int);
typedef void (*J_instruction) (int);

static map<int, R_instruction> RFunctions = {
    {0x20, ADD}, {0x21, ADDU}, {0x24, AND},
    {0x1a, DIV}, {0x1b, DIVU}, {9, JALR}, {8, JR},
    {0x10, MFHI}, {0x12, MFLO}, {0x11, MTHI}, {0x13, MTLO}, 
    {0x18, MULT}, {0x19, MULTU}, {0x27, NOR}, {0x25, OR},
    {0, SLL}, {4, SLLV}, {0x2a, SLT}, {0x2b, SLTU},
    {3, SRA}, {7, SRAV}, {2, SRL}, {6, SRLV},
    {0x22, SUB}, {0x23, SUBU}, {0x26, XOR}
};

static map<int, I_instruction> IFunctions = {
    {8, ADDI}, {9, ADDIU}, {0xc, ANDI},
    {4, BEQ}, {7, BGTZ},  {6, BLEZ}, {5, BNE},
    {0x20, LB}, {0x24, LBU},{0x21, LH},{0x25, LHU},
    {0xf, LUI}, {0x23, LW}, {0xd, ORI}, 
    {0x28, SB}, {0xa, SLTI}, {0xb, SLTIU},
    {0x29, SH}, {0x2b, SW}, {0xe, XORI},
    {0x22, LWL},{0x26, LWR},{0x2a, SWL}, {0x2e, SWR}
};
// op=0x01 rt = 00001, 00000
static map<int, I_instruction> IFUNCTIONS_2 = {
    {1, BGEZ}, {0, BLTZ}
};

static map<int, J_instruction> JFunctions = {
    {2, J}, {3, JAL}
};


// pointers
static int* pc;
static int* pcregister; // PC register
static int* hi;  // HI register
static int* lo;  // LO register
static int* genReg;  // 32 general purpose registers
static int* textStart;  // store the initial address of text
static int* dynamicData;  // point to the address of dynamic data
static int flag = 0;  // used for exit the program
static set<int> checkpoints;


//R funct = 0x20
void ADD(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs + *rt;
}
//R funct = 0x21
void ADDU(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs + *rt;
}
//R funct = 0x24
void AND(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs & *rt;
}
//R funct = 0x1a
void DIV(int* rs, int* rt, int* rd, int shamt) {
    *lo = *rs / *rt;
    *hi = *rs % *rt;
}
//R funct = 0x1b
void DIVU(int* rs, int* rt, int* rd, int shamt) {
    *lo = *rs / *rt;
    *hi = *rs % *rt;
}
//R funct = 0x09
void JALR(int* rs, int* rt, int* rd, int shamt) {
    *rd = (pc - textStart + 1) * 4 + 0x400000;
    pc = textStart + ((*rs) - 0x400000) / 4 - 1;
}
//R funct = 0x08
void JR(int* rs, int* rt, int* rd, int shamt) {
    pc = textStart + ((*rs) - 0x400000) / 4 - 1;  // minus 1
}
//R funct = 0x10
void MFHI(int* rs, int* rt, int* rd, int shamt) {
    *rd = *hi;
}
//R funct = 0x12
void MFLO(int* rs, int* rt, int* rd, int shamt) {
    *rd = *lo;
}
//R funct = 0x11
void MTHI(int* rs, int* rt, int* rd, int shamt) {
    *hi = *rs;
}
//R funct = 0x13
void MTLO(int* rs, int* rt, int* rd, int shamt) {
    *lo = *rs;
}
//R funct = 0x18
void MULT(int* rs, int* rt, int* rd, int shamt) {
    long long RS = (long long)*rs;
    long long RT = (long long)*rt;
    long long result = RS * RT;
    *lo = (int)result;
    result >>= 32;
    *hi = (int)result;
}
//R funct = 0x19
void MULTU(int* rs, int* rt, int* rd, int shamt) {
    long long RS = (long long)*rs;
    long long RT = (long long)*rt;
    long long result = RS * RT;
    *lo = (int)result;
    result >>= 32;
    *hi = (int)result;
}
//R funct = 0x27
void NOR(int* rs, int* rt, int* rd, int shamt) {
    *rd = ~(*rs | *rt);
}
//R funct = 0x25
void OR(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs | *rt;
}
//R funct = 0x00
void SLL(int* rs, int* rt, int* rd, int shamt) {
    *rd = (*rt) << shamt;
}
//R funct = 0x04
void SLLV(int* rs, int* rt, int* rd, int shamt) {
    *rd = (*rt) << (*rs);
}
//R funct = 0x2a
void SLT(int* rs, int* rt, int* rd, int shamt) {
    *rd = (*rs < *rt)?1:0;
}
//R funct = 0x2b
void SLTU(int* rs, int* rt, int* rd, int shamt) {
    unsigned int RS = (unsigned int)*rs;
    unsigned int RT = (unsigned int)*rt;
    *rd = (RS < RT)?1:0;
}
//R funct = 0x03
void SRA(int* rs, int* rt, int* rd, int shamt) {
    *rd = (*rt) >> shamt;
}
//R funct = 0x07
void SRAV(int* rs, int* rt, int* rd, int shamt) {
    *rd = (*rt) >> *rs;
}
//R funct = 0x02
void SRL(int* rs, int* rt, int* rd, int shamt) {
    unsigned RT = (unsigned)*rt;
    *rd = (RT >> shamt);
}
//R funct = 0x06
void SRLV(int* rs, int* rt, int* rd, int shamt) {
    unsigned RT = (unsigned)*rt;
    *rd = (RT >> (*rs));
}
//R funct = 0x22
void SUB(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs - *rt;
}
//R funct = 0x23
void SUBU(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs - *rt;
}
//R funct = 0x26
void XOR(int* rs, int* rt, int* rd, int shamt) {
    *rd = *rs ^ *rt;
}

//I op=8
void ADDI(int* rs, int* rt, int imm) {
    *rt = *rs + imm;
}
//I op=9
void ADDIU(int* rs, int* rt, int imm) {
    *rt = *rs + imm;
}
//I op=0xc
void ANDI(int* rs, int* rt, int imm) {
    *rt = *rs & imm;
}
//I op=4
void BEQ(int* rs, int* rt, int imm) {
    if (*rs == *rt) pc += imm;
}
//I op=7
void BGTZ(int* rs, int* rt, int imm) {
    if (*rs > 0) pc += imm;
}
//I op=6
void BLEZ(int* rs, int* rt, int imm) {
    if (*rs <= 0) pc += imm;
}
//I op=5
void BNE(int* rs, int* rt, int imm) {
    if (*rs != *rt) pc += imm;
}
//I op=0x20
void LB(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *rt = *((signed char*)(textST - 0x400000 + *rs + imm));
}
//I op=0x24
void LBU(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *rt = *((unsigned char*)(textST - 0x400000 + *rs + imm));
}
//I op=0x21
void LH(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *rt = *((short*)(textST - 0x400000 + *rs + imm));
}
//I op=0x25
void LHU(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *rt = *((unsigned short*)(textST - 0x400000 + *rs + imm));
}
//I op=0xf
void LUI(int* rs, int* rt, int imm) {
    int lower = imm & 0xffff;
    lower <<= 16;
    *rt = lower;
}
//I op=0x23
void LW(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *rt = *((int*)(textST - 0x400000 + *rs + imm));
}
// I op=0xd
void ORI(int* rs, int* rt, int imm) {
    *rt = *rs | imm;
}
//I op=0x28
void SB(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *((signed char*)(textST - 0x400000 + *rs + imm)) = *rt;
}
//I op=0xa
void SLTI(int* rs, int* rt, int imm) {
    *rt = (*rs < imm) ? 1 : 0;
}
//I op=0xb
void SLTIU(int* rs, int* rt, int imm) {
    unsigned int RS = (unsigned int)*rs;
    unsigned int IMM = (unsigned int)imm;
    *rt = (RS < IMM) ? 1 : 0;
}
//I op=0x29
void SH(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *((short*)(textST - 0x400000 + *rs + imm)) = *rt;
}
//I op=0x2b
void SW(int* rs, int* rt, int imm) {
    char* textST = (char*)textStart;
    *((int*)(textST - 0x400000 + *rs + imm)) = *rt;
}
//I op=0xe
void XORI(int* rs, int* rt, int imm) {
    *rt = *rs ^ imm;
}
//I op=0x22
void LWL(int* rs, int* rt, int imm) {
    int addr = *rs + imm;
    int n = addr % 4;
    uint8_t* p1 = (uint8_t*)textStart;
    uint8_t* p2 = (uint8_t*)(addr + p1 - 0x400000);
    uint8_t* t = (uint8_t*)rt;
    for (int i = 0; i < 4 - n; i++) {
        *(t + i) = *(p2 + i);
    }
}
//I op=0x26
void LWR(int* rs, int* rt, int imm) {

    int addr = *rs + imm;
    int n = addr % 4;
    uint8_t* p1 = (uint8_t*)textStart;
    uint8_t* p2 = (uint8_t*)(addr + p1 - 0x400000);
    uint8_t* t = (uint8_t*)rt+3;
    for (int i = 0; i < n+1; i++) {
        *(t - i) = *(p2 - i);
    }
}
//I op=0x2a
void SWL(int* rs, int* rt, int imm) {
    int addr = *rs + imm;
    int n = addr % 4;
    uint8_t* p1 = (uint8_t*)textStart;
    uint8_t* p2 = (uint8_t*)(addr + p1 - 0x400000);
    uint8_t* t = (uint8_t*)rt;
    for (int i = 0; i < 4 - n; i++) {
         *(p2 + i) = *(t + i);
    }
}
//I op=0x2e
void SWR(int* rs, int* rt, int imm) {
    int addr = *rs + imm;
    int n = addr % 4;
    uint8_t* p1 = (uint8_t*)textStart;
    uint8_t* p2 = (uint8_t*)(addr + p1 - 0x400000);
    uint8_t* t = (uint8_t*)rt + 3;
    for (int i = 0; i < n + 1; i++) {
        *(p2 - i) = *(t - i);
    }
}
//I op=1 rt=1
void BGEZ(int* rs, int* rt, int imm) {
    if (*rs >= 0) pc += imm;
}
//I op=1 rt=0
void BLTZ(int* rs, int* rt, int imm) {
    if (*rs < 0) pc += imm;
}

//J op=2
void J(int target) {
    target -= 0x400000;
    pc = textStart + (target / 4) - 1;
}
//J op=3
void JAL(int target) {
    target -= 0x400000;
    genReg[31] = (pc - textStart+1) * 4 + 0x400000;
    pc = textStart + (target / 4) - 1;
}


// convert binary string instruction into int
unsigned int binToint(string line) {
    unsigned int res = 0;
    int s = line.size();
    for (int i = s - 1, j = 0; i >= 0; i--, j++) {
        res += (line[i] - '0') * pow(2, j);
    }
    return res;
}


// select instruction from R
void Rins(unsigned int codes) {
    // use mask to find rs, rt, rd. shamt, funct
    int Rs = (codes & 0b00000011111000000000000000000000);
    Rs >>= 21;
    int* rs = (genReg + Rs);

    int Rt = (codes & 0b00000000000111110000000000000000);
    Rt >>= 16;
    int* rt = (genReg + Rt);

    int Rd = (codes & 0b00000000000000001111100000000000);
    Rd >>= 11;
    int* rd = (genReg + Rd);

    unsigned int shamt = (codes & 0b00000000000000000000011111000000);
    shamt >>= 6;
    int funct = (codes & 0b00000000000000000000000000111111);
    RFunctions[funct](rs, rt, rd, shamt);
}
// select instruction from I
void Iins(int op, unsigned int codes, int type) {
    // use mask to find rs, rt, imm
    int Rs = (codes & 0b00000011111000000000000000000000);
    Rs >>= 21;
    int* rs = (genReg + Rs);

    int Rt = (codes & 0b00000000000111110000000000000000);
    Rt >>= 16;
    int* rt = (genReg + Rt);

    int imm = (codes & 0b00000000000000001111111111111111);
    //convert imm into 2's complement form if imm is negative
    if (imm & 0b1000000000000000) {
        imm |= 0b11111111111111110000000000000000;
    }
    
    if (type == 0) IFunctions[op](rs, rt, imm);
    else IFUNCTIONS_2[op](rs, rt, imm);  // BGEZ or BLTZ
}
// select instruction from J
void Jins(int op, unsigned int codes) {
    // use mask to find target
    int target = (codes & 0b00000011111111111111111111111111);
    target <<= 2;
    int curAddr = (pc - textStart)*4 + 0x400000;
    curAddr &= 0b11110000000000000000000000000000;  // first 4 bit
    target = curAddr | target;
    JFunctions[op](target);
}

// syscall
void SYSCALL(ifstream& infile, ofstream& outfile) {


    // create a map for file operation
    static map<int, fstream*> filemap;
    static map<string, int> filename_fd;
    static int filecount = 0;

    // print_int
    if (genReg[2] == 1) {
        int* num = genReg + 4;
        outfile << *num;
    }
    // print_string
    else if (genReg[2] == 4) {
        char* text = (char*)textStart;
        char* str = (char*)((genReg[4] - 0x400000) + text);
        outfile << str;
    }
    // read_int
    else if (genReg[2] == 5) {
        string num;
        getline(infile, num);
        genReg[2] = stoi(num);
    }
    // read_string
    else if (genReg[2] == 8) {
        string tmp;
        getline(infile, tmp);
        tmp = tmp.substr(0, genReg[5]);
        char* text = (char*)textStart;
        char* pchar = (char*)((genReg[4] - 0x400000) + text);
        for (int i = 0; i < genReg[5]; i++) {
            *pchar = tmp[i];
            pchar++;
        }
        *pchar = '\0';
    }
    // sbrk
    else if (genReg[2] == 9) {
        int addr = (dynamicData - textStart) * 4 + 0x400000;
        genReg[2] = addr;
        dynamicData += genReg[4];
    }
    // exit
    else if (genReg[2] == 10) {
        flag = 1;
    }
    // print_char
    else if (genReg[2] == 11) {
        char ch = (char)(genReg[4]);
        outfile << ch;
    }
    // read_char
    else if (genReg[2] == 12) {
        string tmp;
        getline(infile, tmp);
        if (tmp.size() > 0) {
            char ch = tmp[0];
            genReg[2] = ch;
        }
        else
            genReg[2] = '\0';
    }
    // open
    else if (genReg[2] == 13) {
        string filename;
        fstream opfile;
        char* text = (char*)textStart;
        char* addr = (char*)((genReg[4] - 0x400000) + text);
        int fd;
        filename = addr;

        // find file from map according to file descriptor
        if (filename_fd.count(filename)) {
            fd = filename_fd[filename];
        }
        else {
            fd = filecount;
            filename_fd[filename] = filecount;
        }

        // only read mode
        if (genReg[5] == 0) {
            filemap[fd] = new fstream(filename.c_str(), ios::in);
        }
        // only write mode
        else if (genReg[5] == 1) {
            filemap[fd] = new fstream(filename.c_str(), ios::out);
        }
        // read and write
        else {
            filemap[fd] = new fstream(filename.c_str(), ios::out | ios::in);
        }

        // return file descriptor into $a0
        genReg[4] = fd;
        filecount++;
    }
    // read
    else if (genReg[2] == 14) {
        char* text = (char*)textStart;
        char* pchar = (char*)((genReg[5] - 0x400000) + text);
        int count = 0;
        char tmp;
        while (((*filemap[genReg[4]]).get(tmp)) && (count < genReg[6])) {
            *pchar = tmp;
            pchar++;
            count++;
        }
        *pchar = '\0';
        genReg[4] = count;
    }
    // write
    else if (genReg[2] == 15) {
        char* text = (char*)textStart;
        char* pchar = (char*)((genReg[5] - 0x400000) + text);
        string tmp = pchar;
        tmp = tmp.substr(0, genReg[6]);
        (*filemap[genReg[4]]) << tmp;
        genReg[4] = genReg[6];
    }
    // close
    else if (genReg[2] == 16) {
        (*filemap[genReg[4]]).close();
        filemap.erase(genReg[4]);
    }
    // exit2
    else if (genReg[2] == 17) {
        flag = 2;
    }
}

// simulate the execution of the instruction
int* simulator(int* p, ifstream& infile, ofstream& outfile) {
    // use mask to find opcode so that it's easy to classify the instruction
    unsigned int mc = *p;
    int op = mc & 0b11111100000000000000000000000000;
    op >>= 26;
    op &= 0b00000000000000000000000000111111;

    // syscall
    if (mc == 0xc) {
        SYSCALL(infile, outfile);
        return ++pc;
    }
    // R instruction
    if (op == 0) {
        Rins(mc);
        return ++pc;
    }
    // J instruction
    else if (op == 2 || op == 3) {
        Jins(op, mc);
        return ++pc;
    }
    // I instruction
    else {
        if (op == 1) {
            int rt = mc & 0b00000000000111110000000000000000;
            rt >>= 16;
            Iins(rt, mc, 1);
        }
        else {
            Iins(op, mc, 0);
        }
        return ++pc;
    }
}

// store .text part into the simulative memory
void storeText(ifstream& infile, int* p) {
    unsigned int* mc = (unsigned int*)p;
    string line;
    while (getline(infile, line)) {
        *mc = binToint(line);
        mc++;
    }
}

void init_checkpoints(ifstream& checkpoint_file) {
    string num;
    while (getline(checkpoint_file, num)) {
        checkpoints.insert(stoi(num));
    }
}
void checkpoint_memory(int ins_count) {
    if (!checkpoints.count(ins_count))
        return;
    string name = "memory_" + to_string(ins_count) + ".bin"; 
    FILE* fp = fopen(name.c_str(), "wb");
    fwrite(textStart, 1, 0x600000, fp);
    fclose(fp);
}
void checkpoint_register(int ins_count) {
    if (!checkpoints.count(ins_count))
        return;
    string name = "register_" + to_string(ins_count) + ".bin";
    FILE* fp = fopen(name.c_str(), "wb");
    for (int i = 0; i < 32; i++) {
        fwrite(&genReg[i], 4, 1, fp);
    }
    fwrite(pcregister, 4, 1, fp);
    fwrite(hi, 4, 1, fp);
    fwrite(lo, 4, 1, fp);

    fclose(fp);
}

int main(int argc, char* argv[]) {
    ifstream assemble(argv[1]);
    ifstream machine_code(argv[2]);
    ifstream checkFile(argv[3]);
    ifstream inputFile (argv[4]);
    ofstream outputFile (argv[5]);

    init_checkpoints(checkFile);

    unsigned int totalmemory = 6 * 1024 * 1024;
    unsigned int registersize = 32 * 4;
    // initialize the total memory
    char* p;
    
    p = (char*)malloc(totalmemory);
    memset(p, 0, totalmemory);

    // initialize the register memory
    genReg = (int*)malloc(registersize);
    memset(genReg, 0, registersize);
    // $sp
    genReg[29] = 0xa00000;
    // $fp
    genReg[30] = 0xa00000;
    // $gp
    genReg[28] = 0x508000;

    // initialize the HI and LO registers
    int* hilo = (int*)malloc(4 * 2);
    memset(hilo, 0, 4*2);
    hi = hilo;
    lo = hilo + 1;

    // data segment
    int* dp = (int*)(p + 1024 * 1024);
    dynamicData = solver(assemble, dp);

    // text segment
    int* tp = (int*)p;
    storeText(machine_code, tp);

    // PC register
    pcregister = (int*)malloc(4);
    *pcregister = 0x400000;
    pc = (int*)p;
    textStart = (int*)p;
    
    int ins_count = 0;
    while ((*pc) != 0) {
        checkpoint_memory(ins_count);
        checkpoint_register(ins_count);

        if (flag == 1) {
            exit(0);
        }
        else if (flag == 2) {
            exit(genReg[4]);
        }

        pc = simulator(pc, inputFile, outputFile);
        ins_count++;
        
        *pcregister = (pc - textStart)*4 + 0x400000;
    }

    assemble.close();
    machine_code.close();
    checkFile.close();
    inputFile.close();
    outputFile.close();
}