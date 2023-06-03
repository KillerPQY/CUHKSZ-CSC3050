#ifndef DATASOLVER_H
#define DATASOLVER_H

#include <string>
#include <fstream>


// .ascii and .asciiz
int* ASCII(std::string data, int* dp, bool z);
// .word
int* WORD(std::string data, int* dp);
// .byte
int* BYTE(std::string data, int* dp);
// .half
int* HALF(std::string data, int* dp);


// store different types of data
int* solver(std::ifstream& infile, int* dp);


#endif