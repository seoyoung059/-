module ThreetoOneMUX(a, b, c, out, sel);
	input [63:0] a, b, c;
	input [1:0] sel;
	output [63:0] out;
	
	assign out =  (sel == 2'b00) ? a : 
	( (sel == 2'b01) ? b : 
	( (sel == 2'b10) ? c : 64'b0) ) ;

endmodule