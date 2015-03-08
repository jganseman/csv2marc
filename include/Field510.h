#ifndef FIELD510_H
#define FIELD510_H

#include <MarcField.h>
#include <MarcRecordException.h>
#include <Helper.h>

#include <string>
#include <algorithm>
#include <vector>


class Field510: public MarcField
{
    public:
        Field510(int nr);
        virtual ~Field510();

        virtual void update(char marcsubfield, std::string data);
        virtual std::string const print() const;
    protected:
    private:
};

#endif // FIELD510_H
