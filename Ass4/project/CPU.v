`include "InstructionRAM.v"
`include "MainMemory.v"
`include "Units.v"
`include "Stages.v"
`include "Tools.v"

`timescale 100fs/100fs

module CPU;

/* Preparation */
reg ENABLE;
reg IF_ID_RESET, ID_EX_RESET, EX_MEM_RESET;

// CLK
wire CLK;
clock clk(CLK, 1'b1);

// PC Selection
wire [31:0] PC;
wire [31:0] PCF;
wire zeroM;
wire BranchM;
wire [31:0] PCBranchM;
MUX32 PC_MUX(PCF+4, PCBranchM, zeroM & BranchM, PC);

// Read Instruction by RAM
wire [31:0] InstrF;
InstructionRAM InstructionRAM_1(1'b0,ENABLE, PCF>>2, InstrF);

// WB->IF
WB_IF WB_IF_1(PC, CLK, ENABLE, PCF);

// Control Unit
wire RegWriteD, MemtoRegD, MemWriteD, BranchD;
wire [3:0] ALUControlD;
wire ALUSrcD, ALUSrc_saD, RegDstD;
ControlUnit ControlUnit_1(InstrD[31:26], InstrD[5:0], RegWriteD, MemtoRegD, MemWriteD, BranchD, ALUControlD, ALUSrcD, ALUSrc_saD, RegDstD);

// Register File
wire [31:0] RD1D,RD2D;
wire RegWriteW;
wire [4:0] WriteRegW;
wire [31:0] ResultW;
RegisterFile RegisterFile_1(InstrD[25:21], InstrD[20:16], WriteRegW, ResultW, CLK, RegWriteW, RD1D, RD2D);

// Sign Extend
wire [31:0] SignImmD;
SignExtend SignExtend_1(InstrD[15:0], SignImmD);

// IF->ID
wire [31:0] InstrD;
wire [31:0] PCplus4D;
IF_ID IF_ID_1(CLK, IF_ID_RESET, PCF+4, InstrF, PCplus4D,InstrD);

// ID->EX
wire RegWriteE, MemtoRegE, MemWriteE, BranchE, ALUSrcE, ALUSrc_saE, RegDstE;
wire [3:0] ALUControlE;
wire [4:0] RtE, RdE, shamtE;
wire signed [31:0] RD1E,RD2E,PCplus4E,SignImmE;
ID_EX ID_EX_1(CLK, ID_EX_RESET, RegWriteD, MemtoRegD, MemWriteD, BranchD, ALUControlD, ALUSrcD, ALUSrc_saD, RegDstD,
RegWriteE, MemtoRegE, MemWriteE, BranchE, ALUControlE, ALUSrcE, ALUSrc_saE, RegDstE,
RD1D, RD2D, InstrD[20:16], InstrD[15:11], InstrD[10:6], SignImmD, PCplus4D,
RD1E, RD2E, RtE, RdE, shamtE, SignImmE, PCplus4E);

// SrcBE Selection
wire [31:0] SrcBE;
MUX32 SrcBE_MUX(RD2E,(ALUControlE==4'b0100 || ALUControlE==4'b0101) ? {16'h0000,SignImmE[15:0]} : SignImmE, // ori, xori should do unsign extend
ALUSrcE, SrcBE);

// SrcAE Selection
wire [31:0] SrcAE;
MUX32 SrcAE_MUX(RD1E, {27'b0,shamtE}, ALUSrc_saE, SrcAE);

// WriteRegE Selection
wire [4:0] WriteRegE;
MUX5 WriteRegE_MUX(RtE, RdE, RegDstE, WriteRegE);

// ALU
wire [31:0] ALUOutE;
wire zeroE;
ALU ALU_1(SrcAE, SrcBE, ALUControlE, ALUOutE, zeroE);

// EX->MEM
wire RegWriteM, MemtoRegM, MemWriteM;
wire [31:0] ALUOutM, WriteDataM;
wire [4:0] WriteRegM;
EX_MEM EX_MEM_1(CLK, EX_MEM_RESET, RegWriteE, MemtoRegE, MemWriteE, BranchE,
RegWriteM, MemtoRegM, MemWriteM, BranchM,
ALUOutE, zeroE, RD2E, WriteRegE, (SignImmE<<2)+PCplus4E,
ALUOutM, zeroM, WriteDataM, WriteRegM, PCBranchM);

// MainMemory
wire [31:0] ReadDataM;
MainMemory MainMemory_1(CLK, 1'b0, MemWriteM, ALUOutM>>2, {MemWriteM,ALUOutM[31:0]>>2,WriteDataM[31:0]}, ReadDataM);

// MEM->WB
wire MemtoRegW;
wire [31:0] ALUOutW,ReadDataW;
MEM_WB MEM_WB_1( CLK, RegWriteM, MemtoRegM, ALUOutM, ReadDataM, WriteRegM,
RegWriteW, MemtoRegW, ALUOutW, ReadDataW, WriteRegW);

// ResultW Selection
MUX32 ResultW_MUX(ALUOutW, ReadDataW, MemtoRegW, ResultW);


/* Beginning */
integer clock_cycle=0;  // Record the clock cycle

initial begin
    ENABLE = 0;
    IF_ID_RESET = 0;
    ID_EX_RESET = 0;
    EX_MEM_RESET = 0;
    #4000;
    ENABLE = 1;
    clock_cycle = 0;
end

// Branch 
always @(zeroM, BranchM) begin
    if (zeroM & BranchM) // branch
    begin
        #400;
        EX_MEM_RESET = 1;
        ID_EX_RESET = 1;
        IF_ID_RESET = 1;
    end
    else begin
        EX_MEM_RESET=0;
        ID_EX_RESET=0;
        IF_ID_RESET=0;
    end
end

// Jump
always @(ALUControlD) begin
    if (ALUControlD==4'b1100) // j, jal
    begin
        #400;
        ID_EX_RESET=1;
        // RESET_IF_ID=1;
        if (InstrD[31:26]==6'h3) // jal
            RegisterFile_1.data[31]=WB_IF_1.PCF;
        WB_IF_1.PCF={6'b0,InstrD[25:0]}<<2;
    end
    else if (ALUControlD==4'b1101) // jr
    begin
        #400;
        ID_EX_RESET=1;
        // RESET_IF_ID=1;
        #1000
        WB_IF_1.PCF=RD1D;
    end
    else begin
        EX_MEM_RESET = 0;
        ID_EX_RESET = 0;
        IF_ID_RESET = 0;
    end
end

integer i;
integer final_cycle=0, flag=0;
always @(negedge(CLK)) begin
    #900;
    clock_cycle = clock_cycle + 1;
    if (InstrD == 32'b11111111111111111111111111111111)
        flag = 1;
    if (flag==1)
        final_cycle = final_cycle+1;
    if (final_cycle>=5)  // the last instruction is finished
    // display the whole Main Memory
    begin
        i = 0;
        while(i<512)
        begin
            $display("%b", MainMemory_1.DATA_RAM[i]);
            i = i + 1;
        end
        $display("Clock cycles: %d", clock_cycle);
        $finish;
    end
end
endmodule