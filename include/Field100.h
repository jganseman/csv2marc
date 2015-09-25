#ifndef FIELD100_H
#define FIELD100_H

#include <MarcField.h>
#include <Helper.h>
#include <Translator.h>

#include <string>
#include <algorithm>
#include <vector>
#include <set>

class Field100 : public MarcField
{
    public:
        Field100(int nr);
        virtual ~Field100();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
        bool isValidRelator(std::string& data);
        //static std::set<std::string> relatorterms;
        //static std::set<std::string> init();
        void relatorFixer(std::string& data);
        static Translator t100e;
};

#endif // FIELD100_H
