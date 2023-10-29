
module reg_file(clk, RegWrite, RN1, RN2, WN, RD1, RD2, WD);
	input clk;
	input RegWrite;
	input [4:0] RN1, RN2, WN;
	input [63:0] WD;
	output [63:0] RD1, RD2;

	reg [63:0] RD1, RD2;
	reg [63:0] ARRAY[0:63];

	initial
	begin	
		ARRAY[0] <= 64'd0; ARRAY[1] <= 64'd24; ARRAY[2] <= 64'd20; ARRAY[3] <= 64'd14;
		ARRAY[4] <= 64'd8; ARRAY[5] <= 64'd35; ARRAY[6] <= 64'd2; ARRAY[7] <= 64'd0;
		ARRAY[8] <= 64'd0; ARRAY[9] <= 64'd0; ARRAY[10] <= 64'd0; ARRAY[11] <= 64'd0;
		ARRAY[12] <= 64'd0; ARRAY[13] <= 64'd0; ARRAY[14] <= 64'd0; ARRAY[15] <= 64'd0;
		ARRAY[16] <= 64'd0; ARRAY[17] <= 64'd0; ARRAY[18] <= 64'd0; ARRAY[19] <= 64'd0;
		ARRAY[20] <= 64'd0; ARRAY[21] <= 64'd0; ARRAY[22] <= 64'd0; ARRAY[23] <= 64'd0;
		ARRAY[24] <= 64'd0; ARRAY[25] <= 64'd0; ARRAY[26] <= 64'd0; ARRAY[27] <= 64'd0;
		ARRAY[28] <= 64'd0; ARRAY[29] <= 64'd0; ARRAY[30] <= 64'd0; ARRAY[31] <= 64'd0;
	end

	always@(negedge clk)
	begin
		
	if(RegWrite)
		begin
			ARRAY[WN] = WD;
		end
		
		RD1 <= ARRAY[RN1];
		RD2 <= ARRAY[RN2];

	end

endmodule

