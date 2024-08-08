#include <bits/stdc++.h>
using namespace std;

pair<string,int> int2Binary(int num){
    if (num == 0) return {"0",0};

    int oneNum = 0;
    string binary;
    while (num > 0) {
        bool isOdd = num & 1;
        if(isOdd){
            binary = "1" + binary;
            ++oneNum;
        }
        else{
            binary = "0" + binary;
        }
        num = num >> 1; // num /= 2;
    }
    return {binary,oneNum};
}

int binary2Int(string binary){
    int result = 0;
    size_t len = binary.length();
    for(size_t i = 0; i < len; ++i){
        result += (binary[(int)len-1-i]-'0') * (1 << i);
    }

    return result;
}

int main(){
    ofstream output("inc/Table.h");
    output << "#ifndef INT2BINARY_h" << '\n';
    output << "#define INT2BINARY_h" << "\n";

    output << "#include <string>" << '\n';
    output << "#include <vector>" << '\n';
    output << "#include <unordered_map>" << '\n';
    output << "using namespace std;" << "\n\n";
    output << "const vector<vector<pair<string,int>>> int2Binary = {" << '\n';
    for(int bitNum = 0; bitNum <= 8; bitNum++){
        int len = 1 << bitNum;
        output << "    {";
        for(int i = 0; i < len; i++){
            pair<string,int> p = int2Binary(i);
            while(p.first.length() < bitNum) p.first = "0"+p.first;
            output << "{\"" << p.first << "\"," << p.second << "}";
            if(i != len-1) output << ",";
        }
        output << "}";
        if(bitNum != 8) output << ",\n";
        else output << "\n";
    }
    output << "};\n\n";
    output << "const unordered_map<string,int> binary2Int = {\n";
    for(int bitNum = 1; bitNum <= 8; bitNum++){
        int len = 1 << bitNum;
        for(int i = 0; i < len; i++){
            output << "    {";
            pair<string,int> binary = int2Binary(i);
            while(binary.first.length() < bitNum) binary.first = "0" + binary.first;
            output << "\"" << binary.first << "\"," << i << "}";
            if(!(bitNum == 8 && i == len-1)) output << ",\n";
            else output << '\n';
        }
    }
    output << "};\n";



    output << "#endif";

    return 0;
}