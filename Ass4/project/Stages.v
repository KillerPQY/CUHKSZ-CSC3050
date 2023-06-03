`timescale 100fs/100fs

// Simulate the WB_IF register
module WB_IF(PC, CLK, ENABLE, PCF);
input [31:0] PC;
input CLK, ENABLE;
output reg [31:0] PCF;

initial begin
    PCF=0;
end

always @(posedge CLK) begin
    if(ENABLE==1 && $signed(PC)>=0 )
        PCF = PC;
end
endmodule

// Simulate the IF_ID register
module IF_ID(CLK, RESET, PCplus4_in, Instr_in, PCplus4_out, Instr_out);
input CLK,RESET;
input [31:0] PCplus4_in;
input [31:0] Instr_in;
output reg [31:0] PCplus4_out;
output reg [31:0] Instr_out;

initial begin
    Instr_out <= 32'b0;                                     
    PCplus4_out <= 0;
end

always @(posedge CLK) begin
    if (RESET) begin
        Instr_out <= 32'b00000000000000000000000000000000;
        PCplus4_out <= -1; 
    end
    else begin
        Instr_out <= Instr_in;                                         
        PCplus4_out <= PCplus4_in;                      
    end
end
endmodule

// Simulate the ID_EX register
module ID_EX(CLK, RESET, RegWrite_in, MemtoReg_in, MemWrite_in, Branch_in,
ALUControl_in, ALUSrc_in, ALUSrc_sa_in, RegDst_in,
RegWrite_out, MemtoReg_out, MemWrite_out, Branch_out,
ALUControl_out, ALUSrc_out, ALUSrc_sa_out, RegDst_out,
RD1_in, RD2_in, Rt_in, Rd_in, shamt_in, SignImm_in, PCplus4_in,
RD1_out, RD2_out, Rt_out, Rd_out, shamt_out, SignImm_out, PCplus4_out);
input CLK, RESET;
input RegWrite_in, MemtoReg_in, MemWrite_in, Branch_in, ALUSrc_in, ALUSrc_sa_in, RegDst_in;
input [3:0] ALUControl_in;
input [31:0] RD1_in, RD2_in, SignImm_in, PCplus4_in;
input [4:0] Rt_in, Rd_in, shamt_in;
output reg RegWrite_out, MemtoReg_out, MemWrite_out, Branch_out, ALUSrc_out, ALUSrc_sa_out, RegDst_out;
output reg [3:0] ALUControl_out;
output reg [31:0] RD1_out, RD2_out, SignImm_out, PCplus4_out;
output reg [4:0] Rt_out, Rd_out, shamt_out;

initial begin
    RegWrite_out <= 0;
    MemtoReg_out <= 0;
    MemWrite_out <= 0;
    Branch_out <= 0;
    ALUSrc_out <= 0;
    ALUSrc_sa_out <= 0;
    RegDst_out <= 0;
    ALUControl_out <= 0;
    RD1_out <= 0;
    RD2_out <= 0;
    SignImm_out <= 0;
    PCplus4_out <= 0;
    Rt_out <= 0;
    Rd_out <= 0;
    shamt_out <= 0;
end

always @(posedge CLK) begin
    if(RESET) begin
        RegWrite_out <= 0;
        MemtoReg_out <= 0;
        MemWrite_out <= 0;
        Branch_out <= 0;
        ALUSrc_out <= 0;
        ALUSrc_sa_out <= 0;
        RegDst_out <= 0;
        ALUControl_out <= 0;
        RD1_out <= 0;
        RD2_out <= 0;
        SignImm_out <= 0;
        PCplus4_out <= -1;
        Rt_out <= 0;
        Rd_out <= 0;
        shamt_out <= 0;
    end
    else begin
        RegWrite_out <= RegWrite_in;
        MemtoReg_out <= MemtoReg_in;
        MemWrite_out <= MemWrite_in;
        Branch_out <= Branch_in;
        ALUSrc_out <= ALUSrc_in;
        ALUSrc_sa_out <= ALUSrc_sa_in;
        RegDst_out <= RegDst_in;
        ALUControl_out <= ALUControl_in;
        RD1_out <= RD1_in;
        RD2_out <= RD2_in;
        SignImm_out <= SignImm_in;
        PCplus4_out <= PCplus4_in;
        Rt_out <= Rt_in;
        Rd_out <= Rd_in;
        shamt_out <= shamt_in;
    end
end
endmodule

// Simulate the EX_MEM register
module EX_MEM(CLK, RESET, RegWrite_in, MemtoReg_in, MemWrite_in, Branch_in,
RegWrite_out, MemtoReg_out, MemWrite_out, Branch_out,
C_in, zero_in, WriteData_in, WriteReg_in, PCBranch_in,
C_out, zero_out, WriteData_out, WriteReg_out, PCBranch_out);
input CLK, RESET;
input RegWrite_in, MemtoReg_in, MemWrite_in, Branch_in;
input [31:0] C_in, WriteData_in, PCBranch_in;
input zero_in;
input [4:0] WriteReg_in;
output reg RegWrite_out, MemtoReg_out, MemWrite_out, Branch_out;
output reg [31:0] C_out, WriteData_out, PCBranch_out;
output reg zero_out;
output reg [4:0] WriteReg_out;

initial begin
    RegWrite_out <= 0;
    MemtoReg_out <= 0;
    MemWrite_out <= 0;
    Branch_out <= 0;
    C_out <= 0;
    WriteData_out <= 0;
    PCBranch_out <= 0;
    zero_out <= 0;
    WriteReg_out <= 0;
end

always @(posedge CLK) begin
    if (RESET) begin
        RegWrite_out <= 0;
        MemtoReg_out <= 0;
        MemWrite_out <= 0;
        Branch_out <= 0;
        C_out <= 0;
        WriteData_out <= 0;
        PCBranch_out <= -1;
        zero_out <= 0;
        WriteReg_out <= 0;
    end else begin
        RegWrite_out <= RegWrite_in;
        MemtoReg_out <= MemtoReg_in;
        MemWrite_out <= MemWrite_in;
        Branch_out <= Branch_in;
        C_out <= C_in;
        WriteData_out <= WriteData_in;
        PCBranch_out <= PCBranch_in;
        zero_out <= zero_in;
        WriteReg_out <= WriteReg_in;
    end
end
endmodule

// Simulate the MEM_WB register
module MEM_WB(CLK, RegWrite_in, MemtoReg_in, C_in, ReadData_in, WriteReg_in,
RegWrite_out, MemtoReg_out, C_out, ReadData_out, WriteReg_out);
input CLK;
input RegWrite_in, MemtoReg_in;
input [31:0] C_in, ReadData_in;
input [4:0] WriteReg_in;
output reg RegWrite_out, MemtoReg_out;
output reg [31:0] C_out, ReadData_out;
output reg [4:0] WriteReg_out;

initial begin
    RegWrite_out <= 0;
    MemtoReg_out <= 0;
    C_out <= 0;
    ReadData_out <= 0;
    WriteReg_out <= 0;
end

always @(posedge CLK) begin
    RegWrite_out <= RegWrite_in;
    MemtoReg_out <= MemtoReg_in;
    C_out <= C_in;
    ReadData_out <= ReadData_in;
    WriteReg_out <= WriteReg_in;
end
endmodule