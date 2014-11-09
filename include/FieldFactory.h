#ifndef FIELDFACTORY_H
#define FIELDFACTORY_H

#include <MarcField.h>
#include <FieldLDR.h>
#include <Field005.h>
#include <Field008.h>
#include <Field020.h>
#include <Field022.h>
#include <Field024.h>
#include <Field041.h>
#include <Field100.h>

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

