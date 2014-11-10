#include "FieldFactory.h"

// Define static member here, required by linker.
FieldFactory* FieldFactory::singletonFactory = NULL;


FieldFactory::FieldFactory()
{
    //ctor
}


FieldFactory::~FieldFactory()
{
    //dtor
}


FieldFactory* FieldFactory::getFactory()
{
    if(singletonFactory == NULL)
       singletonFactory = new FieldFactory;

    return singletonFactory;
}

MarcField* FieldFactory::getMarcField(int marcnr)
{
    switch (marcnr)
    {
        case 0 : return new FieldLDR(marcnr);
        case 5 : return new Field005(marcnr);
        case 8 : return new Field008(marcnr);
        case 20 : return new Field020(marcnr);
        case 22 : return new Field022(marcnr);
        case 24 : return new Field024(marcnr);
        case 41 : return new Field041(marcnr);
        case 100 : return new Field100(marcnr);
        case 700 : return new Field700(marcnr);
        default: return new MarcField(marcnr);
    }
}
