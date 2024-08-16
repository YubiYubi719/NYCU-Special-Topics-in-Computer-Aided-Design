module c17 (N1, N2, N3, N6, N7, N22, N23);
	input N1, N2, N3, N6, N7;/*I hate STA*/
	/*I hate STA*/output N22, N23;// I hate STA;
	wire /*I hate STA*/n8, n9,/*//I hate comment*/ n10, n11,/*//I hate comment*/ n12;
    // I hate STA;
	/*//I hate comment*/NOR2X1 U8 (/*I hate STA*/.A1(n8),/*//I hate comment*/ .A2(n9), .ZN(N23));
	NOR2X1 U9 (.A1(N2), ./*I hate STA*/A2(N7), .ZN(n9));// I hate STA;
	INVX1/*I hate STA*/ U10 (.I(n10/*//I hate comment*/), .ZN(n8)); /*//I hate comment*/
	NANDX1 U11/*I hate STA*/ (.A1(n11), .A2(n12), .ZN(N22));// I hate STA;
	    /*
    I hate STA
    // I hate comment
    
    */NANDX1 U12 (.A1(N2/*//I hate comment*/), .A2/*I hate STA*/(n10), .ZN(n12));
    /*
    I hate STA
    // I hate comment
    */
	/*I hate STA*/NANDX1 U13 /*I hate STA*/(.A1(N6), .A2(N3), .    /*
    I hate STA
    // I hate comment
    
    */ZN(n10));
	NANDX1 U14 (.A1/*//I hate comment*/(N1), .A2(N3),    /*
    I hate STA
    // I hate comment
    
    */ /*//I hate comment*/.ZN(n11));/*I hate STA*/
endmodule