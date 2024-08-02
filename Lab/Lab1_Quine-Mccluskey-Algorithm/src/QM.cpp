#include "QM.h"

QuineMccluskey::QuineMccluskey():varNum(0){
    ;
}

QuineMccluskey::~QuineMccluskey(){
    ;
}

QuineMccluskey::ImpWithLit::ImpWithLit(string bin, int lit): binary(bin), literal(lit){
    ;
}

void QuineMccluskey::readfile(string filename){
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

pair<string,int> QuineMccluskey::int2Binary(int num){
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

int QuineMccluskey::binary2Int(string binary){
    int result = 0;
    size_t len = binary.length();
    for(size_t i = 0; i < len; ++i){
        result += (binary[(int)len-1-i]-'0') * (1 << i);
    }

    return result;
}


void QuineMccluskey::buildImplicationTable(){
    vector<int> positions(on_set);
    positions.insert(positions.end(),dc_set.begin(),dc_set.end());
    for(int pos:positions){
        pair<string,int> binary = int2Binary(pos);
        if((int)implicationTable.size() < binary.second+1){
            implicationTable.resize(binary.second+1);
        }
        // Set all binaries' lengths to varNum
        while((int)binary.first.length() < varNum) binary.first = "0" + binary.first;
        implicationTable[binary.second].emplace_back(binary.first);
    }
}

int QuineMccluskey::findDiff(const string &s1, const string &s2){
    // if there is only one char different, return the different position
    // otherwise, return -1
    int diffNum = 0;
    int diffIdx = 0;
    for(size_t i = 0; i < s1.length(); ++i){
        if(s1[i] == s2[i]) continue;
        ++diffNum;
        if(diffNum > 1) return -1;
        diffIdx = i;
    }

    return diffIdx;
}

void QuineMccluskey::removeNonPrimeImplicant(list<Implicant> &curList){
    for(auto iter = curList.begin(); iter != curList.end();){
        if(iter->combinable) iter = curList.erase(iter);
        else ++iter;
    }
}

bool QuineMccluskey::growImplicant(){
    int combinationCount = 0;
    unordered_set<Implicant,Implicant::Hash> biggerImplicants;
    for(size_t i = 0; i < implicationTable.size()-1; ++i){
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

vector<int> QuineMccluskey::implicant2Pos(string implicant){
    // change implicant to corresponding integer
    // ex: 01-- equals to 0100, 0101, 0110, 0111
    // corresponding integer are 4, 5, 6, 7
    vector<int> dc_idx;
    for(size_t i = 0; i < implicant.length(); ++i){
        if(implicant[i] == '-') dc_idx.push_back((int)i); 
    }
    int boxSize = 1 << dc_idx.size(); // pow(2,dc_idx.size());
    vector<string> binaries(boxSize,implicant);
    int curBox = 0;
    for(string &binary:binaries){
        string box = int2Binary(curBox).first;
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
    for(size_t i = 0; i < result.size(); ++i){
        result[i] = binary2Int(binaries[i]);
    }

    return result;
}

int QuineMccluskey::calLiteral(const string &imp){
    int lit = 0;
    for(char c:imp){
        if(c != '-') ++lit; 
    }
    return lit;
}

vector<string> QuineMccluskey::coverRemainingOnset(vector<int> remainOnset){
    // initialize coverage table and literal count of each nonEssPrimeImp
    vector<int> implicantCoverage;
    vector<int> literalsCount;
    for(string imp:nonEssPrimeImp){
        vector<int> pos = implicant2Pos(imp);
        int val = 0;
        for(const int &p:pos){
            vector<int>::iterator iter = find(remainOnset.begin(), remainOnset.end(), p);
            if(iter != remainOnset.end()) val += 1 << ((remainOnset.size()-1) - (iter-remainOnset.begin()));
        }
        implicantCoverage.push_back(val);
        literalsCount.push_back(calLiteral(imp));
    }

    // Using Dynamic Programming approach
    int maxState = 1 << remainOnset.size();
    vector<int> dp(maxState, INT_MAX); // idx: covered onset, ex: if covered 0,1,3 onset, its idx is 3'b1011 = 11
                                       // value: minimum implicant number that can cover current onsets
    vector<int> parent(maxState, -1);
    vector<int> choice(maxState, -1);
    vector<int> literalsDp(maxState, INT_MAX);
    dp[0] = 0;
    literalsDp[0] = 0;

    for (int i = 0; i < maxState; ++i) {
        if (dp[i] == INT_MAX) continue; // can not cover the i_th onset
        for (size_t j = 0; j < nonEssPrimeImp.size(); ++j) {
            int nextCover = i | implicantCoverage[j];
            if (dp[i] + 1 < dp[nextCover] || (dp[i] + 1 == dp[nextCover] && (literalsDp[i] + literalsCount[j] < literalsDp[nextCover]))) {
                dp[nextCover] = dp[i] + 1;
                literalsDp[nextCover] = literalsDp[i] + literalsCount[j];
                parent[nextCover] = i;
                choice[nextCover] = j;
            }
        }
    }

    // top-down to trace the choosed prime implicants
    vector<string> implicants;
    int curState = maxState - 1;
    // cout << dp[curState] << '\n';
    while (parent[curState] != -1) {
        implicants.push_back(nonEssPrimeImp[choice[curState]]);
        curState = parent[curState];
    }

    return implicants;
}


void QuineMccluskey::columnCovering(){ // final answer stores in list<string> essPrimeImp;
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
        for(const int &p:pos){
            if(mp.count(p)){
                mp[p].push_back(primeImplicant);
            }
        }
    }

    // Find essential prime implicant 
    unordered_set<string> tmpEssImp;
    unordered_set<string> tmpnonEssImp;
    for(pair<int,vector<string>> p:mp){
        if(p.second.size() == 1){
            tmpEssImp.insert(p.second[0]);
            continue;
        }
        for(string imp:p.second){
            tmpnonEssImp.insert(imp);
        }
    }
    essPrimeImp.insert(essPrimeImp.end(),tmpEssImp.begin(),tmpEssImp.end());
    nonEssPrimeImp.insert(nonEssPrimeImp.end(),tmpnonEssImp.begin(),tmpnonEssImp.end());

    // check which on-set has not been covered by essential prime implicant yet
    unordered_set<int> remainOnset(on_set.begin(), on_set.end());
    for(string esspi:essPrimeImp){
        vector<int> coveredPos = implicant2Pos(esspi);
        for(int pos:coveredPos){
            remainOnset.erase(pos);
        }
    }

    vector<string> imps = coverRemainingOnset(vector<int> (remainOnset.begin(),remainOnset.end()));
    if(!imps.empty()) essPrimeImp.insert(essPrimeImp.end(),imps.begin(),imps.end());
}

bool QuineMccluskey::ImpWithLit::impCmp(const ImpWithLit &imp1, const ImpWithLit &imp2){
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

void QuineMccluskey::printImplicants(ofstream &output){
    output << ".p " << primeImplicants.size() << '\n';

    vector<ImpWithLit> primeImp_with_literal;
    for(string imp:primeImplicants){
        primeImp_with_literal.emplace_back(imp,calLiteral(imp));
    }
    sort(primeImp_with_literal.begin(), primeImp_with_literal.end(),QuineMccluskey::ImpWithLit::impCmp);
    int printNum = 0;
    for(ImpWithLit imp:primeImp_with_literal){
        int cnt = 0;
        for(char c:imp.binary){
            if(c == '-'){
                cnt++;
                continue;
            }
            if(c == '1') output << (char)('A'+cnt);
            else output << (char)('A'+cnt) << '\'';
            ++cnt;
        }
        output << '\n';
        ++printNum;
        if(printNum == 15) break;
    }
    output << '\n';
}

void QuineMccluskey::printMinimumCovering(ofstream &output){
    output << ".mc " << essPrimeImp.size() << '\n';
    
    vector<ImpWithLit> imp_with_literal;
    for(string imp:essPrimeImp){
        imp_with_literal.emplace_back(imp,calLiteral(imp));
    }
    sort(imp_with_literal.begin(), imp_with_literal.end(),QuineMccluskey::ImpWithLit::impCmp);
    int totalLiteral = 0;
    for(ImpWithLit imp:imp_with_literal){
        int cnt = 0;
        for(char c:imp.binary){
            if(c == '-'){
                cnt++;
                continue;
            }
            if(c == '1') output << (char)('A'+cnt);
            else output << (char)('A'+cnt) << '\'';
            ++cnt;
        }
        totalLiteral += imp.literal;
        output << '\n';
    }
    output << "literal=" << totalLiteral;
}
