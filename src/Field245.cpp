#include "Field245.h"

Field245::Field245(int nr) : MarcField(nr)
{
    //ctor
}

Field245::~Field245()
{
    //dtor
}


// copied mostly from Field240.cpp
void Field245::update(char marcsubfield, std::string data)
{
    try{
        //trim front and trailing whitespace
        Helper::Trim(data);
    } catch (exception e)
    {
        // this will throw an error if data is empty and return
        if (verbose) throw MarcRecordException("Empty title");
        return;
    }

    if (data.empty() || data == "")
        return;

    Helper::Remove(data, "¬");

    if (marcsubfield == 'a')        // this is the main title
    {

        Setindicator1('1');      // by default, add title entry into the catalog
        Setindicator2('0');

        // put data already in
        MarcField::update(marcsubfield, data);

        //indicator 2 is the nr of nonfiling characters (i.e. how much space to leave out for articles)
        // so, calculate nr of letters used by articles in NL, FR, EN

        std::string upper = data;
        Helper::MakeUppercase(upper);

        // dutch articles, make sure to have whitespace after each!
        const char* initlist[] = {"DE ", "HET ", "EEN ", "L'", "LA ", "LE ", "LES ", "UN ", "UNE ", "A ", "AN ", "THE " };
        std::vector<std::string> articles(initlist, initlist+12);       //TODO: get this hardcoded size out
        int nrarticles = articles.size();

        for (int i=0; i<nrarticles;i++)
        {
            if (upper.compare(0, articles[i].length(), articles[i]) == 0) {         // if string begins with article
                char buffer [33];       // for safety let's say 32bytes + null
                itoa( articles[i].length(), buffer, 10);
                Setindicator2( buffer[0] );
            }
        }

    }


}
