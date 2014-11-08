#ifndef FIELD024_H
#define FIELD024_H

#include <MarcField.h>
#include <MarcRecordException.h>

#include <string>
#include <algorithm>
#include <vector>


class Field024 : public MarcField
{
    public:
        Field024(int nr);
        virtual ~Field024();

        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD024_H
