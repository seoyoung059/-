module imm_gen (input_32, output_64);
	input [31:0] input_32;
	output [63:0] output_64;
	
	parameter LD = 7'b0000011;
	parameter SD = 7'b0100011;
	parameter BEQ = 7'b1100111;
	parameter ADDI = 7'b0010011;
	// fill in the missing code
		
	assign output_64[63:0] = (input_32[6:0]== LD) ? {{52{input_32[31]}} , input_32[31:20]} : 
								(input_32[6:0]== SD) ? {{53{input_32[31]}},input_32[30:25],input_32[11:7]} : 
									(input_32[6:0]== BEQ) ? {{53{input_32[31]}},input_32[7],input_32[30:25],input_32[11:8]} : 
										(input_32[6:0]== ADDI) ? {{52{input_32[31]}}, input_32[31:20]} : {63{input_32[31]}};
	
	// fill in the missing code
endmodule
