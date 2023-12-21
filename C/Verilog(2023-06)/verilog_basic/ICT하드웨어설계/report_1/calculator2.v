module calculator2(o_add, o_sub, o_mul, o_div, clk, rstb, i_data1, i_data2);
input                 clk    ;
input                 rstb   ;

input       [5:0]     i_data1;
input       [5:0]     i_data2;

output  reg [6:0]     o_add;
output  reg [6:0]     o_sub;
output  reg [11:0]    o_mul;
output  reg [5:0]     o_div; 

always @(posedge clk or negedge rstb)
begin
  if(!rstb)
  begin
    o_add <=    0;
    o_sub <=    0;
    o_mul <=    0;
    o_div <=    0;
  end
  else
  begin
    o_add <=   i_data1 + i_data2 ;   //i_data1과 idata2 더하기 연산 수행
    o_sub <=   i_data1 - i_data2;   //i_data1과 idata2 빼기 연산 수행
    o_mul <=   i_data1 * i_data2;   //i_data1과 idata2 곱하기 연산 수행
    o_div <=    i_data1 / i_data2;   //i_data1과 idata2 나누기 연산 수행
  end
end

endmodule