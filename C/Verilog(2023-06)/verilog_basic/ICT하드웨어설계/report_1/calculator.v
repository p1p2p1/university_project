module calculator(o_add, o_sub, o_mul, o_div, i_data1, i_data2);

input   [5:0]     i_data1;
input   [5:0]     i_data2;

output  [6:0]     o_add;
output  [6:0]     o_sub;
output  [11:0]    o_mul;
output  [5:0]     o_div; 

assign  o_add =    i_data1 + i_data2     ; //i_data1과 idata2 더하기 연산 수행
assign  o_sub =    i_data1 - i_data2     ; //i_data1과 idata2 빼기 연산 수행
assign  o_mul =    i_data1 * i_data2     ; //i_data1과 idata2 곱하기 연산 수행
assign  o_div =    i_data1 / i_data2     ; //i_data1과 idata2 나누기 연산 수행

endmodule