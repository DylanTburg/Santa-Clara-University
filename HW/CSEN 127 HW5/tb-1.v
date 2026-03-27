module testbench();

reg [9:0] address;
wire [11:0] data;
reg [10:0] i;
rom dut(address, data);

initial begin
	$display("Start of ROM File Test \n");
	for (i=0; i < 1024; i= i + 1) begin
		address=i; //address too small. cant ever be 1024 so inf loop caused without i
		#1 //delay needed for data to load
		$display("Address = %d --- Data = %d \n", address, data);
	end
	$display("End of the Register File Test");
$finish;

end

endmodule
