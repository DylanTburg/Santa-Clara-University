`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Create Date: 03/23/2026 07:30:21 PM
// Module Name: Extender
//////////////////////////////////////////////////////////////////////////////////


module Extender(
    input clk,
    input sign,
    input size,
    input [15:0] data_i,
    output reg [31:0] data_o
    );

    wire extend = sign ? (size ? data_i[15] : data_i[7]) : 1'b0;

    always @ (posedge clk)
        data_o = size ? {{16{extend}}, data_i} : {{24{extend}}, data_i[7:0]};


endmodule
