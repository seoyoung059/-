module add_4(PC_value,sum_1);
	
	input [63:0] PC_value;
	output [63:0] sum_1;
	
	reg [63:0] sum_1;
	
	always @ (PC_value)
	begin
		sum_1<=(PC_value+64'b100);//plus 4
	end
	
endmodule

