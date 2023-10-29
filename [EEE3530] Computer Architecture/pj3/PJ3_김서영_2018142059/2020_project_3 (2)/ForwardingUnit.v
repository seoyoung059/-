module ForwardingUnit(ID_EXRS1, IDEXRS2, EX_MEMRd, MEM_WBRd,
						EX_MEMRegWrite, MEM_WBRegWrite,
						ForwardA, ForwardB);
						
	input [4:0] ID_EXRS1, IDEXRS2, EX_MEMRd, MEM_WBRd;
	input EX_MEMRegWrite, MEM_WBRegWrite;
	
	output [1:0] ForwardA, ForwardB;
	
		//decide whether the register declarations are needed
	reg [1:0] ForwardA, ForwardB;
	
	// fill in the missing code

	always @(ID_EXRS1 or IDEXRS2) //or EX_MEMRd or MEM_WBRd or EX_MEMRegWrite or MEM_WBRegWrite)
	begin
	
		if((EX_MEMRegWrite==1) && (EX_MEMRd !=0) && (EX_MEMRd == ID_EXRS1))
		begin
		ForwardA<=2'b10;
		end
		
		else if ((MEM_WBRegWrite==1) && (MEM_WBRd!=0) && ~ ((EX_MEMRegWrite==1) && (EX_MEMRd !=0) && (EX_MEMRd == ID_EXRS1))	&& (MEM_WBRd==ID_EXRS1))
		begin
		ForwardA<=2'b01;
		end
		
		else
		begin
		ForwardA<=2'b00;
		end
		
		
		if ((EX_MEMRegWrite==1) && (EX_MEMRd !=0) && (EX_MEMRd == IDEXRS2))
		begin
		ForwardB<=2'b10;
		end
		
		else if ((MEM_WBRegWrite==1) && (MEM_WBRd!=0) && ~ ((EX_MEMRegWrite==1) && (EX_MEMRd !=0) && (EX_MEMRd == IDEXRS2))	&& (MEM_WBRd==IDEXRS2))
		begin
		ForwardB<=2'b01;
		end
		
		else
		begin
		ForwardB<=2'b00;	
		end
	end

	initial
	begin
	ForwardA=2'b00;
	ForwardB=2'b00;
	end
				

endmodule