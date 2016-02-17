#ifndef FIELD022_H
#define FIELD022_H

#include <MarcField.h>
#include <MarcRecordException.h>
#include <Helper.h>

#include <string>
#include <algorithm>
#include <vector>
#include <set>


class Field022 : public MarcField
{
    public:
        Field022(int nr);
        virtual ~Field022();

        virtual void update(char marcsubfield, std::string data);
    protected:
    private:

        static std::set<std::string> knownInvalidISSNs;
        static std::set<std::string> init();
};

#endif // FIELD022_H
