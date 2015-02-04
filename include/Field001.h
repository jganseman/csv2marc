#ifndef FIELD001_H
#define FIELD001_H

#include <set>
#include <string>
#include <MarcField.h>
#include <MarcRecordException.h>


// this class is only used to detect doubles of shelf numbers
class Field001 : public MarcField
{
    public:
        Field001(int nr);
        virtual ~Field001();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
        static std::set<std::string> usedIDs;
        static std::set<std::string> init();
};

#endif // FIELD001_H
