#include "ArgParser.h"


namespace cpparser {
    ArgParser::ArgParser(int argc, const char *argv[]) {
        this->argc = argc;
        this->argv = argv;
        this->argsParsed = false;
    }

    ArgParser::~ArgParser() {
        if(!labels.empty()) {
            for(auto itr = labels.begin(); itr != labels.end(); itr++) {
                delete[] itr->second;
            }
            labels.clear();
        }
    }

    void ArgParser::addLabel(Label label) {
        int* pos = new int[2];
        pos[0] = 0;
        pos[1] = 0;
        if(label.isSwitch) {
            pos[0] = -1;
            pos[1] = -1;
        }

        if(argsParsed) return;
        
        labels[label.name] = pos;
    }

    void ArgParser::addLabels(Label arglabels[], const int labelCnt) {
        if(argsParsed) return;

        for(int i = 0; i < labelCnt; i++) {
            int* pos = new int[2];
            pos[0] = 0;
            pos[1] = 0;
            if(arglabels[i].isSwitch) {
                pos[0] = -1;
                pos[1] = -1;
            }
            labels[arglabels[i].name] = pos;
        }
    }


    void ArgParser::parseArguments() {
        std::unordered_map<std::string, int*>::iterator itr;
        int* pos = NULL;

        for(int i = 1; i < this->argc; i++) {
            if((itr = labels.find(argv[i])) != labels.end()) {
                if(pos != NULL){
                    if(pos[0] != -1){
                        pos[1] = i-1;
                    }
                    
                } 

                pos = itr->second;
                if(pos[0] != -1) pos[0] = i;
            }
            
        }
        if(pos[0] != -1) pos[1] = argc-1;


        this->argsParsed = true;
    }
}