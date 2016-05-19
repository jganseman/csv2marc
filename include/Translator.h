#ifndef TRANSLATOR_H
#define TRANSLATOR_H

using namespace std;

#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <Helper.h>

class Translator
{
    public:
        Translator(std::string filename = "");
        virtual ~Translator();
        std::string translate(std::string& input);
        std::string translatepartial(std::string& input);
        bool isKey(std::string& input);
        bool isValue(std::string& input);
    protected:
    private:
        std::map<std::string, std::string> mapping;
        std::set<std::string> keys;
        std::set<std::string> values;
};

#endif // TRANSLATOR_H
