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
        case 28 : return new Field028(marcnr);
        case 41 : return new Field041(marcnr);
        case 100 : return new Field100(marcnr);
        case 240 : return new Field240(marcnr);
        case 245 : return new Field245(marcnr);
        case 254 : return new Field254(marcnr);
        case 264 : return new Field264(marcnr);
        case 490 : return new Field490(marcnr);
        case 505 : return new Field505(marcnr);
        case 650 : return new Field650(marcnr);
        case 700 : return new Field700(marcnr);
        default: return new MarcField(marcnr);
    }
}
