#ifndef FIELD650_H
#define FIELD650_H

#include <MarcField.h>
#include <MarcRecordException.h>
#include <Helper.h>
#include <Translator.h>

#include <string>
#include <algorithm>
#include <vector>

class Field650 : public MarcField
{
    public:
        Field650(int nr);
        virtual ~Field650();
        virtual void update(char marcsubfield, std::string data);
    protected:
    private:
        static Translator t650a;
};

#endif // FIELD650_H
