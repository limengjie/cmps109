// $Id: commands.cpp,v 1.11 2014-06-11 13:49:31-07 - - $

#include "commands.h"
#include "debug.h"
//#include "inode.h"
//#include "util.h"

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
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   for(size_t i = 1; i < words.size(); i++)
	std::cout << words.at(i) << " ";
   std::cout << std::endl;
   
}

void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit_exn();
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr pinode = state.get_cwd();
   directory_ptr pdir = pinode->get_dir_ptr(); 
   cout << pdir->get_name() << ":\n";
   map<string, inode_ptr> dir_map = pdir->get_dir_map();
   for(map<string, inode_ptr>::iterator iter = dir_map.begin(); iter != dir_map.end(); ++iter)
	cout << "\t" << (iter->second)->get_inode_nr() << "\t" << dir_map.size() << " " << iter->first << endl; 
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() > 1) {
   	inode_ptr pinode = state.get_cwd();
   	directory_ptr pdir = pinode->get_dir_ptr();
   	inode_ptr pnew_inode = pdir->mkfile(words.at(1));//new inode & file
   	plain_file_ptr pnew_file = pnew_inode->get_file_ptr();
   	string new_filename = pnew_file->get_name();
   	map<string, inode_ptr> dir_map = pdir->get_dir_map();
   	dir_map[new_filename] = pnew_inode;
   	pdir->set_dir(dir_map, pdir->get_name()); //update cwdirectory
   	vector<string> content;
   	for(size_t i = 2; i < words.size(); i++) {
		cout << "write " << words.at(i) << endl;
		content.push_back(words.at(i));
	}
   	pnew_file->writefile(content);
   }
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr pinode = state.get_cwd();
   pinode->inc_inode_nr();
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   state.set_prompt(words.at(1));
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr pinode = state.get_cwd();
   directory_ptr pdir = pinode->get_dir_ptr();
   cout << pdir->get_name() << endl;
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

