`timescale 100fs/100fs

// Used for sign extending immediate values
module SignExtend(n16, n32);
input signed [15:0] n16;
output reg signed [31:0] n32;

initial begin
    n32=0;
end

always @(n16) begin
    n32=n16;
end
endmodule

// Used for selecting the suitable 5-bits value
module MUX5(A0, A1, Control, C);
input [4:0] A0,A1;
input Control;
output reg [4:0] C;

initial begin
    C=0;
end

always @(A0,A1,Control) begin
    if (Control==0)
        C=A0;
    else 
        C=A1;
end
endmodule

// Used for selecting the suitable 32-bits value
module MUX32(A0, A1, Control, C);
input [31:0] A0,A1;
input Control;
output reg [31:0] C;

initial begin
    C=0;
end

always @(A0,A1,Control) begin
    if (Control==0)
        C=A0;
    else 
        C=A1;
end
endmodule

// Used for simulate the CLK signal
module clock(CLK, ENABLE);
input ENABLE;
output reg CLK;

parameter CYCLE = 2000;

initial begin
    CLK=0;
end

always begin
    if(ENABLE==1)
    begin
        #(CYCLE/2);
        CLK=~CLK;
    end
end
endmodule