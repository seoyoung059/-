
module alu(ctl, op1, op2, zero, result);
	input [3:0] ctl;
	input [63:0] op1, op2;
	output [63:0] result;
	output zero;

	// symbolic constants for ALU Operations
	parameter ALU_add  = 4'b0010;
	parameter ALU_sub  = 4'b0110;
	parameter ALU_and  = 4'b0000;
	parameter ALU_or   = 4'b0001;

assign result = (ctl == ALU_add)?(op1 + op2):
				(ctl == ALU_sub)?(op1 - op2):
				(ctl == ALU_and)?(op1 & op2):
				(ctl == ALU_or)?(op1 | op2):64'b0;

assign zero = (result == 64'd0)?1'b1:1'b0;	

endmodule

