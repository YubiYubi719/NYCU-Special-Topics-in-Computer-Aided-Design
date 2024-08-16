module NR2 (IN1, IN2, OUT1);
	input IN1, IN2;
	output OUT1;
	NOR2X1 U1 (.A1(IN1), .A2(IN2), .ZN(OUT1));
endmodule