// $Id: main.cpp,v 1.3 2014-06-11 13:52:31-07 - - $

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <unistd.h>

using namespace std;

#include "commands.h"
#include "debug.h"
#include "inode.h"
#include "util.h"

void init_root_dir(inode_state &);
//
// scan_options
//    Options analysis:  The only option is -Dflags. 
//

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) option << ": invalid option"
                       << endl;
            break;
      }
   }
   if (optind < argc) {
      complain() << "operands not permitted" << endl;
   }
}

//
// main -
//    Main program which loops reading commands until end of file.
//

int main (int argc, char** argv) {
   execname (argv[0]);
   cout << boolalpha; // Print false or true instead of 0 or 1.
   cerr << boolalpha;
   cout << argv[0] << " build " << __DATE__ << " " << __TIME__ << endl;
   scan_options (argc, argv);
   bool need_echo = want_echo();
   commands cmdmap;
   string prompt = "%";
   inode_state state;
   init_root_dir(state);
   cout << state << endl;

   try {
      for (;;) {
         try {
   
            // Read a line, break at EOF, and echo print the prompt
            // if one is needed.
            prompt = state.get_prompt();
            cout << prompt << " ";
            string line;
            getline (cin, line);
            if (cin.eof()) {
               if (need_echo) cout << "^D";
               cout << endl;
               DEBUGF ('y', "EOF");
               break;
            }
            if (need_echo) cout << line << endl;
   
            // Split the line into words and lookup the appropriate
            // function.  Complain or call it.
            wordvec words = split (line, " \t");
            DEBUGF ('y', "words = " << words);
            command_fn fn = cmdmap.at(words.at(0));
            fn (state, words);
         }catch (yshell_exn& exn) {
            // If there is a problem discovered in any function, an
            // exn is thrown and printed here.
            complain() << exn.what() << endl;
         }
      }
   } catch (ysh_exit_exn& ) {
      // This catch intentionally left blank.
   }

   return exit_status_message();
}

void init_root_dir(inode_state & is) {
   inode_ptr proot = (inode_ptr) new inode(DIR_INODE);
   map<string, inode_ptr> m_root;
   m_root["."] = proot;
   m_root[".."] = proot;
   directory_ptr dir_root_ptr = (directory_ptr) new directory;
   dir_root_ptr->set_dir(m_root, "/");
   proot->set_cts((file_base_ptr) dir_root_ptr);
   is.set_cwd(proot);
   is.set_root(proot);
   is.add_q("/", proot);
}
