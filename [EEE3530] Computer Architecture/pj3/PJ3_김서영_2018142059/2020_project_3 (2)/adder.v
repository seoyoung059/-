module adder (op1, op2, result);
	input [63:0] op1, op2;
	output [63:0] result;
	
	assign result = op1 + op2;
	
endmodule