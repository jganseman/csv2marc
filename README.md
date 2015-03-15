# csv2marc
C++ program to convert library records from an ASCII csv table to MARC21 format.

(c) J. Ganseman , for the [Royal Conservatory of Brussels library](http://www.kcb.be/en/kcb/page/library/intro).

## License
[CC-BY-SA](http://creativecommons.org/licenses/by-sa/3.0/)

## Aim
This project was created to convert library records made in an CSV file format (exported from Excel, LibreOffice or another database)
to a MARC21 format. The motivation is an upgrade project at the Royal Conservatory of Brussels library (RISM siglum: B-Bc), where an old MS-DOS
database needed to be replaced by something more modern, with a web interface. Naturally, the old records needed to be preserved.
The old system was MARC-like but not MARC, and could only export to CSV. Hence the need of a conversion routine, with additional error checks
and merge / split functionalities, to get the data into a MARC-supporting library system like [Koha](http://www.koha.org/).

The code is still a little messy, most lines are pretty much written as the need arose. Suggestions for improvements are most welcomed!

## Use
You'll need some experience in programming to use this code: an IDE is required. The project was made in CodeBlocks, but any C++ should be able to build it.

The program runs on the command line. takes 2 arguments: first a CSV file to import from, then a MARC mnemonic file to export to. If the latter does not exist, it will be created. If it does exist, it will be overwritten. MARC mnemonic files can easily be converted to MARC proper with [MarcEdit](http://marcedit.reeset.net/).

The file "fieldmap.txt" needs to be modified to suit your own application. It maps the columns of the CSV file to (one or more) MARC fields and subfields.
Valid lines in the file contain 2 integers and a character, separated by spaces:
  * The first integer is the column number to be mapped, STARTING AT 0.
  * The second integer is the corresponding MARC field.
  * The character is the subfield in which the data must be recorded.
Everything else is ignored regarded as comment.

The program is neutral regarding text encoding, but for later use, it's safe to have your data encoded in UTF-8.
A good way to convert from one text encoding to another is to use [jEdit](http://www.jedit.org/), then at "buffer options" change to UTF-8.

## Known issues and incompletenesses
  * The validity checks are often embedded in the code, and tailored towards the B-Bc library. Some serious refactoring is needed to clean this up.
  * An upgrade to C++11 might be useful for better regex capabilities, string handling and initializers.
  * Certainly not all MARC fields are present in the code, only those used by the B-Bc library.
  * Lack of documentation
  * Much, much more :p
  
Have fun!
  

  
  
