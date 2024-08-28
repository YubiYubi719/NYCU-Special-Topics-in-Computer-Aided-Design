//Verilog HDL for "PLL", "Encoder" "functional"

module Encoder(
	// input
	v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,
	//output
	out0,out1,out2,out3
);

input v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15;
output reg out0,out1,out2,out3;

always@(*)begin
    if(v15) begin
        out3 = 1'b1;
        out2 = 1'b1;
        out1 = 1'b1;
        out0 = 1'b1;
    end
    else if(v14) begin
        out3 = 1'b1;
        out2 = 1'b1;
        out1 = 1'b1;
        out0 = 1'b0;
    end
    else if(v13) begin
        out3 = 1'b1;
        out2 = 1'b1;
        out1 = 1'b0;
        out0 = 1'b1;
    end
    else if(v12) begin
        out3 = 1'b1;
        out2 = 1'b1;
        out1 = 1'b0;
        out0 = 1'b0;
    end
    else if(v11) begin
        out3 = 1'b1;
        out2 = 1'b0;
        out1 = 1'b1;
        out0 = 1'b1;
    end
    else if(v10) begin
        out3 = 1'b1;
        out2 = 1'b0;
        out1 = 1'b1;
        out0 = 1'b0;
    end
    else if(v9) begin
        out3 = 1'b1;
        out2 = 1'b0;
        out1 = 1'b0;
        out0 = 1'b1;
    end
    else if(v8) begin
        out3 = 1'b1;
        out2 = 1'b0;
        out1 = 1'b0;
        out0 = 1'b0;
    end
    else if(v7) begin
        out3 = 1'b0;
        out2 = 1'b1;
        out1 = 1'b1;
        out0 = 1'b1;
    end
    else if(v6) begin
        out3 = 1'b0;
        out2 = 1'b1;
        out1 = 1'b1;
        out0 = 1'b0;
    end
    else if(v5) begin
        out3 = 1'b0;
        out2 = 1'b1;
        out1 = 1'b0;
        out0 = 1'b1;
    end
    else if(v4) begin
        out3 = 1'b0;
        out2 = 1'b1;
        out1 = 1'b0;
        out0 = 1'b0;
    end
    else if(v3) begin
        out3 = 1'b0;
        out2 = 1'b0;
        out1 = 1'b1;
        out0 = 1'b1;
    end
    else if(v2) begin
        out3 = 1'b0;
        out2 = 1'b0;
        out1 = 1'b1;
        out0 = 1'b0;
    end
    else if(v1) begin
        out3 = 1'b0;
        out2 = 1'b0;
        out1 = 1'b0;
        out0 = 1'b1;
    end
	else begin
        out3 = 1'b0;
        out2 = 1'b0;
        out1 = 1'b0;
        out0 = 1'b0;
    end
end

endmodule

