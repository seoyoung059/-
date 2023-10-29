module shiftleft_1 (input1, output1);
	input [63:0] input1;
	output [63:0] output1;	
	assign output1 = input1 << 1;
endmodule