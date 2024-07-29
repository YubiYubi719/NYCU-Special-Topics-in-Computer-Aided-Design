#include "QM.h"

QuineMcclusky::QuineMcclusky():varNum(0), maxLen(0){
    ;
}

QuineMcclusky::~QuineMcclusky(){
    ;
}

QuineMcclusky::ImpWithLit::ImpWithLit(string bin, int lit): binary(bin), literal(lit){
    ;
}

void QuineMcclusky::readfile(string filename){
    ifstream input(filename);
    string curString;
    // variable num
    input >> curString >> varNum; // ignore .i

    // on-set
    input >> curString; // ignore .m
    input.ignore(2,'\n');
    getline(input,curString);
    stringstream ss(curString);
    int onset_pos;
    while(ss >> onset_pos){
        on_set.push_back(onset_pos);
    }

    // don't-care set
    input >> curString; // ignore .d
    input.ignore(2,'\n');
    ss.clear();
    getline(input,curString);
    ss.str(curString);
    int dc_pos;
    while(ss >> dc_pos){
        dc_set.push_back(dc_pos);
    }

    input.close();
}

string QuineMcclusky::int2Binary(int num){
    if (num == 0) return "0";

    // int originNum = num;
    int oneNum = 0;
    string binary;
    while (num > 0) {
        bool isOdd = num % 2;
        if(isOdd){
            binary = "1" + binary;
            ++oneNum;
        }
        else{
            binary = "0" + binary;
        }
        num /= 2;
    }
    return binary;
}

pair<string,int> QuineMcclusky::int2Binary(int num, int &maxLen){
    if (num == 0) return {"0",0};

    // int originNum = num;
    int curLen = 0;
    int oneNum = 0;
    string binary;
    while (num > 0) {
        bool isOdd = num % 2;
        if(isOdd){
            binary = "1" + binary;
            ++oneNum;
        }
        else{
            binary = "0" + binary;
        }
        ++curLen;
        num /= 2;
    }
    maxLen = (curLen > maxLen)? curLen : maxLen;
    return {binary,oneNum};
}

int QuineMcclusky::binary2Int(string binary){
    int result = 0;
    int len = binary.length();
    for(int i = 0; i < len; ++i){
        result += (binary[len-1-i]-'0') * pow(2,i);
    }

    return result;
}


void QuineMcclusky::buildImplicationTable(){
    vector<int> positions(on_set);
    positions.insert(positions.end(),dc_set.begin(),dc_set.end());
    for(int pos:positions){
        pair<string,int> binary = int2Binary(pos,maxLen);
        if((int)implicationTable.size() < binary.second+1){
            implicationTable.resize(binary.second+1);
        }
        implicationTable[binary.second].emplace_back(binary.first);
    }
    // Set all binaries' lengths to maxLen
    for(list<Implicant> &v:implicationTable){
        for(Implicant &curImp:v){
            while((int)curImp.binary.length() < maxLen) curImp.binary = "0" + curImp.binary;
        }
    }
}

int QuineMcclusky::findDiff(string s1, string s2){
    // if there is only one char different, return the different position
    // otherwise, return -1
    int len = s1.length();
    int diffNum = 0;
    int diffIdx = 0;
    for(int i = 0; i < len; ++i){
        if(s1[i] == s2[i]) continue;
        ++diffNum;
        diffIdx = i;
    }

    return (diffNum == 1)? diffIdx : -1;
}

void QuineMcclusky::removeNonPrimeImplicant(list<Implicant> &curList){
    for(auto iter = curList.begin(); iter != curList.end();){
        if(iter->combinable) iter = curList.erase(iter);
        else ++iter;
    }
}

bool QuineMcclusky::growImplicant(){
    int combinationCount = 0;
    unordered_set<Implicant,Implicant::Hash> biggerImplicants;
    for(int i = 0; i < (int)implicationTable.size()-1; ++i){
        if(implicationTable[i].empty()) continue;
        biggerImplicants.clear();
        for(auto iter1 = implicationTable[i].begin(); iter1 != implicationTable[i].end(); ++iter1){
            for(auto iter2 = implicationTable[i+1].begin(); iter2 != implicationTable[i+1].end(); ++iter2){
                int diffIdx = findDiff(iter1->binary,iter2->binary);
                if(diffIdx != -1){
                    string newImp = iter1->binary;
                    newImp[diffIdx] = '-';
                    biggerImplicants.insert(Implicant(newImp));
                    iter1->combinable = true;
                    iter2->combinable = true;
                    ++combinationCount;
                }
            }
        }
        removeNonPrimeImplicant(implicationTable[i]);
        implicationTable[i].insert(implicationTable[i].end(), biggerImplicants.begin(), biggerImplicants.end());
    }
    removeNonPrimeImplicant(implicationTable[implicationTable.size()-1]);
    return (combinationCount > 0)? true : false;
}

vector<int> QuineMcclusky::implicant2Pos(string implicant){
    // change implicant to corresponding integer
    // ex: 01-- equals to 0100, 0101, 0110, 0111
    // corresponding integer are 4, 5, 6, 7
    vector<int> dc_idx;
    for(int i = 0; i < (int)implicant.length(); ++i){
        if(implicant[i] == '-') dc_idx.push_back(i); 
    }
    int boxSize = pow(2,dc_idx.size());
    vector<string> binaries(boxSize,implicant);
    int curBox = 0;
    for(string &binary:binaries){
        string box = int2Binary(curBox);
        while(box.length() < dc_idx.size()) box = "0" + box;
        int idx = 0;
        for(char &c:binary){
            if(c != '-') continue;
            c = box[idx];
            ++idx;
        }
        ++curBox;
    }
    
    vector<int> result(binaries.size());
    for(int i = 0; i < (int)result.size(); ++i){
        result[i] = binary2Int(binaries[i]);
    }

    return result;
}

void QuineMcclusky::columnCovering(){
    // Get all prime implicants inside implicant table
    for(list<Implicant> curList:implicationTable){
        if(curList.empty()) continue;
        for(Implicant imp:curList){
            primeImplicants.push_back(imp.binary);
        }
    }
    unordered_map<int,vector<string>> mp; // key: on-set position, value: prime implicants
    // initialize mp
    for(int pos:on_set){
        mp.insert({pos,{}});
    }
    // put prime implicants into corresponding on-set
    for(string primeImplicant:primeImplicants){
        vector<int> pos = implicant2Pos(primeImplicant);
        for(int p:pos){
            if(mp.count(p)){
                mp[p].push_back(primeImplicant);
            }
        }
    }
    // Find essential prime implicant
    vector<string> essPrimeImp;
    for(pair<int,vector<string>> p:mp){
        if(p.second.size() == 1) essPrimeImp.push_back(p.second[0]);
    }
    // check which on-set has not been covered by essential prime implicant yet
    list<int> remainOnset(on_set.begin(), on_set.end());
    for(string esspi:essPrimeImp){
        vector<int> coveredPos = implicant2Pos(esspi);
        for(int pos:coveredPos){
            remainOnset.remove(pos);
        }
    }
    // todo: find best fit prime implicant to cover on-set
    
}

bool QuineMcclusky::impCmp(const ImpWithLit &imp1, const ImpWithLit &imp2){
    if(imp1.literal != imp2.literal) return imp1.literal > imp2.literal;

    int len = imp1.binary.length();
    for(int i = 0; i < len; i++){
         if(imp1.binary[i] != imp2.binary[i]){
            if (imp1.binary[i] == '-') return false;
            if (imp2.binary[i] == '-') return true;
            return imp1.binary[i] > imp2.binary[i];
        }
    }
    return imp1.binary > imp2.binary;
}

void QuineMcclusky::printImplicants(){
    cout << ".p " << primeImplicants.size() << '\n';
    for(string imp:primeImplicants){
        int litNum = 0;
        for(char c:imp){
            if(c == '-') continue;
            ++litNum;
        }
        primeImp_with_literal.emplace_back(imp,litNum);
    }
    sort(primeImp_with_literal.begin(), primeImp_with_literal.end(),QuineMcclusky::impCmp);
    int printNum = 0;
    for(ImpWithLit imp:primeImp_with_literal){
        int cnt = 0;
        for(char c:imp.binary){
            if(c == '-'){
                cnt++;
                continue;
            }
            if(c == '1') cout << (char)('A'+cnt);
            else cout << (char)('A'+cnt) << '\'';
            ++cnt;
        }
        cout << '\n';
        ++printNum;
        if(printNum == 15) break;
    }
}