#ifndef FIELDFACTORY_H
#define FIELDFACTORY_H

#include <MarcField.h>
#include <FieldLDR.h>
#include <Field005.h>
#include <Field008.h>
#include <Field020.h>
#include <Field022.h>
#include <Field024.h>
#include <Field028.h>
#include <Field041.h>
#include <Field100.h>
#include <Field240.h>
#include <Field245.h>
#include <Field254.h>
#include <Field264.h>
#include <Field490.h>
#include <Field505.h>
#include <Field650.h>
#include <Field700.h>

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

