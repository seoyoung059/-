
module reg_file(clk, RegWrite, RN1, RN2, WN, RD1, RD2, WD);
	input clk;
	input RegWrite;
	input [4:0] RN1, RN2, WN;
	input [63:0] WD;
	output [63:0] RD1, RD2;

	reg [63:0] RD1, RD2;
	reg [63:0] ARRAY[0:63];
	
	// fill in the missing code
	
	always @ (posedge clk)
	begin
		RD1<=ARRAY[RN1];
		RD2<=ARRAY[RN2];
	end
	
	always @ (negedge clk) //to prevent error at the rising edge
	
	/*the error occurred when the operation executed in the program,
	since the data change occurs with the rising edge of the clock.
	Therefore, in the code, when the control signal RegWrite is 1,
	the part writes data from MemtoReg MUX on the register
	corresponding to WN, at the falling edge of the clock, inevitably*/
	
	begin
		if(RegWrite==1'b1)
		begin
			ARRAY[WN]<=WD;
		end
	end
		
	// fill in the missing code
endmodule

