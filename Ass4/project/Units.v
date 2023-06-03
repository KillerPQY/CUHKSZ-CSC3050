`timescale 100fs/100fs

// Control Unit
module ControlUnit(Op, Funct, RegWrite, MemtoReg, MemWrite, Branch, ALUControl, ALUSrc, ALUSrc_sa, RegDst);

input [5:0] Op, Funct;  // control unit input
output reg RegWrite, MemtoReg, MemWrite, Branch;  // control unit output
output reg [3:0] ALUControl;  // control unit output
output reg ALUSrc, RegDst;  // control unit output
output reg ALUSrc_sa;  // select ALU input source wheter it is from shamt or not

initial begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0;
end

always @(Op, Funct) begin
    if (Op==6'h23) // lw
    begin
        RegWrite=1;
        MemtoReg=1;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h2b) // sw
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=1;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h0 && Funct==6'h20) // add
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h21) // addu
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h8) // addi
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h9) // addiu
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0000;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h0 && Funct==6'h22) // sub
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0001;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h23) // subu
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0001;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h24) // and
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0010;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'hc) // andi
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0010;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h0 && Funct==6'h27) // nor
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0011;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h25) // or
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0100;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'hd) // ori
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0100;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h0 && Funct==6'h26) // xor
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0101;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'he) // xori
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0101;
        ALUSrc=1;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h0 && Funct==6'h0) // sll
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0110;
        ALUSrc=0;
        ALUSrc_sa=1;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h4) // sllv
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0110;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h2) // srl
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0111;
        ALUSrc=0;
        ALUSrc_sa=1;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h6) // srlv
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b0111;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h3) // sra
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1000;
        ALUSrc=0;
        ALUSrc_sa=1;
        RegDst=1;
    end
    else if (Op==6'h0 && Funct==6'h7) // srav
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1000;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h4) // beq
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=1;
        ALUControl=4'b1001;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0; // x
    end
    else if (Op==6'h5) // bne
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=1;
        ALUControl=4'b1010;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0; // x
    end
    else if (Op==6'h0 && Funct==6'h2a) // slt
    begin
        RegWrite=1;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1011;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=1;
    end
    else if (Op==6'h2) // j
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1100;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h3) // jal
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1100;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else if (Op==6'h0 && Funct==6'h8) // jr
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1101;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0;
    end
    else  // invalid
    begin
        RegWrite=0;
        MemtoReg=0;
        MemWrite=0;
        Branch=0;
        ALUControl=4'b1011;
        ALUSrc=0;
        ALUSrc_sa=0;
        RegDst=0; // x
    end
    
end
endmodule

// RegisterFile unit
module RegisterFile(A1, A2, A3, WD, CLK, WE, RD1, RD2);
input [4:0] A1,A2,A3;
input [31:0] WD;
input CLK;
input WE;
output reg [31:0] RD1,RD2;

// Simulate the registers to store data
reg signed [31:0] data[31:0];

integer counter;

initial begin
    RD1 = 0;
    RD2 = 0;
    for(counter=0;counter<=31;counter=counter+1) begin
        data[counter]=0;
    end
end

always @(negedge CLK) begin // read in the second half
    RD1 = data[(A1)];
    RD2 = data[(A2)];
end


always @(posedge CLK) begin // write in the first half
    #10;
    if (WE==1)
        data[(A3)]=WD;
end
endmodule

// ALU unit
module ALU(A, B, ALUControl, C, zero);

input signed [31:0] A, B;  // ALU input
input [3:0] ALUControl;  // ALU control
output reg signed [31:0] C;  // ALU output
output reg zero;  // used for branch instructions 

reg signed [31:0] signedA, signedB;
reg unsigned [31:0] unsignedA, unsignedB;

initial begin
    signedA = 0;
    signedB = 0;
    unsignedA = 0;
    unsignedB = 0;
    C = 0;
    zero = 0;
end

always @(A, B, ALUControl) begin
    signedA = A;
    signedB = B;
    unsignedA = A;
    unsignedB = B;
    zero=0;
    case (ALUControl)
        4'b0000: // lw, sw, add, addu, addi, addiu
        begin
            C = signedA + signedB;
        end
        4'b0001: // sub, subu
        begin
            C = signedA - signedB;
        end
        4'b0010: // and, andi
        begin
            C = signedA & signedB;
        end
        4'b0011: // nor
        begin
            C = ~(signedA | signedB);
        end
        4'b0100: // or, ori
        begin
            C = signedA | signedB;
        end
        4'b0101: // xor, xori
        begin
            C = signedA ^ signedB;
        end
        4'b0110: // sll, sllv
        begin
            C = unsignedB << signedA;
        end
        4'b0111: // srl, srlv
        begin
            C = unsignedB >> signedA;
        end
        4'b1000: // sra, srav
        begin
            C= signedB >>> signedA;  
        end
        4'b1001: // beq
        begin
            C = A == B ? 32'h0000_0001 : 32'h0000_0000;
            zero = A == B ? 1 : 0;
        end
        4'b1010: // bne
        begin
            C = A != B ? 32'h0000_0001 : 32'h0000_0000;
            zero = A != B ? 1 : 0;
        end
        4'b1011: // slt
        begin
            C = signedA < signedB ? 32'h0000_0001 : 32'h0000_0000;
        end
        // j, jr, jal don't need to enter ALU
        // default: $display("Unsupported ALUControl\n");
    endcase
end
endmodule