module test;

reg[7:0] in;
reg clk,reset;
wire[7:0] out;

initial begin
	clk=0;
	forever
    #5 clk = !clk;
end

initial begin
    $dumpfile("cpu.vcd");
    $dumpvars;	
end

cpu cc(.in(in), .clk(clk), .reset(reset), .out(out));

initial begin
	in    =8'b00000000;
	reset =1'b1;

#12 in    =8'b01101111;
	reset =1'b0;
     
//write your test pattern----------------

//input mem

//addr 0000:load in 10101011
#10 in =8'b10000000;
#10 in=8'b10101011;

//addr 0001:load in 00000001
#10 in=8'b10000001;
#10 in=8'b00000001;

//addr 0010:load in 10000000
#10 in=8'b10000010;
#10 in=8'b10000000;

//addr 0011:load in 10000000
#10 in=8'b10000011;
#10 in=8'b10000000;

//input A
#10 in=8'b01100001;
#10 in=8'b00000101;

//input B
#10 in=8'b01110001;
#10 in=8'b00001101;

//load C
#10 in=8'b10100010;
#10 in=8'b00000000;

//C=A+B
#10 in=8'b00000000;

//C=A+1
#10 in=8'b00100110;

//A=C
#10 in=8'b11011101;

//Storage
#10 in=8'b10010101;


#10 in=8'b01100000;
#10 in=8'b11011111;

#10 in=8'b11101111;

#10 in=8'b11011111;

#10 in=8'b00001111;

#10 in=8'b10111111;

#10 in=8'b00001111;

#10 in=8'b00110000;

//input B
#10 in=8'b01110001;
#10 in=8'b01001101;

//load C
#10 in=8'b10100001;
#10 in=8'b00000000;

//----------------------------------------
#10  $finish;

end
endmodule