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

MarcField* FieldFactory::getMarcField(int marcnr)//, MarcRecord* parent)
{
    switch (marcnr)
    {
        case 0 : return new FieldLDR(marcnr);
        case 1 : return new Field001(marcnr);
        case 5 : return new Field005(marcnr);
        case 8 : return  new Field008(marcnr);
        case 20 : return new Field020(marcnr);
        case 22 : return new Field022(marcnr);
        case 24 : return new Field024(marcnr);
        case 28 : return new Field028(marcnr);
        case 41 : return new Field041(marcnr);
        case 100 : return new Field100(marcnr);
        case 240 : return new Field240(marcnr);
        case 245 : return new Field245(marcnr);
        case 254 : return new Field254(marcnr);
        case 260 : return new Field260(marcnr);
        case 264 : return new Field264(marcnr);
        case 300 : return new Field300(marcnr);
        case 336 : return new Field336(marcnr);
        case 382 : return new Field382(marcnr);
        case 383 : return new Field383(marcnr);
        case 384 : return new Field384(marcnr);
        case 490 : return new Field490(marcnr);
        case 500 : return new Field500(marcnr);
        case 505 : return new Field505(marcnr);
        case 510 : return new Field510(marcnr);
        case 533 : return new Field533(marcnr);
        case 534 : return new Field534(marcnr);
        case 542 : return new Field542(marcnr);
        case 583 : return new Field583(marcnr);
        case 591 : return new Field591(marcnr);
        case 650 : return new Field650(marcnr);
        case 700 : return new Field700(marcnr);
        case 952 : return new Field952(marcnr);
        default: return new MarcField(marcnr);
    }

    // Note: Won't work like below. Maybe to do with compiler support for polymorphism/dynamic cast
    /*
    MarcField* newfield = 0;
    switch (marcnr)
    {
        case 0 : newfield =  new FieldLDR(marcnr);
        case 5 : newfield =  new Field005(marcnr);
        default: newfield= new MarcField(marcnr);
    }
    //    newfield->Setparent(parent);
    //return newfield;
    */
}
