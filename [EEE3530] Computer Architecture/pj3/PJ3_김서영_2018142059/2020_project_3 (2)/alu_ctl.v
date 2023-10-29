
module alu_ctl(ALUOp, Funct7, Funct3, ALUOperation);
	input [1:0] ALUOp;
	input Funct7;
	input [2:0] Funct3;
	output [3:0] ALUOperation;
	

	// symbolic constants for instruction function code
	parameter F7_add  = 1'b0;
	parameter F7_sub  = 1'b1;
	parameter F7_and  = 1'b0;
	parameter F7_or   = 1'b0;
	
	parameter F3_add  = 3'b000;
	parameter F3_sub  = 3'b000;
	parameter F3_and  = 3'b111;
	parameter F3_or   = 3'b110;

	// symbolic constants for ALU Operations
	parameter ALU_add  = 4'b0010;
	parameter ALU_sub  = 4'b0110;
	parameter ALU_and  = 4'b0000;
	parameter ALU_or   = 4'b0001;
	
	
 assign ALUOperation = (ALUOp == 2'b00)?ALU_add:
					   (ALUOp == 2'b01)?ALU_sub:
					   (ALUOp == 2'b10)?
							( (Funct7==F7_add && Funct3==F3_add)?ALU_add:
							(Funct7==F7_sub && Funct3==F3_sub)?ALU_sub:
							(Funct7==F7_and && Funct3==F3_and)?ALU_and:
							(Funct7==F7_or && Funct3==F3_or)?ALU_or:0 )	: 4'b0;
																								
endmodule

