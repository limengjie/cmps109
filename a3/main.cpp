// $Id: main.cpp,v 1.6 2014-07-09 11:50:34-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_pair = xpair<const string,string>;
using str_str_map = listmap<string,string>;

string trim (string &);

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);

   str_str_map test;
   str_str_map::iterator itor = test.begin();
   size_t count = 0;
   for (;;) {
      ++count;
      cout << "% ";
      string line;
      getline (cin, line);
      if (cin.eof()) break;
      cout << "-: " << count << ": " << line << endl;
      line = trim(line);
      if (line.size() == 0 or line[0] == '#') {
         cout << "Comment or blank line.\n";
         continue;
      }

      string key;
      string value;
      //split the line
      size_t opt;
      size_t pos = line.find_first_of ("=");
      if (pos == string::npos) { 
         //input doesn't include '='
         opt = 0;
         key = line;
         //cout << "key = " << key << endl;
         if ((itor = test.find(key)) != test.end())
            opt = 1;
      }
      else {
         key = trim (line.substr 
                     (0, pos == 0 ? 0 : pos));
         value = trim (line.substr (pos + 1));
         //cout << "key = " << key << endl;
         //cout << "value = " << value << endl;
         opt = 3;
         if (value.size() == 0) {
            opt = 2;
            if (key.size() == 0) opt = 4;
         }
         else if (key.size() == 0) 
            opt = 5;
      }
      str_str_pair pair (key, value);

      //cout << "option is " << opt << endl;

      switch (opt) {
         case 0: 
                 cout << "key: " << key << " not found\n";
                 break;
         case 1: 
                 cout << *itor << endl;
                 break;
         case 2: 
                 if ((itor = test.find(key)) != test.end())
                    test.erase(itor);
                 else 
                    cout << "key: " << key << " not found\n";
                 break;
         case 3: 
                 if ((itor = test.find(key)) == test.end())
                    test.insert(pair);
                 else {
                    test.erase(itor);
                    test.insert(pair);
                 }
                 cout << key << " = " << value << endl;
                 break;
         case 4: 
                 for (itor = test.begin(); 
                      itor != test.end(); ++itor)
                    cout << *itor << endl;
                 break;
         case 5: 
                 for (itor = test.begin(); 
                      itor != test.end(); ++itor) {
                    if (itor->value.second == value)
                       cout << *itor << endl;
                 }
                 break;
        default: 
                 cout << "No such case.\n";
      }
   }

/*
   for (int argi = 0; argi < argc; ++argi) {
      str_str_pair pair (argv[argi], to_string<int> (argi));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   itor = test.find("a");
   cout << *itor << " is found\n";
//   str_str_map::iterator itor = test.begin();
//   test.erase (itor);
*/

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

string trim (string & str) {
   size_t first = str.find_first_not_of (" \t");
   if (first == string::npos) return "";
   size_t last = str.find_last_not_of (" \t");
   return str.substr (first, last - first + 1);
}
