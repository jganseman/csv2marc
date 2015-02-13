#ifndef FIELD001_H
#define FIELD001_H

#include <set>
#include <map>
#include <string>
#include <cstdlib>
#include <MarcField.h>
#include <MarcRecordException.h>


// this class is only used to detect doubles of shelf numbers
class Field001 : public MarcField
{
    public:
        Field001(int nr);
        virtual ~Field001();
        virtual void update(char marcsubfield, std::string data);
        static void printIDcounts();
    protected:
    private:
        static std::set<std::string> usedIDs;
        static std::set<std::string> init();
        static std::map<std::string, int> IDcount;
        static std::map<std::string, int> init2();

};

#endif // FIELD001_H
