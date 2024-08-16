module INV (N1, OUT1);
	input N1;
	output OUT1;
	INVX1 U10 (.I(N1), .ZN(OUT1));
endmodule