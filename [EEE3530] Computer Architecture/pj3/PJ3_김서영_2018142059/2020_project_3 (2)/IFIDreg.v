module IFIDreg(IFinstr, IFPC,
			   IDinstr, IDPC,	
				clk);

	input [31:0] IFinstr;
	input [63:0] IFPC;
	
    output  [31:0] IDinstr; 
    output [63:0] IDPC;
    
    reg  [31:0] IDinstr;
    reg [63:0] IDPC;
   
    input clk;
    
    initial
		begin
            IDinstr <= 0;
            IDPC <= 0;
        end
    
    
always @(posedge clk)		// MEM/WB Pipeline Register
    begin
            IDinstr <= IFinstr;
            IDPC <= IFPC;
    end   


endmodule