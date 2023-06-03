#include <iostream>
#include "tools.h"
using namespace std;

int main (int argc, char * argv[]) {   
    // let user enter the name of every file
    string in = argv[1];
    string out = argv[2];
    string compare = argv[3];

    ifstream infile(in);
    ifstream comfile(compare);
    ofstream outfile(out);

    map<string, int> labelTable = findLabels(infile);
    infile.close();

    infile.open(in);
    getOutFile(infile, outfile, labelTable);
    infile.close();
    outfile.close();

    infile.open(out);
    if (compareFiles(infile, comfile)) {
        cout << "ALL PASSED! CONGRATS :)" << endl;
    }
    else {
        cout << "YOU DID SOMETHING WRONG :(" << endl;
    }

    infile.close();
    comfile.close();

}