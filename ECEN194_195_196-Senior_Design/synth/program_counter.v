`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Create Date: 10/26/2025 04:00:06 PM
// Module Name: program_counter
//////////////////////////////////////////////////////////////////////////////////


module program_counter(
    input clk, reset,
    input [31:0] PC_in,
    output reg [31:0] PC_out
    );

    always @ (negedge clk) begin
        if (!reset) PC_out <= 0;
        else begin
            PC_out <= PC_in;
        end
    end

endmodule
