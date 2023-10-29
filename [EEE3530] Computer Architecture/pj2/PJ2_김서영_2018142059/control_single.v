module control_single(opcode, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, ALUOp);
	input [6:0] opcode;
	output ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
	output [1:0] ALUOp;
	reg ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
	reg [1:0] ALUOp;

	parameter R_FORMAT = 7'b0110011;
	parameter LD       = 7'b0000011;
	parameter SD       = 7'b0100011;
	parameter BEQ      = 7'b1100011;
	//parameter ADDi     = 7'b0010011;

	always @(opcode)
	begin
		case (opcode)//according to opcode
		// fill in the misisng code
		
		R_FORMAT:
		begin
			ALUSrc<=1'b0;
			MemtoReg<=1'b0;
			RegWrite<=1'b1;
			MemRead<=1'b0;
			MemWrite<=1'b0;
			Branch<=1'b0;
			ALUOp<=2'b10;
		end
		
		LD:
		begin
			ALUSrc<=1'b1;
			MemtoReg<=1'b1;
			RegWrite<=1'b1;
			MemRead<=1'b1;
			MemWrite<=1'b0;
			Branch<=1'b0;
			ALUOp<=2'b00;
		end
		
		SD:
		begin
			ALUSrc<=1'b1;
			MemtoReg<=1'bx;//don't care - uses previous value
			RegWrite<=1'b0;
			MemRead<=1'b0;
			MemWrite<=1'b1;
			Branch<=1'b0;
			ALUOp<=2'b00;
		end
		
		BEQ:
		begin
			ALUSrc<=1'b0;
			MemtoReg<=1'bx;
			RegWrite<=1'b0;
			MemRead<=1'b0;
			MemWrite<=1'b0;
			Branch<=1'b1;
			ALUOp<=2'b01;
		end

		// fill in the missing code  

		endcase
	end
endmodule

