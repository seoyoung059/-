module IDEXreg(ID_ALUSrc, ID_MemtoReg, ID_RegWrite, ID_MemRead, ID_MemWrite, ID_Branch, ID_ALUOp, 
				ID_Inst, ID_PCvalue, ID_Rs1, ID_Rs2, ID_Rd, ID_res1, ID_res2, ID_Imm, clk,
				EX_Inst, EX_PCvalue, EX_ALUOp, EX_ALUSrc, EX_Branch, EX_MemRead, EX_MemWrite, EX_RegWrite,
				EX_MemtoReg, EX_Rs1, EX_Rs2, EX_Rd, EX_res1, EX_res2, EX_Imm);
				
	input [63:0] ID_PCvalue, ID_res1, ID_res2, ID_Imm;
	input [31:0] ID_Inst;
	input [4:0] ID_Rs1, ID_Rs2, ID_Rd;
	input ID_ALUSrc, ID_MemtoReg, ID_RegWrite,  ID_MemRead, ID_MemWrite, ID_Branch,  clk;
	input [1:0] ID_ALUOp;

	
	output [63:0] EX_PCvalue, EX_res1, EX_res2, EX_Imm;
	output [31:0] EX_Inst;
	output [4:0] EX_Rs1, EX_Rs2, EX_Rd;
	output [1:0] EX_ALUOp;
	output EX_ALUSrc, EX_Branch, EX_MemRead, EX_MemWrite, EX_RegWrite, EX_MemtoReg;
	
	reg [63:0] EX_PCvalue, EX_res1, EX_res2, EX_Imm;
	reg [31:0] EX_Inst;
	reg [4:0] EX_Rs1, EX_Rs2, EX_Rd;
	reg [1:0] EX_ALUOp;
	reg EX_ALUSrc, EX_Branch, EX_MemRead, EX_MemWrite, EX_RegWrite, EX_MemtoReg;

		
	always @(posedge clk)
		begin
				EX_ALUOp<=ID_ALUOp;
				EX_ALUSrc<=ID_ALUSrc;
				EX_Branch<=ID_Branch;
				EX_MemRead<=ID_MemRead;
				EX_MemWrite<=ID_MemWrite;
				EX_RegWrite<=ID_RegWrite;
				EX_MemtoReg<=ID_MemtoReg;
				EX_PCvalue<=ID_PCvalue;
				EX_res1<=ID_res1;
				EX_res2<=ID_res2;
				EX_Imm<=ID_Imm;
				EX_Inst<=ID_Inst;
				EX_Rs1<=ID_Rs1;
				EX_Rs2<=ID_Rs2;
				EX_Rd<=ID_Rd;
		end
		
	initial
		begin
			EX_ALUOp<=2'b00;
			EX_ALUSrc<=0;
			EX_Branch<=0;
			EX_MemRead<=0;
			EX_MemWrite<=0;
			EX_RegWrite<=0;
			EX_MemtoReg<=0;
			EX_PCvalue<=0;
			EX_res1<=0;
			EX_res2<=0;
			EX_Imm<=0;
			EX_Inst<=0;
			EX_Rs1<=0;
			EX_Rs2<=0;
			EX_Rd<=0;
		end
endmodule


