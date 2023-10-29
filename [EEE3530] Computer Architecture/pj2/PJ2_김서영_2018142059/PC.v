
	module PC (NewValue, OldValue, clk);
	input [63:0] NewValue;
	input clk;
	output [63:0] OldValue;
	reg [63:0] OldValue;
	
	always@(posedge clk)
	begin
		OldValue <= NewValue;
	end
	
	initial
	begin
		OldValue <= 64'ha0000000;
	end
endmodule
