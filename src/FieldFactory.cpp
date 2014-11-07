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
//        case 0 : return new LeaderField(marcnr);
//        case 5 : return new Field005(marcnr);
//        case 8 : return new Field008(marcnr);
        default: return new MarcField(marcnr);
    }
}
