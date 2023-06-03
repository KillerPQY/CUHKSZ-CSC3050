#ifndef	ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

// maps that the procedure need
//const map<string, string> regMap;
//const map<string, string> R1;
//const map<string, string> R2;
//const map<string, string> R3;
//const map<string, string> R4;
//const map<string, string> R5;
//const map<string, string> R6;
//const map<string, string> R7;
//const map<string, string> R8;
//const map<string, string> I1;
//const map<string, string> I2;
//const map<string, string> I3;
//const map<string, string> I4;
//const map<string, string> I5;
//const map<string, string> J;

// create the binary form of registers in unordered map
map<string, string> createRegMap();

// find labels and record their address
map<string, int> findLabels(ifstream& infile);

// convert the number from decimal form to two's complement form with specific size
string decToTwoComplement(int num, int len);

// replace "<", "(", ")" with " " in the instruction
void reline(string& line, const string& before, const string& after);

// convert instruction into machine code
string assembler(string t1, string t2, string t3, string t4, map<string, int>& labels, int& curAddr);

// creater the output file
void getOutFile(ifstream& infile, ofstream& outfile, map<string, int>& labels);

// compare the outputfile with the expected
bool compareFiles(ifstream& f1, ifstream& f2);

#endif