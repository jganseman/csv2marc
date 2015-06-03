#ifndef FIELD260_H
#define FIELD260_H

#include <MarcField.h>
#include <Helper.h>
#include <MarcRecordException.h>

#include <vector>
#include <string>


class Field260 : public MarcField
{
    public:
        Field260(int nr);
        virtual ~Field260();
        virtual void update(char marcsubfield, std::string data);
        virtual void updateLegacy(char marcsubfield, std::string data);
    protected:
    private:
};

#endif // FIELD260_H
