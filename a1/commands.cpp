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

void ls(inode_ptr, directory_ptr);

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
   if (words.size() > 1) {
      string filename = words.at(1); 
      inode_ptr pinode = state.get_cwd();
      directory_ptr pdir = pinode->get_dir_ptr(); 
      map<string, inode_ptr> dir_map = pdir->get_dir_map();
      map<string, inode_ptr>::iterator it;
      if ((it = dir_map.find(filename)) == dir_map.end() ) {
         cout << "cat: " << filename;
         cout << ": No such file or directory.\n";
      }
      else {
         inode_ptr pch_inode = it->second;
         plain_file_ptr pfile = pch_inode->get_file_ptr();
	 for(size_t i = 0; i < pfile->readfile().size(); i++)
	    cout << pfile->readfile().at(i) << " ";
         cout << endl;
      }
   }		
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr pinode = state.get_cwd();
   directory_ptr pdir = pinode->get_dir_ptr(); 
   if (words.size() == 1) {
      inode_ptr proot = state.get_root();
      state.set_cwd(proot);
   }	
   else if (words.size() == 2) {
      string dirname = words.at(1);
      map<string, inode_ptr> dir_map = pdir->get_dir_map();
      map<string, inode_ptr>::iterator it;
      it = dir_map.find(dirname);
      if (it != dir_map.end() && 
         ((it->second)->get_type()) == DIR_INODE) {
	    state.set_cwd(it->second);
      }
      else { 
         cout << "cd: " << dirname;
         cout << ": No such directory.\n";
      }
   }
   else
      cout << "This command needs 2 arguments.\n";
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   for(size_t i = 1; i < words.size(); i++)
      cout << words.at(i) << " ";
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
   inode_ptr pinode = state.get_cwd();
   ls(pinode);
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr pinode;
   string dirname;
   if (words.size() == 1) {
	pinode = state.get_cwd();
   	directory_ptr pdir = pinode->get_dir_ptr();
	dirname = pdir->get_name();	
   }
   else if (words.size() == 2) 
	dirname = words.at(1);
   size_t start = getStartPtr(state, dirname);
   for(size_t i = start; i < state.get_qsize(); i++) {
      ls(state.get_qpair(i).second);	
   }
}

void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() > 1) {
      inode_ptr pinode = state.get_cwd();
      directory_ptr pdir = pinode->get_dir_ptr();
      inode_ptr pnew_inode = 
      pdir->mkfile(words.at(1));//new inode & file
      plain_file_ptr pnew_file = pnew_inode->get_file_ptr();
      string new_filename = pnew_file->get_name();
      map<string, inode_ptr> dir_map = pdir->get_dir_map();
      dir_map[new_filename] = pnew_inode;
      pdir->set_dir(dir_map, pdir->get_name());
      //update cwdirectory
      vector<string> content;
      for(size_t i = 2; i < words.size(); i++) {
         content.push_back(words.at(i));
      }
      pnew_file->writefile(content);
   }
}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if (words.size() == 2) {
      inode_ptr pinode = state.get_cwd();
      directory_ptr pdir = pinode->get_dir_ptr();
      inode_ptr pnew_inode = pdir->mkdir(words.at(1));
      directory_ptr pnew_dir = pnew_inode->get_dir_ptr();
      string new_dirname = pnew_dir->get_name();
      map<string, inode_ptr> dir_map = pdir->get_dir_map();
      dir_map[new_dirname] = pnew_inode;
      pdir->set_dir(dir_map, pdir->get_name()); //update cwdirectory
      state.add_q(new_dirname, pdir->get_name(), pnew_inode);
      map<string, inode_ptr> dir_map_new = pnew_dir->get_dir_map();
      dir_map_new[".."] = pinode;
      pnew_dir->set_dir(dir_map_new, new_dirname);
   }
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
   inode_ptr pinode = state.get_cwd();
   directory_ptr pdir = pinode->get_dir_ptr();
   map<string, inode_ptr> dir_map = pdir->get_dir_map();
   string target = words.at(1);
   map<string, inode_ptr>::iterator it;
   it = dir_map.find(target);
   if (it != dir_map.end() ) {
      dir_map.erase(it);
      pdir->set_dir(dir_map, pdir->get_name()); 
      //update cwdirectory
      state.erase_q(target);
   }
   else
      cout << "No such file " << target << endl;
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr pinode = state.get_cwd();
   directory_ptr pdir = pinode->get_dir_ptr();
   map<string, inode_ptr> dir_map = pdir->get_dir_map();
   string target = words.at(1);
   map<string, inode_ptr>::iterator it;
   it = dir_map.find(target);
   if (it != dir_map.end() ) {
      dir_map.erase(it);
      pdir->set_dir(dir_map, pdir->get_name()); 
      //update cwdirectory
      state.erase_q(target);
   }
   else
      cout << "No such file " << target << endl;
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void ls(inode_ptr pinode) {
   directory_ptr pdir = pinode->get_dir_ptr(); 
   map<string, inode_ptr> dir_map = pdir->get_dir_map();
   cout << pdir->get_name() << ":\n";
   for(map<string, inode_ptr>::iterator iter = dir_map.begin(); 
      iter != dir_map.end(); ++iter) {
      inode_ptr p_inode = (iter->second);
      cout << "\t" << (p_inode->get_inode_nr()) << "\t";
      inode_t type = (p_inode->get_type());
      if (type == DIR_INODE) {
      directory_ptr dir_ptr = p_inode->get_dir_ptr();
      size_t dir_size = dir_ptr->size();
      cout << dir_size;
      }
      else if (type == PLAIN_INODE) {
         plain_file_ptr file_ptr = (p_inode->get_file_ptr());
	 size_t file_size = (file_ptr->size());
	 cout << file_size;
      }
      cout << " " << (iter->first) << endl; 
   } //endfor
}

size_t getStartPtr(inode_state & state, string dirname) {
   size_t pos;
   for(pos = 0; pos < state.get_qsize(); pos++) 
      if (state.get_qpair(pos).first == dirname)
         break;
   return pos;
}
