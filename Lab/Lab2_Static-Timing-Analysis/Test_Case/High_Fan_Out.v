module high_fanout(
    // input
    N1,
    N2,
    // output
    N4, 
    N5, 
    N6, 
    N7, 
    N8 , 
    N9, 
    N10, 
    N11, 
    N12, 
    N13,
    N15, 
    N16,
    N17
);
    input N1, N2;
    output N4, N5, N6, N7, N8 , N9, N10, N11, N12, N13, N14, 
    N15, N16, N17;
    wire n3;


    NOR2X1 U8  (.A1(N1), .A2(N2), .ZN(n3));
    INVX1  U10 (.I(n3), .ZN(N4));
    INVX1  U11 (.I(n3), .ZN(N5));
    INVX1  U12 (.I(n3), .ZN(N6));
    INVX1  U13 (.I(n3), .ZN(N7));
    INVX1  U14 (.I(n3), .ZN(N8));
    INVX1  U15 (.I(n3), .ZN(N9));
    INVX1  U16 (.I(n3), .ZN(N10));
    INVX1  U17 (.I(n3), .ZN(N11));
    INVX1  U18 (.I(n3), .ZN(N12));
    INVX1  U19 (.I(n3), .ZN(N13));
    INVX1  U20 (.I(n3), .ZN(N14));
    INVX1  U21 (.I(n3), .ZN(N15));
    INVX1  U22 (.I(n3), .ZN(N16));
    NANDX1 U1  (.A1(n3), .A2(N16), .ZN(N17));

endmodule