module rom(
	input [9:0] address,
	output reg [11:0] data  
);
	reg [11:0] mem [0:1023];
	initial $readmemh("data.mem", mem);
	always@(*) data = mem[address];
endmodule
		
