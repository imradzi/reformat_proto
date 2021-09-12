#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <vector>

std::string globalString {
R"(
    class X {
        int a;  // esting
        double b;
    };
    LexiconRecord abc {10};
    bool abcdef {10};

    class ServerKeyRecord {
        bool autoGenerateMembershipCode;
        bool autoGenerateSupplierCode;
        bool doRunHereIfServerNotFound;  // run command here if not found
        bool enableCheckPoisonSales;
        bool enableCheckPseudoPoisonSales;
        bool enableCreditLimitCheck;
        bool enableFractionalPackSize;
        bool enableFractionalQuantity;
        bool hasGST;
        bool invoiceFormatPortrait;
        bool invoiceShowCustomerAllPages;
    }

    struct MessageLayer {
        int layer;  // for receiptPrinter layer=1 for Header; layer=2 for Footer
            // for customerDisplay layer=n for each page of the display ???
        std::vector<wxString> messages;
    };
    
    struct MessageRecord : public wpObject {
        int64_t id {0};
        wxDateTime effectiveDate;   // for searching
        LexiconRecord messageType;  // 1=receiptPrinter, 2=customerDisplay
        wxString priority;
        wxDateTime fromDate;
        wxDateTime toDate;
        std::vector<std::shared_ptr<MessageLayer>> messages;
    };
    
    struct ServerEventMessage : public wpObject {
        int test;
        struct MessageRecord {
            wxString code;
            wxString key;
            std::string value;
        };
        int type;
        bool isBroadcast;
        bool isMobileNotification;
        std::vector<std::shared_ptr<MessageRecord>> messages;
    };

)"};

size_t countChar(const std::string& word, char c) {
    size_t count = 0;
    for (auto const& x : word) {
        if (x == c) count++;
    }
    return count;
}

bool IsEmpty(std::string w) {
    for (const auto& x : w)
        if (!isspace(x)) return false;
    return true;
}

static std::unordered_map<std::string, std::string> convertList {
    {"wxString", "string"},
    {"std::string", "string"},
    {"int64_t", "int64"},
    {"int", "int32"},
    {"long", "int64"},
    {"wxLongLong", "int64"},
    {"wxDateTime", "int64"},
    {"std::vector<std::shared_ptr<MessageRecord>>", "repeated MessageRecord"}

};

std::string process(std::string w, bool justTranslate, char &endwith) {
    if (IsEmpty(w)) return w;
    if (w.back() == ';') {
        endwith = ';';
        w.pop_back();
        if (IsEmpty(w)) return w;
    }

    if (convertList.find(w) != convertList.end()) {
        return convertList[w];
    }

    if (w.find("std::vector") != w.npos) {
        auto idx = w.find_last_of('<');
        auto final = w.substr(idx + 1).find_first_of('>');
        std::string term = w.substr(idx+1, final);
        if (convertList.find(term) != convertList.end())
            return "repeated " + convertList[term];
        else
            return "repeated " + term;
    }
    if (justTranslate) return w;

    std::string res;

    auto isCaseChanged = [](char prev, char c) { 
        if (!std::isalpha(prev) || !std::isalpha(c)) return false;
        return (std::islower(prev) && !std::islower(c)) || (!std::islower(prev) && std::islower(c)); 
    };

    bool justchanged = false;
    for (size_t i = 0; i < w.length(); i++) {
        if (i == 0 || i == w.length() - 1)
            res.push_back(std::tolower(w[i]));
        else if (std::isspace(w[i - 1]))
            res.push_back(w[i]);
        else if (std::isalpha(w[i])) {
            if (isCaseChanged(w[i - 1], w[i])) {
                if (!justchanged) res.push_back('_');
                res.push_back(std::tolower(w[i]));
                justchanged = true;
            } else {
                res.push_back(std::tolower(w[i]));
                justchanged = false;
            }
        } else {
            res.push_back(w[i]);
            justchanged = false;
        }
    }
    return res;
}

static std::unordered_set<std::string> keywords {"class", "struct", "public"};

void PrintLine(std::vector<std::string>& v, std::vector<std::string> &comment, int &seq) {
    std::string delim {};
    int nw = 0;
    char endwith {};
    std::string lastword {};
    char lastEndWith {};
    for (int i = 0; i < v.size(); i++) {
        endwith = 0;
        if (i > 0 && keywords.find(v[i - 1]) != keywords.end()) {
            std::cout << delim <<  process(v[i], true, endwith);
        } else if (nw == 0) {
            if (convertList.find(v[i]) != convertList.end() || v[i].find("std::vector") != v[i].npos)
                std::cout << delim << process(v[i], true, endwith);
            else 
                std::cout << delim << v[i];
        } else {
            std::cout << delim << process(v[i], false, endwith);
        }
        delim = " ";
        if (!IsEmpty(v[i])) {
            lastword = v[i];
            lastEndWith = endwith;
            nw++;
        }
    }
    if (!lastword.empty() && lastEndWith == ';') {
        if (lastword != "};") 
            std::cout << " = " << ++seq << ";";
    }
    if (!comment.empty()) {
        for (auto const& c : comment) {
            std::cout << " " << c;
        }
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {

    std::string all;
    if (argc > 1) {
        std::ifstream file(argv[1]);
        std::string temp;
        while (std::getline(file, temp)) {
            all.append(temp);
            all.append("\n");
        }
    } else
        all = globalString;

    if (all.empty()) return 0;
    std::string word {};
    std::stack<int> seqStack;
    seqStack.push(0);
    std::vector<std::string> line;
    std::vector<std::string> comment;

    auto fnCheckStruct = [&]() {
        if (word == "struct" || word == "class")
            seqStack.push(0);
        else if (word == "};") {
            if (!seqStack.empty()) seqStack.pop();
        }
    };

    for (const auto& w : all) {
        if (w == '\n') {
            fnCheckStruct();
            if (comment.empty()) {
                line.push_back(word);
                word.clear();
            } else {
                comment.push_back(word);
                word.clear();
            }
            // get rid of all trailing blank after ; in line and move to comment
            bool atEndofline = false;
            for (int i = 0; i < line.size(); i++) {
                if (atEndofline) {
                    line.erase(line.begin()+i);
                    comment.insert(comment.begin(), "");
                } else {
                    atEndofline = !line[i].empty() && line[i].back() == ';';
                }
            }
            PrintLine(line, comment, seqStack.top());
            line.clear();
            comment.clear();
        } else if (std::isspace(w)) {
            fnCheckStruct();
            if (word == "//" || !comment.empty()) {
                comment.push_back(word);
                word.clear();
            } else {
                line.push_back(word);
                word.clear();
            }
        } else if (std::isprint(w)) {
            word.push_back(w);
        }
    }
}



//int mainX(int argc, char* argv[]) {
//    std::unordered_set<std::string> keywords {"class", "struct", "public"};
//
//    std::string word {};
//    bool ignoreNext {false};
//    bool ignoreUntilEOL = false;
//    int seq = 0;
//    
//    std::string v;
//    if (argc > 1) {
//        std::ifstream file(argv[1]);
//        std::string temp;
//        while (std::getline(file, temp)) {
//            v.append(temp);
//            v.append("\n");
//        }
//    } else
//        v = globalString;
//
//
//    auto fnEol = [&](char x) { 
//        if (x == ';' && word != "}")
//            std::cout << " = " << ++seq << ";";
//        else
//            std::cout << x;
//        word.clear();
//
//    };
//    std::stack<int> noSeq;
//    noSeq.push(0);
//    noSeq.push(0);
//    int wordNo = 0;
//    char endwith {};
//    for (const auto& x : v) {
//        if (ignoreUntilEOL) {
//            if (x != 0x0D && x != 0x0A) {
//                std::cout << x;
//                continue;
//            }
//            word.clear();
//            ignoreUntilEOL = false;
//        }
//        if (word == "//") {
//            ignoreUntilEOL = true;
//        }
//        if (!IsEmpty(word) && (std::isspace(x) || x == ';')) {
//            if (x ==';' && word == "}") {
//                if (!noSeq.empty()) {
//                    seq = noSeq.top();
//                    noSeq.pop();
//                } else
//                    seq = 0;
//            }
//            if (ignoreNext) {
//                std::cout << word;
//                ignoreNext = false;
//                fnEol(x);
//                continue;
//            }
//            if (keywords.contains(word)) {
//                ignoreNext = true;
//                if (word == "struct" || word == "class") {
//                    noSeq.push(seq);
//                    seq = 0;
//                    std::cout << "message";
//                } else 
//                    std::cout << word;
//                fnEol(x);
//                continue;
//            } else if (!IsEmpty(word)) {
//                std::cout << process(word, true, endwith);
//                ignoreNext = false;
//            }
//            fnEol(x);
//        } else {
//            if (!isspace(x)) word.push_back(x);
//        } 
//        if (x == 0x0a || x == 0x0d) {
//            ignoreNext = true;
//        } 
//    }
//    return 0;
//}
//
