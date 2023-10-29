module EXMEMreg(EX_PCadd, EX_Branch, EX_MemRead, EX_MemWrite, EX_RegWrite,EX_MemtoReg, EX_Rd, EX_ALUresult, EX_MemData, EX_zero, clk,
				MEM_PCadd, MEM_zero, MEM_Branch, MEM_MemRead, MEM_MemWrite, MEM_RegWrite,MEM_MemtoReg, MEM_Rd, MEM_ALUresult, MEM_MemData);

	input [63:0] EX_PCadd, EX_ALUresult, EX_MemData;
	input [4:0] EX_Rd;
	input EX_Branch, EX_MemRead, EX_MemWrite, EX_RegWrite, EX_MemtoReg, EX_zero, clk;

	output [63:0] MEM_PCadd, MEM_ALUresult, MEM_MemData;
	output [4:0] MEM_Rd;
	output MEM_Branch, MEM_MemRead, MEM_MemWrite, MEM_RegWrite, MEM_MemtoReg, MEM_zero;
	
	
	reg [63:0] MEM_PCadd, MEM_ALUresult, MEM_MemData;
	reg [4:0] MEM_Rd;
	reg MEM_Branch, MEM_MemRead, MEM_MemWrite, MEM_RegWrite, MEM_MemtoReg, MEM_zero;



	always @(posedge clk)
		begin
			MEM_ALUresult<=EX_ALUresult;
			MEM_MemData<=EX_MemData;
			MEM_Rd<=EX_Rd;
			MEM_Branch<=EX_Branch;
			MEM_MemRead<=EX_MemRead;
			MEM_MemWrite<=EX_MemWrite;
			MEM_RegWrite<=EX_RegWrite;
			MEM_MemtoReg<=EX_MemtoReg;
			MEM_zero<=EX_zero;
			MEM_PCadd<=EX_PCadd;
		end
		
	initial
		begin
			MEM_Branch<=0;
			MEM_MemRead<=0;
			MEM_MemWrite<=0;
			MEM_RegWrite<=0;
			MEM_MemtoReg<=0;
			MEM_zero<=0;
			MEM_ALUresult<=0;
			MEM_MemData<=0;
			MEM_Rd<=0;
			MEM_PCadd<=0;
		end

endmodule

