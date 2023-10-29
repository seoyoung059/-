module PC (NewValue, OldValue, clk);
	input [63:0] NewValue;
	input clk;
	output [63:0] OldValue;
	reg [63:0] OldValue;
	
	always@(negedge clk)
	begin
		OldValue <= NewValue;
	end
	
	initial
	begin
		OldValue <= 64'h000000000a0000000;
	end
	

	
	
endmodule