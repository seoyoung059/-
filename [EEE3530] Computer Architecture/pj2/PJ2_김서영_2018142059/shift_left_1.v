module shift_left_1(Imm, s_Imm, clk);

	input [63:0] Imm;
	input clk;
	output [63:0] s_Imm;
	
	reg [63:0] s_Imm;
	reg [64:0] imm_temp;
	
	
	always @(negedge clk) //to prevent error at the rising edge

/*Since there occurs an error when the branch is taken
when the PC value changes at rising edge of each clock,
in the code, the operation occurs when the falling edge
of the clock.*/
	
	begin
		s_Imm<=(Imm<<<1); //shift left operation
	end
	
endmodule
