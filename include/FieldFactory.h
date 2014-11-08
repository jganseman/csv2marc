#ifndef FIELDFACTORY_H
#define FIELDFACTORY_H

#include <MarcField.h>
#include <FieldLDR.h>
#include <Field008.h>

class FieldFactory
{
    public:
        virtual ~FieldFactory();
        static FieldFactory* getFactory();
        static MarcField* getMarcField (int marcnr);
    protected:
        FieldFactory();
    private:
        static FieldFactory* singletonFactory;
};

#endif // FIELDFACTORY_H

