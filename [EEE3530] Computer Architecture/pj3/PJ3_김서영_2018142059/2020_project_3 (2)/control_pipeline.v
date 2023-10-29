
module control_pipeline(opcode, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, ALUOp);
	input [6:0] opcode;
	output ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
	output [1:0] ALUOp;
	reg ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
	reg [1:0] ALUOp;

	parameter R_FORMAT = 7'b0110011;
	parameter LD       = 7'b0000011;
	parameter SD       = 7'b0100011;
	parameter BEQ      = 7'b1100111;
	
	initial
		begin
        ALUSrc<=0;MemtoReg<=0;RegWrite<=0;MemRead<=0;MemWrite<=0;Branch<=0;ALUOp<=2'b00;
        end

	always @(opcode)
	begin
		// fill in the misisng code
	case(opcode)
				R_FORMAT:
				begin
					ALUSrc<=0;
					MemtoReg<=0;
					RegWrite<=1;
					MemRead<=0;
					MemWrite<=0;
					Branch<=0;
					ALUOp<=2'b10;
				end
				
				LD:
				begin
					ALUSrc<=1;
					MemtoReg<=1;
					RegWrite<=1;
					MemRead<=1;
					MemWrite<=0;
					Branch<=0;
					ALUOp<=2'b00;
				end
				
				SD:
				begin
					ALUSrc<=1;
					RegWrite<=0;
					MemRead<=0;
					MemWrite<=1;
					Branch<=0;
					MemtoReg=1'bx;
					ALUOp<=2'b00;
	
				end
				
				BEQ:
				begin
					ALUSrc<=0;
					RegWrite<=0;
					MemRead<=0;
					MemWrite<=0;
					Branch<=1;
					MemtoReg=1'bx;
					ALUOp<=2'b01;
				end
				
				default
				begin
					ALUSrc<=0;
					RegWrite<=0;
					MemRead<=0;
					MemWrite<=0;
					Branch<=1'b0;
					ALUOp<=2'b00;
				end
			endcase



		// fill in the misisng code
	end
endmodule

