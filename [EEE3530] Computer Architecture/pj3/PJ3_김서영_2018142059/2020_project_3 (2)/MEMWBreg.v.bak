module MEMWBreg(MEM_RegWrite, MEM_MemtoReg, MEM_ALUOut, MEM_memout, MEM_RegRd,
				WB_RegWrite, WB_MemtoReg, WB_memout, WB_ALUOut, WB_RegRd,
				clk);

	input MEM_RegWrite, MEM_MemtoReg;
	input [63:0] MEM_ALUOut, MEM_memout;
	input [4:0] MEM_RegRd;
	
	output WB_RegWrite, WB_MemtoReg;  // WB Control Signals
    output  [63:0] WB_memout, WB_ALUOut;
    output  [4:0] WB_RegRd;
    
	reg WB_RegWrite, WB_MemtoReg;  // WB Control Signals
    reg  [63:0] WB_memout, WB_ALUOut;
    reg  [4:0] WB_RegRd;
   
    wire [63:0] WB_wd;
    
    input clk;
    
    initial
		begin
            WB_RegWrite <= 0;
            WB_MemtoReg <= 0;
            WB_ALUOut   <= 0;
            WB_memout   <= 0;
            WB_RegRd    <= 0;
        end
    
    
always @(posedge clk)		// MEM/WB Pipeline Register
    begin
            WB_RegWrite <= MEM_RegWrite;
            WB_MemtoReg <= MEM_MemtoReg;
            WB_ALUOut   <= MEM_ALUOut;
            WB_memout   <= MEM_memout;
            WB_RegRd    <= MEM_RegRd;
    end   


endmodule
    