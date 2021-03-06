#include "Field240.h"

Field240::Field240(int nr) : MarcField(nr)
{
    //ctor

    Setindicator1('1');      // always display title on screen
    Setindicator2('0');
    verbose=false;
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


        Helper::Remove(data, "¬");          // legacy symbol denoting articles. Remove.
        //trim front and trailing whitespace
        Helper::Trim(data);

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

        // Throw warning in case of too many comma's.
        /*
        size_t n = std::count(data.begin(), data.end(), ',');
        if (n > 2)
            throw MarcRecordException("WARNING field 240: too many keywords in uniform title.");
        */
        // Also, list of keywords should be limited but that's for later

        // institute a warning if the data contains more than 1 ":". This happens when series title is recorded in main title
            if (data.find(':') != data.rfind(':'))
            {
                if (verbose) throw MarcRecordException("WARNING field 240 : excess semicolons in uniform title" + data);
            }
    }


}
