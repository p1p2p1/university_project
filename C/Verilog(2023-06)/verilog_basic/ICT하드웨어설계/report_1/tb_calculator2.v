`timescale 1ns/1ns

module tb_calculator2();

reg         clk;
reg         rstb;

reg [5:0]   x_data;
reg [5:0]   y_data;

wire [6:0]  w_add;
wire [6:0]  w_sub;
wire [11:0] w_mul;
wire [5:0]  w_div;

always #5 clk=~clk;

initial begin
    clk  =  0;
    rstb =  0;
    x_data=6'd0;
    y_data=6'd0;
#15 rstb =  1;
    x_data=6'd4;
    y_data=6'd2;

#15 x_data=6'd5;
    y_data=6'd6;     

#20 x_data=6'd7;
    y_data=6'd2;

end

calculator2  U0_calculator2(
                        .o_add  (w_add ), 
                        .o_sub  (w_sub ), 
                        .o_mul  (w_mul ), 
                        .o_div  (w_div ), 
                        .clk    (clk   ),
                        .rstb   (rstb  ),
                        .i_data1(x_data), 
                        .i_data2(y_data)
                        );
endmodule
