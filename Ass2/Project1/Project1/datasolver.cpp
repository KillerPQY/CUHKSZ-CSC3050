#include <string>
#include <fstream>
#include <sstream>
#include "datasolver.h"
#include <iostream>

using namespace std;

int* ASCII(string data, int* dp, bool z) {
	char* cp = (char*)dp;
	int l = data.size();
	for (int i = 0; i < l; i++) {
		*cp = data[i];
		cp++;
	}

	// if it is asciiz, add '\0'
	if (z) {
		*cp = '\0';
		l++;
	}

	// Four bytes are stored once
	int q = l / 4;
	int r = l % 4;
	if (r != 0) q++;
	// if the data is empty
	if (q == 0)
		q = 1;
	return dp + q;
}

int* WORD(string data, int* dp) {
	int tmp = 0;
	bool neg = 0;  // the word may be negative

	// store each word except the last one
	for (int i = 0; i < data.size(); i++) {
		if (data[i] == ',') {
			if (neg == 1) {
				tmp = -tmp;
				neg = 0;
			}
			*dp = tmp;
			dp++;
			tmp = 0;
		}
		else if (data[i] == '-') {
			neg = 1;
		}
		else {
			tmp = tmp * 10 + (data[i] - '0');
		}
	}

	// store the last word
	if (neg == 1) tmp = -tmp;
	*dp = tmp;
	dp++;
	return dp;

}

int* BYTE(string data, int* dp) {
	int tmp = 0;         // .byte 1,2
	char chtmp = ' ';    // .byte 'a','b'
	int times = 0;
	char* cp = (char*)dp;
	bool charflag = 0;

	// store each byte except the last one
	for (int i = 0; i < data.size(); i++) {
		if (data[i] == ',') {
			if (!charflag) chtmp = tmp;
			*cp = chtmp;
			cp++;
			times++;
			tmp = 0;
			charflag = 0;
		}
		else if (data[i] == '\'') {
			charflag = 1;
		}
		else if (charflag) {
			chtmp = data[i];
		}
		else {
			tmp = tmp * 10 + (data[i] - '0');
		}
	}

	// store the last byte
	if (!charflag) chtmp = tmp;
	*cp = chtmp;
	times++;

	// Four bytes are stored once
	int q = times / 4;
	int r = times % 4;
	if (r != 0) q++;

	// pointer after dealing with the bytes
	return dp + q;
}

int* HALF(string data, int* dp) {
	int tmp = 0;         // .half 1,2
	char chtmp = ' ';    // .half 'a','b'
	int times = 0;
	unsigned short* cp = (unsigned short*)dp;
	bool charflag = 0;

	// store each half except the last one
	for (int i = 0; i < data.size(); i++) {
		if (data[i] == ',') {
			if (!charflag) chtmp = tmp;
			*cp = chtmp;
			cp++;
			times++;
			tmp = 0;
			charflag = 0;
		}
		else if (data[i] == '\'') {
			charflag = 1;
		}
		else if (charflag) {
			chtmp = data[i];
		}
		else {
			tmp = tmp * 10 + (data[i] - '0');
		}
	}

	// store the last half
	if (!charflag) chtmp = tmp;
	*cp = chtmp;
	times++;

	// two halfs are stored once
	int q = times / 2;
	int r = times % 2;
	if (r != 0) q++;

	// pointer after dealing with the halfs
	return dp + q;
}

int* solver(ifstream& infile, int* dp) {
	string line, t1, t2, t3;
	bool dataflag = false;

	while (getline(infile, line)) {
		// delete the comments
		if (line.find("#") != string::npos) {
			line = line.substr(0, line.find("#"));
		}
		stringstream detect(line);
		detect >> t1 >> t2;
		if (t1 == ".data") {
			dataflag = true;
			t1 = "";
			t2 = "";
			continue;
		}
		if (dataflag == false) {
			t1 = "";
			t2 = "";
			continue;
		}
		if (t1 == ".text") {
			break;
		}
		// not an empty line after deleting comments
		if (t1.empty() == false) {
			if (t2 == ".ascii") {
				int start = line.find("\"");
				int end = line.rfind("\"");
				line = line.substr(start + 1, end - start - 1);
				while (line.find("\\n") != string::npos) {
					string tmp1 = line.substr(0, line.find("\\n"));
					string tmp2 = line.substr(line.find("\\n") + 2, line.size() - 2 - tmp1.size());
					line = tmp1 + '\n' + tmp2;
				}
				while (line.find("\\0") != string::npos) {
					string tmp1 = line.substr(0, line.find("\\0"));
					string tmp2 = line.substr(line.find("\\0") + 2, line.size() - 2 - tmp1.size());
					line = tmp1 + '\0' + tmp2;
				}
				dp = ASCII(line, dp, 0);
				t1 = "";
				t2 = "";
			}
			else if (t2 == ".asciiz") {

				int start = line.find("\"");
				int end = line.rfind("\"");
				line = line.substr(start + 1, end - start - 1);
				while (line.find("\\n") != string::npos) {
					string tmp1 = line.substr(0, line.find("\\n"));
					string tmp2 = line.substr(line.find("\\n") + 2, line.size() - 2 - tmp1.size());
					line = tmp1 + '\n' + tmp2;
				}
				while (line.find("\\0") != string::npos) {
					string tmp1 = line.substr(0, line.find("\\0"));
					string tmp2 = line.substr(line.find("\\0") + 2, line.size() - 2 - tmp1.size());
					line = tmp1 + '\0' + tmp2;
				}
				dp = ASCII(line, dp, 1);
				t1 = "";
				t2 = "";

			}
			else if (t2 == ".word") {
				string t3, tmp;
				while (detect >> tmp) {
					t3 = t3 + tmp;
				}
				dp = WORD(t3, dp);
				t1 = "";
				t2 = "";
			}
			else if (t2 == ".byte") {
				string t3, tmp;
				while (detect >> tmp) {
					t3 = t3 + tmp;
				}
				dp = BYTE(t3, dp);
				t1 = "";
				t2 = "";
			}
			else if (t2 == ".half") {
				string t3, tmp;
				while (detect >> tmp) {
					t3 = t3 + tmp;
				}
				dp = HALF(t3, dp);
				t1 = "";
				t2 = "";
			}
		}
	}
	return dp;
}