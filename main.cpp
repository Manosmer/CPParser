#include <iostream>
#include <stdlib.h>

#include "ArgParser.h"



// prototyping declaration
void foo(char* [], int, cpparser::ArgParser::CBStatus);



int main(int argc, const char* argv[])
{
    char* testStr;
    char* callbackTestStr;

    cpparser::ArgParser p(argc, argv);

    cpparser::Label labels[] = {
        {"-c", false},
        {"-void", true},
        {"-int", false},
        {"-arg1", false}
    };
    // adding elements
    p.addLabels(labels, 4);


    // lambda
    p.on<void>("-c", [](char* args[], int argsCnt, cpparser::ArgParser::CBStatus status) {
        // prints the arguments corresponding to "-c"
        if(status == cpparser::ArgParser::CBStatus::Good) {
            for(int i = 0; i < argsCnt; i++) {
                std::cout << args[i] << std::endl;

            }
        }
    });

    // or pass a named function
    p.on<void>("-c", foo);



    // if label was not defined with ArgParser::addLabel or ArgParser::addLabels status = LabelUndefined
    callbackTestStr = p.on<char*>("undefinedLabel", [&testStr](char* args[], int argsCnt, cpparser::ArgParser::CBStatus status) {
        testStr = new char[20];

        if(status == cpparser::ArgParser::CBStatus::LabelUndefined) {
            std::strcpy(testStr, "undef label");
        } else {
            std::strcpy(testStr, "All GOOD");
        }
        
        return testStr;
    });

    std::cout << callbackTestStr << std::endl;

    delete[] testStr;



    system("pause");

    return 0;
}


// foo definition
void foo(char* args[], int argsCnt, cpparser::ArgParser::CBStatus status) {
    if(status == cpparser::ArgParser::CBStatus::Good) {
        for(int i = 0; i < argsCnt; i++) {
            std::cout << args[i] << std::endl;

        }
    }
}