#ifndef FIELD020_H
#define FIELD020_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include <string>
#include <algorithm>
#include <vector>
#include <set>

#include <Helper.h>


class Field020 : public MarcField
{
    public:
        Field020(int nr);
        virtual ~Field020();

        virtual void update(char marcsubfield, std::string data);
    protected:
    private:

        static std::set<std::string> knownInvalidISBNs;
        static std::set<std::string> init();
};

#endif // FIELD020_H
