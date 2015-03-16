#ifndef FIELDFACTORY_H
#define FIELDFACTORY_H

#include <MarcField.h>
#include <FieldLDR.h>
#include <Field001.h>
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
#include <Field260.h>
#include <Field264.h>
#include <Field382.h>
#include <Field383.h>
#include <Field384.h>
#include <Field490.h>
#include <Field505.h>
#include <Field510.h>
#include <Field650.h>
#include <Field700.h>
#include <Field952.h>

//Forward declaration of class MarcRecord to be able to use ptr to parent
// class MarcRecord;

class FieldFactory
{
    public:
        virtual ~FieldFactory();
        static FieldFactory* getFactory();
        static MarcField* getMarcField (int marcnr);//, MarcRecord* parent=0);
    protected:
        FieldFactory();
    private:
        static FieldFactory* singletonFactory;
};

#endif // FIELDFACTORY_H

