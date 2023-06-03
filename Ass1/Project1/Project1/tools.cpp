#include "tools.h"

using namespace std;

// create the binary form of registers in unordered map
map<string, string> createRegMap()
{
    map<string, string> regMap;
    regMap["$zero"] = "00000";
    regMap["$v0"] = "00010";
    regMap["$v1"] = "00011";
    regMap["$a0"] = "00100";
    regMap["$a1"] = "00101";
    regMap["$a2"] = "00110";
    regMap["$t0"] = "01000";
    regMap["$t1"] = "01001";
    regMap["$t2"] = "01010";
    regMap["$t3"] = "01011";
    regMap["$t4"] = "01100";
    regMap["$t5"] = "01101";
    regMap["$t6"] = "01110";
    regMap["$t7"] = "01111";
    regMap["$s0"] = "10000";
    regMap["$s1"] = "10001";
    regMap["$s2"] = "10010";
    regMap["$s3"] = "10011";
    regMap["$s4"] = "10100";
    regMap["$s5"] = "10101";
    regMap["$s6"] = "10110";
    regMap["$s7"] = "10111";
    regMap["$t8"] = "11000";
    regMap["$t9"] = "11001";
    regMap["$k0"] = "11010";
    regMap["$k1"] = "11011";
    regMap["$gp"] = "11100";
    regMap["$sp"] = "11101";
    regMap["$fp"] = "11110";
    regMap["$ra"] = "11111";
    return regMap;
}
// create the binary form of registers in unordered map
const map<string, string> regMap = createRegMap();


// create maps between assembly and machine code
// R-Type
// xxx rd rs rt
const map<string, string> R1 = {
    make_pair("add", "000000ssssstttttddddd00000100000"),
    make_pair("addu", "000000ssssstttttddddd00000100001"),
    make_pair("and", "000000ssssstttttddddd00000100100"),
    make_pair("nor", "000000ssssstttttddddd00000100111"),
    make_pair("or", "000000ssssstttttddddd00000100101"),
    make_pair("slt", "000000ssssstttttddddd00000101010"),
    make_pair("sltu", "000000ssssstttttddddd00000101011"),
    make_pair("sub", "000000ssssstttttddddd00000100010"),
    make_pair("subu", "000000ssssstttttddddd00000100011"),
    make_pair("xor", "000000ssssstttttddddd00000100110")
};
// xxx
const map<string, string> R2 = {
    make_pair("syscall", "00000000000000000000000000001100")
};
// xxx rs rt
const map<string, string> R3 = {
    make_pair("div", "000000sssssttttt0000000000011010"),
    make_pair("divu", "000000sssssttttt0000000000011011"),
    make_pair("mult", "000000sssssttttt0000000000011000"),
    make_pair("multu", "000000sssssttttt0000000000011001")
};
// xxx rd rs
const map<string, string> R4 = {
    make_pair("jalr", "000000sssss00000ddddd00000001001")
};
// xxx rs
const map<string, string> R5 = {
    make_pair("jr", "000000sssss000000000000000001000"),
    make_pair("mthi", "000000sssss000000000000000010001"),
    make_pair("mtlo", "000000sssss000000000000000010011")
};
// xxx rd
const map<string, string> R6 = {
    make_pair("mfhi", "0000000000000000ddddd00000010000"),
    make_pair("mflo", "0000000000000000ddddd00000010010")
};
// xxx rd rt sa
const map<string, string> R7 = {
    make_pair("sll", "00000000000tttttdddddaaaaa000000"),
    make_pair("sra", "00000000000tttttdddddaaaaa000011"),
    make_pair("srl", "00000000000tttttdddddaaaaa000010")
};
// xxx rd rt rs
const map<string, string> R8 = {
    make_pair("sllv", "000000ssssstttttddddd00000000100"),
    make_pair("srav", "000000ssssstttttddddd00000000111"),
    make_pair("srlv", "000000ssssstttttddddd00000000110"),
};

// I-Type
// xxx rt rs imm
const map<string, string> I1 = {
    make_pair("addi", "001000ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("addiu", "001001ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("andi", "001100ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("ori", "001101ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("slti", "001010ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("sltiu", "001011ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("xori", "001110ssssstttttiiiiiiiiiiiiiiii")
};
// xxx rs rt label
const map<string, string> I2 = {
    make_pair("beq", "000100ssssstttttllllllllllllllll"),
    make_pair("bne", "000101ssssstttttllllllllllllllll")
};
// xxx rs label
const map<string, string> I3 = {
    make_pair("bgez", "000001sssss00001llllllllllllllll"),
    make_pair("bgtz", "000111sssss00000llllllllllllllll"),
    make_pair("blez", "000110sssss00000llllllllllllllll"),
    make_pair("bltz", "000001sssss00000llllllllllllllll")
};
// xxx rt imm(rs)
const map<string, string> I4 = {
    make_pair("lb", "100000ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("lbu", "100100ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("lh", "100001ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("lhu", "100101ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("lw", "100011ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("sb", "101000ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("sh", "101001ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("sw", "101011ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("lwl", "100010ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("lwr", "100110ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("swl", "101010ssssstttttiiiiiiiiiiiiiiii"),
    make_pair("swr", "101110ssssstttttiiiiiiiiiiiiiiii")
};
// xxx rt imm
const map<string, string> I5 = {
    make_pair("lui", "00111100000tttttiiiiiiiiiiiiiiii")
};

// J-Type
// xxx label
const map<string, string> J = {
    make_pair("j", "000010llllllllllllllllllllllllll"),
    make_pair("jal", "000011llllllllllllllllllllllllll")
};
// find labels and record their address
map<string, int> findLabels(ifstream& infile) {
    int addr = 4194304;  // 0x400000(base 16) = 4194304(base 10)
    map<string, int> LabelTable;
    string line, t1, t2;
    bool textFlag = false;
    while (getline(infile, line)) {
        // delete the comments
        if (line.find("#") != string::npos) {
            line = line.substr(0, line.find("#"));
        }
        istringstream detect(line);
        detect >> t1 >> t2;
        // find .text part
        if (t1 == ".text") {
            textFlag = true;
            t1 = "";
            t2 = "";
            continue;
        }
        if (textFlag == false) {
            t1 = "";
            t2 = "";
            continue;
        }
        if (t1.empty() == false) {
            // find and store labels
            if (t1.find(":") != string::npos) {
                LabelTable[t1.substr(0, t1.size() - 1)] = addr;
                // check if there is a instruction after the label
                if (t2.empty() == false) {
                    addr += 4;
                    t1 = "";
                    t2 = "";
                }
                else {
                    t1 = "";
                    t2 = "";
                }
            }
            else {
                addr += 4;
                t1 = "";
                t2 = "";
            }
        }
    }
    return LabelTable;
}


// convert the number from decimal form to two's complement form with specific size
string decToTwoComplement(int num, int len)
{
    string res;
    // num >= 0, two's complement form equals to binary form
    if (num >= 0) {
        while (num != 0) {
            res.insert(0, num % 2 == 1 ? "1" : "0");
            num /= 2;
        }
        while (res.length() < len) {
            int x = len - res.length();
            res.insert(0, "0");
        }
    }
    // num < 0, abs(num)'s 1'complement + 1 == num's 2's complement
    else {
        int tmp = abs(num);
        while (tmp != 0) {
            res.insert(0, tmp % 2 == 1 ? "0" : "1");
            tmp /= 2;
        }
        for (int i = res.length() - 1; i >= 0; i--) {
            if (res[i] == '0') {
                res[i] = '1';
                break;
            }
            res[i] = '0';
        }
        while (res.length() < len) {
            int x = len - res.length();
            res.insert(0, "1");
        }
    }
    return res;
}


// replace "<", "(", ")" with " " in the instruction
void reline(string& line, const string& before, const string& after)
{
    size_t pos = 0;
    while ((pos = line.find(before, pos)) != string::npos) {
        line.replace(pos, after.size(), after);
        pos += after.size();
    }
}


// convert instruction into machine code
string assembler(string t1, string t2, string t3, string t4, map<string, int>& labels, int& curAddr)
{
    string res;

    if (R1.count(t1) == 1) {
        res = R1.at(t1);
        res.replace(res.find("d"), 5, regMap.at(t2));
        res.replace(res.find("s"), 5, regMap.at(t3));
        res.replace(res.find("t"), 5, regMap.at(t4));
    }
    else if (R2.count(t1) == 1) {
        res = R2.at(t1);
    }
    else if (R3.count(t1) == 1) {
        res = R3.at(t1);
        res.replace(res.find("s"), 5, regMap.at(t2));
        res.replace(res.find("t"), 5, regMap.at(t3));
    }
    else if (R4.count(t1) == 1) {
        res = R4.at(t1);
        res.replace(res.find("d"), 5, regMap.at(t2));
        res.replace(res.find("s"), 5, regMap.at(t3));
    }
    else if (R5.count(t1) == 1) {
        res = R5.at(t1);
        res.replace(res.find("s"), 5, regMap.at(t2));
    }
    else if (R6.count(t1) == 1) {
        res = R6.at(t1);
        res.replace(res.find("d"), 5, regMap.at(t2));
    }
    else if (R7.count(t1) == 1) {
        res = R7.at(t1);
        res.replace(res.find("d"), 5, regMap.at(t2));
        res.replace(res.find("t"), 5, regMap.at(t3));
        res.replace(res.find("a"), 5, regMap.at(t4));
    }
    else if (R8.count(t1) == 1) {
        res = R8.at(t1);
        res.replace(res.find("d"), 5, regMap.at(t2));
        res.replace(res.find("t"), 5, regMap.at(t3));
        res.replace(res.find("s"), 5, regMap.at(t4));
    }

    else if (I1.count(t1) == 1) {
        res = I1.at(t1);
        res.replace(res.find("t"), 5, regMap.at(t2));
        res.replace(res.find("s"), 5, regMap.at(t3));
        int tmp = atoi(t4.c_str());
        res.replace(res.find("i"), 16, decToTwoComplement(tmp, 16));
    }
    else if (I2.count(t1) == 1) {
        res = I2.at(t1);
        res.replace(res.find("s"), 5, regMap.at(t2));
        res.replace(res.find("t"), 5, regMap.at(t3));
        string tmp = decToTwoComplement((labels.at(t4) - curAddr - 4) / 4, 16);
        res.replace(res.find("l"), 16, tmp);

    }
    else if (I3.count(t1) == 1) {
        res = I3.at(t1);
        res.replace(res.find("s"), 5, regMap.at(t2));
        string tmp = decToTwoComplement((labels.at(t3) - curAddr - 4) / 4, 16);
        res.replace(res.find("l"), 16, tmp);
    }
    else if (I4.count(t1) == 1) {
        res = I4.at(t1);
        res.replace(res.find("t"), 5, regMap.at(t2));
        int tmp = atoi(t3.c_str());
        res.replace(res.find("i"), 16, decToTwoComplement(tmp, 16));
        res.replace(res.find("s"), 5, regMap.at(t4));
    }
    else if (I5.count(t1) == 1) {
        res = I5.at(t1);
        res.replace(res.find("t"), 5, regMap.at(t2));
        int tmp = atoi(t3.c_str());
        res.replace(res.find("i"), 16, decToTwoComplement(tmp, 16));
    }

    else if (J.count(t1) == 1) {
        res = J.at(t1);
        string tmp = decToTwoComplement(labels.at(t2), 32);
        tmp = tmp.substr(0, tmp.size() - 2);
        while (tmp.size() > 26) {
            tmp.erase(0, 1);
        }
        res.replace(res.find("l"), 26, tmp);
    }
    return res;
}


// creater the output file
void getOutFile(ifstream& infile, ofstream& outfile, map<string, int>& labels) {
    int curAddr = 4194304;  // 0x400000(base 16) = 4194304(base 10)
    bool textFlag = false;
    string line, t1, t2, t3, t4, t5;
    while (getline(infile, line)) {
        // delete the comments
        if (line.find("#") != string::npos) {
            line = line.substr(0, line.find("#"));
        }

        // replace ",", "(", ")" with " "
        reline(line, ",", " ");
        reline(line, "(", " ");
        reline(line, ")", " ");

        istringstream detect(line);
        detect >> t1 >> t2 >> t3 >> t4 >> t5;
        // find .text part
        if (t1 == ".text") {
            textFlag = true;
            t1 = "";
            t2 = "";
            t3 = "";
            t4 = "";
            t5 = "";
            continue;
        }
        if (textFlag == false) {
            t1 = "";
            t2 = "";
            t3 = "";
            t4 = "";
            t5 = "";
            continue;
        }
        // line is not empty
        if (t1.empty() == false) {
            // t1 is a label
            if (t1.find(":") != string::npos) {
                // there is a instruction after the label
                if (t2.empty() == false) {
                    string res = assembler(t2, t3, t4, t5, labels, curAddr);
                    outfile << res << endl;
                    curAddr += 4;
                }
                t1 = "";
                t2 = "";
                t3 = "";
                t4 = "";
                t5 = "";
            }
            // t1 is not a label
            else {
                string res = assembler(t1, t2, t3, t4, labels, curAddr);
                outfile << res << endl;
                t1 = "";
                t2 = "";
                t3 = "";
                t4 = "";
                t5 = "";
                curAddr += 4;
            }
        }
    }
}


// compare the outputfile with the expected
bool compareFiles(ifstream& f1, ifstream& f2) {
    char a, b;
    while (!f1.eof()) {
        f1 >> a;
        f2 >> b;
        if (a != b)
            return 0;
    }
    return 1;
}