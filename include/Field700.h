#ifndef FIELD700_H
#define FIELD700_H

#include <MarcField.h>

#include <string>
#include <algorithm>
#include <vector>
#include <set>

class Field700 : public MarcField
{
    public:
        Field700(int nr);
        virtual ~Field700();
        virtual void update(char marcsubfield, std::string data);
        virtual std::string const print() const;
    protected:
    private:
        bool isValidRelator(std::string& data);
        static std::set<std::string> relatorterms;
        static std::set<std::string> init();
        void relatorFixer(std::string& data);

        // to keep the relators and date from a single subfield together,
        // we need some additional data structures to group by subfield
        //std::vector< std::multimap<char, std::string> > multisubfields;
};

#endif // FIELD700_H
