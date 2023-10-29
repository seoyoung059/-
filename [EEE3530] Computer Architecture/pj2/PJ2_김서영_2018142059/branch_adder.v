

module branch_adder(PC_value,branch_offset,sum_2);

	input [63:0] PC_value, branch_offset;
	output [63:0] sum_2;
	
	wire [63:0] branch_offset;
	reg [63:0] temp_sum;
	reg [63:0] sum_2;
	
	
	
	always @(branch_offset) //when branch_offset changes 
	begin
		$unsigned(temp_sum);
		$unsigned(sum_2);
		temp_sum<=$unsigned(PC_value)+$signed(branch_offset);
		sum_2=temp_sum;
	end
	
endmodule