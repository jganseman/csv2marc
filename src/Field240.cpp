#include "Field240.h"

Field240::Field240(int nr) : MarcField(nr)
{
    //ctor
}

Field240::~Field240()
{
    //dtor
}

void Field240::update(char marcsubfield, std::string data)
{
    if (data.empty() || data == "")
        return;

    if (marcsubfield == 'a')        // this is the main title
    {

        Setindicator1('1');      // always display title on screen
        Setindicator2('0');

        //trim front and trailing whitespace
        data = data.erase(data.find_last_not_of(" \n\r\t")+1).substr(data.find_first_not_of(" \n\r\t"));

        // put data already in
        MarcField::update(marcsubfield, data);

        //indicator 2 is the nr of nonfiling characters (i.e. how much space to leave out for articles)
        // so, calculate nr of letters used by articles in NL, FR, EN

        std::string upper = data;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

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
