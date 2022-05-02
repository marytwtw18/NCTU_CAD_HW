

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
	reset =1'b0;
	
#12 in    =8'b01101111;
	reset =1'b1;
	
#12	in    =8'b00000000;
	reset =1'b0;
     
//write your test pattern----------------


//input mem
#10 in = 8'b10000000;
#10 in = 8'b00000000;

//inputB
#10 in = 8'b01110000;
#10 in = 8'b00000000;

//inputA
#10 in = 8'b01100000;
#10 in = 8'b00000000;



#10 in = 8'b10100000; //load c

//#20 in = 8'b00100000;  //C=A+1
#20 in = 8'b00111111;  //c=a-1

#10 in = 8'b11000000;  //output mem


#20 in = 8'b11010000;  //A=C
#10 in = 8'b11100000;  //B=C
#10 in = 8'b10010000;  //STORE C
#10 in = 8'b10110000;  //OUTPUT C

#10 in = 8'b10100000;  //LOAD C TO 0

#20 in = 8'b00100000;  //C=A+1
#10 in = 8'b00100000;  //C=A+1

//#20 in = 8'b00111111;  //c=a-1

#10 in = 8'b11010000;  //A=C
#10 in = 8'b11100000;  //B=C

#10 in = 8'b00000000;  //C=A+B

#10 in = 8'b10010000;  //STORE C
#10 in = 8'b10110000;  //OUTPUT C

#10 in = 8'b00000000;  //C=A+B
#10 in = 8'b01010000;  //C=-A
#10 in = 8'b00010000;  //C=A-B
#10 in = 8'b01000000;  //C=A+B+1
#10 in = 8'b00100000;  //C=A+1

#10 in = 8'b10010000;  //STORE C
#10 in = 8'b10110000;  //OUTPUT C

#10 in = 8'b10100000;  //LOAD C TO 0
#20 in = 8'b11110000;  //DEFAULT

//----------------------------------------
#10  $finish;

end
endmodule