#pragma once

#include <ostream>
#include <functional>
#include <string>
#include <unordered_map>


namespace cpparser {
    struct Label {
        std::string name;
        bool isSwitch;

        friend std::ostream& operator<<(std::ostream& o, Label& l) {
            o << l.name << ", s: " << l.isSwitch;
            return o;
        }
    };


    class ArgParser {
        private:
            int argc;
            //TODO argv declaration
            const char* *argv;

            bool argsParsed;

            std::unordered_map<std::string, int*> labels;

            void parseArguments();

        public:
            ArgParser(int, const char* []);
            ~ArgParser();

            enum CBStatus{
                Good,
                LabelUndefined,
                LabelNotGiven,
                ArgumentsMissing
            };


            void addLabel(Label label);
            void addLabels(Label [], const int);


            // 'on' does not free the char** argument array
            template <class _T>
            _T on(const char* label, std::function<_T(char* [], int, CBStatus)> callback) {
                int j;
                auto arg = labels.find(label);
                char** labelArguments = NULL;


                // parse the argument list the first time 'on' is called
                if(!argsParsed)
                    this->parseArguments();

                // label not defined in parser
                if(arg == labels.end()) {
                    return callback(NULL, 0, CBStatus::LabelUndefined);
                }
                
                // label not found in user's input
                if(arg->second[0] == 0) {
                    return callback(NULL, 0, CBStatus::LabelNotGiven);
                }

                // label is a switch, no args required
                if(arg->second[0] == -1){
                    return callback(NULL, 0, CBStatus::Good);
                }

                // required args not given
                if(arg->second[1] - arg->second[0] == 0) {
                    return callback(NULL, 0, CBStatus::ArgumentsMissing);
                }

                // calling callback with corresponding args
                labelArguments = new char*[arg->second[1] - arg->second[0]];

                j = 0;
                for(int i = arg->second[0]+1; i<=arg->second[1]; i++) {
                    int len = std::strlen(this->argv[i]) + 1;
                    labelArguments[j] = new char[len];
                    
                    std::strcpy(labelArguments[j], this->argv[i]);

                    j++;
                }
                

                return callback(labelArguments, j, CBStatus::Good);
            }
    };
}
