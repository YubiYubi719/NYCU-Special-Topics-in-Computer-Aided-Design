module c17 (N1, N2, N3, N6, N7, N22, N23);
	// this is comment 1
	input N1, N2, N3, N6, N7;
/* this is comment 2
		*/output N22, N23;
// hhhh
		// hahaBananice3
/* commentcommentcommentcomment comment comment //  hhh
		*/ //1111
	
	wire n8, n9, n10, n11      , n12;
	NOR2X1 U8 (.A1(n8), .A2(n9), .ZN(N23));NOR2X1 U9 (.A1(N2), .A2(N7), .ZN(n9));
	INVX1 U10 (.I(n10), 
	.ZN(n8)); /* commentcommentcommentcomment comment comment*/   NANDX1 U11 (.A1(n11), . A2(n12), .ZN(N22)
	);
	  NANDX1 U12 (.A1(N2), .A2(n10), .ZN(n12)); // comment /* hh h */
	NANDX1 		U13 (.A1(N6), .A2(N3), .ZN(n10  ));//comme nt
	
	
	
	  NANDX1 U14 (.A1(N1), .A2(N3), .ZN(n11)  );
endmodule