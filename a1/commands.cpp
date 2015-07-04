// $Id: commands.cpp,v 1.11 2014-06-11 13:49:31-07 - - $

#include "commands.h"
#include "debug.h"

commands::commands(): map ({
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
}){}

command_fn commands::at (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   command_map::const_iterator result = map.find (cmd);
   if (result == map.end()) {
      throw yshell_exn (cmd + ": no such function");
   }
   return result->second;
}


void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() != 2)
      cout << "Usage: cat [filename]\n";
   else
      state.fun_cat(words.at(1));
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   
   if (words.size() > 2) 
      cout << "Usage: cd [directory]\n";
   else if (words.size() == 1)
      state.fun_cd();
   else
      state.fun_cd(words.at(1));
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   for (size_t i = 1; i < words.size(); ++i) {
      cout << words.at(i) << " ";
   }

   cout << endl;
}

void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit_exn();
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() > 2)
      cout << "Usage: ls or ls file/dir \n";
   else if (words.size() == 1)
      state.fun_ls();
   else
      state.fun_ls(words.at(1));
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() > 2)
      cout << "Usage: ls or ls file/dir \n";
   else if (words.size() == 1)
      state.fun_lsr();
   else
      state.fun_lsr(words.at(1));
}


void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() < 2)
      cout << "Usage: mkfile + contents\n";
   else
      state.fun_mkfile(words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 2)
      state.fun_mkdir(words.at(1));
   else
      cout << "Usage: mkdir [directory_name]\n";
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() != 2)
      cout << "Usage: prompt [string]\n";
   else
      state.set_prompt(words.at(1));
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() != 1)
      cout << "Usage: pwd\n";
   else
      state.fun_pwd();
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() != 2)
      cout << "Usage: rm [filename] or [empty dir]\n";
   else
      state.fun_rm(words.at(1));
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() != 2)
      cout << "Usage: rm [dir]\n";
   else
      state.fun_rm(words.at(1));
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

