module and_(zero,branch,and_result,clk);

	input zero, branch;
	input clk;
	output and_result;
	
	reg and_result;
	
	
	always @(negedge clk) //to prevent error at the rising edge
	
	begin
		and_result<=(zero && branch); //'and' logic operation
	end
	

	
endmodule
