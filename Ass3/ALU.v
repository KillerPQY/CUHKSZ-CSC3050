module alu(i_datain, gr1, gr2, c, flags);
input [31:0] i_datain, gr1, gr2;
output [31:0] c;  // result
//output zero;
//output neg;
//output overlflow;
output [2:0] flags;

reg [5:0] opcode, func;
reg [4:0] shamt;
reg signed[31:0] reg_A, reg_B, reg_C;
reg [31:0] unsigned_reg_A, unsigned_reg_B;
reg [31:0] imm;
reg ovf;  //used to dectect overflow
reg zero, negative, overflow;


always @(i_datain, gr1, gr2)
begin
    reg_A = gr1;
    unsigned_reg_A = gr1;
    opcode = i_datain[31:26];
    func = i_datain[5:0];
    shamt = i_datain[10:6];
    imm = {{16{i_datain[15]}} ,i_datain[15:0]}; // sign extend imm into 32 bits

    // R type instructions
    if (opcode == 6'b00_0000)
    begin
        // add
        if (func == 6'b10_0000)
        begin
            reg_B = gr2;
            {ovf, reg_C} = {reg_A[31], reg_A} + {reg_B[31], reg_B};
            negative = 1'b0;
            zero = 1'b0;
            overflow = ovf ^ reg_C[31];
        end
        // addu
        else if (func == 6'b10_0001)
        begin
            unsigned_reg_B = gr2;
            reg_C = unsigned_reg_A + unsigned_reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // sub
        else if (func == 6'b10_0010)
        begin
            reg_B = gr2;
            {ovf, reg_C} = {reg_A[31], reg_A} - {reg_B[31], reg_B};
            overflow = ovf ^ reg_C[31];
            negative = 1'b0;
            zero = 1'b0;
        end
        // subu
        else if (func == 6'b10_0011)
        begin
            unsigned_reg_B = gr2;
            reg_C = unsigned_reg_A - unsigned_reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // and
        else if (func == 6'b10_0100)
        begin
            reg_B = gr2;
            reg_C = reg_A & reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // nor
        else if (func == 6'b10_0111)
        begin
            reg_B = gr2;
            reg_C = ~(reg_A | reg_B);
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // or
        else if (func == 6'b10_0101)
        begin
            reg_B = gr2;
            reg_C = reg_A | reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // xor
        else if (func == 6'b10_0110)
        begin
            reg_B = gr2;
            reg_C = reg_A ^ reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // slt
        else if (func == 6'b10_1010)
        begin
            reg_B = gr2;
            reg_C = reg_A - reg_B;
            negative = reg_C < 0 ? 1'b1 : 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // sltu
        else if (func == 6'b10_1011)
        begin
            unsigned_reg_B = gr2;
            reg_C = unsigned_reg_A - unsigned_reg_B;
            negative = reg_C < 0 ? 1'b1 : 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // sll
        else if (func == 6'b00_0000)
        begin
            reg_B = {{27{1'b0}}, shamt};
            reg_C = reg_A << reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // sllv
        else if (func == 6'b00_0100)
        begin
            reg_B = gr2;
            reg_C = reg_A << reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // srl
        else if (func == 6'b00_0010)
        begin
            reg_B = {{27{1'b0}}, shamt};
            reg_C = reg_A >> reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // srlv
        else if (func == 6'b00_0110)
        begin
            reg_B = gr2;
            reg_C = reg_A >> reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // sra
        else if (func == 6'b00_0011)
        begin
            reg_B = {{27{1'b0}}, shamt};
            reg_C = reg_A >>> reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
        // srav  
        else if (func == 6'b00_0111)
        begin
            reg_B = gr2;
            reg_C = reg_A >>> reg_B;
            negative = 1'b0;
            zero = 1'b0;
            overflow = 1'b0;
        end
    end

    // beq
    else if (opcode == 6'b00_0100)
    begin
        reg_B = gr2;
        reg_C = reg_A - reg_B;
        zero = (reg_C == 0)? 1'b1: 1'b0;
        reg_C = reg_A - reg_B;
        negative = 1'b0;
        overflow = 1'b0;
    end
    // bne
    else if (opcode == 6'b00_0101)
    begin
        reg_B = gr2;
        reg_C = reg_A - reg_B;
        zero = (reg_C == 0)? 1'b1: 1'b0;
        negative = 1'b0;
        overflow = 1'b0;
    end
    // addi
    else if (opcode == 6'b00_1000)
    begin
        reg_B = imm;
        {ovf, reg_C} = {reg_A[31], reg_A} + {reg_B[31], reg_B};
        overflow = ovf ^ reg_C[31];
        zero = 1'b0;
        negative = 1'b0;
    end
    // addiu
    else if (opcode == 6'b00_1001)
    begin
        unsigned_reg_B = imm;
        reg_C = unsigned_reg_A + unsigned_reg_B;
        zero = 1'b0;
        negative = 1'b0;
        overflow = 1'b0;
    end
    // andi, ori, xori needs zero extend (not sign extend) to calculate
    else if (opcode == 6'b00_1100 || opcode == 6'b00_1101 || opcode == 6'b00_1110)
    begin
        imm = {{16{1'b0}}, i_datain[15:0]};
        zero = 1'b0;
        negative = 1'b0;
        overflow = 1'b0;
        if (opcode == 6'b00_1100) // andi 
        begin
            reg_B = imm;
            reg_C = reg_A & reg_B;
        end
        else if (opcode == 6'b00_1101) // ori 
        begin
            reg_B = imm;
            reg_C = reg_A | reg_B;
        end
        else if (opcode == 6'b00_1110) // xori 
        begin
            reg_B = imm;
            reg_C = reg_A ^ reg_B;
        end
    end
    // slti
    else if (opcode == 6'b00_1010)
    begin
        reg_B = imm;
        reg_C = reg_A - reg_B;
        negative = reg_C < 0 ? 1'b1 : 1'b0;
        zero = 1'b0;
        overflow = 1'b0;
    end
    // sltiu (can use sltu to calculate)
    else if (opcode == 6'b00_1011)
    begin
        unsigned_reg_B = imm;
        reg_C = unsigned_reg_A - unsigned_reg_B;
        negative = reg_C < 0 ? 1'b1 : 1'b0;
        zero = 1'b0;
        overflow = 1'b0;
    end
    // lw
    else if (opcode == 6'b10_0011)
    begin
        reg_B = imm;
        reg_C = reg_A + reg_B;
        zero = 1'b0;
        negative = 1'b0;
        overflow = 1'b0;
    end
    // sw
    else if (opcode == 6'b10_1011)
    begin
        reg_B = imm;
        reg_C = reg_A + reg_B;
        zero = 1'b0;
        negative = 1'b0;
        overflow = 1'b0;
    end
    // Unsupported instructions or instruction error
    else
    begin
        $display("Unsupported instructions or invalid input");
    end
end

assign c = reg_C[31:0];
assign flags = {zero, negative, overflow};

endmodule