// $Id: inode.cpp,v 1.12 2014-07-03 13:29:57-07 - - $

#include <iostream>
#include <stdexcept>

using namespace std;

#include "debug.h"
#include "inode.h"

//pi = inode_ptr


int inode::next_inode_nr {1};

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case PLAIN_INODE:
           contents = make_shared<plain_file>();
           break;
      case DIR_INODE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

directory_ptr inode::inode2dir() {
   // file base pointer -> directory pointer
   directory_ptr dirptr = directory_ptr_of(contents);

   return dirptr;
}

plain_file_ptr inode::inode2file() {
   // file base pointer -> file pointer
   plain_file_ptr pfptr = plain_file_ptr_of(contents);

   return pfptr;
}

inode_ptr inode::search_map(const string & filename) {
   //get info about current working dirctory
   // file base pointer -> directory pointer
   directory_ptr cw_dir_ptr = directory_ptr_of(contents);

   inode_ptr ind_ptr = cw_dir_ptr->search_dirents(filename);

   return ind_ptr;
}

inode_ptr inode::search_map(const string & filename, 
      inode_t type) {
   //get info about current working dirctory
   // file base pointer -> directory pointer
   directory_ptr cw_dir_ptr = directory_ptr_of(contents);
   //find the inode from the map
   inode_ptr ind_ptr = cw_dir_ptr->search_dirents(filename, type);

   return ind_ptr;
}
   

void inode::printfile() {
   //get file pointer
   if (type == PLAIN_INODE) {
      plain_file_ptr file_ptr = plain_file_ptr_of(contents);
      //print out the file
      file_ptr->print_data();
   }
   else
      cout << "This is not a plain file.\n";
}

void inode::print_info() {
   if (type == PLAIN_INODE) 
      print_file_info();
   else
      print_dir_info();
      
}

void inode::print_file_info() {
   cout << "inode #\tsize\tname\n";
   //print inode number
   cout << "  " << inode_nr << "\t ";
   //print size
   plain_file_ptr file_ptr = plain_file_ptr_of(contents);
   size_t file_size = file_ptr->size();
   cout << file_size;
   //print name
   cout << "\t  " << file_ptr->get_name() << endl;

}

void inode::print_dir_info() {
   //get info about current working dirctory
   directory_ptr cw_dir_ptr = directory_ptr_of(contents);

   // print out the current working directory name
   cout << cw_dir_ptr->get_dirname() << ":\n";

   //get the map info
   map<string, inode_ptr> dir_map = cw_dir_ptr->get_dir_map();
   //list everything in this directory
   map<string, inode_ptr>::iterator iter;
   for (iter = dir_map.begin(); iter != dir_map.end(); ++iter) {
      //do not count "\"
      string name;
      if ((name = (iter->first)) == "\\")
         continue;

      //get inode 
      inode_ptr ind_ptr = iter->second;
      //get inode number
      cout << "\t" << ind_ptr->get_inode_nr() << "\t";
      // get inode type
      inode_t ind_type = ind_ptr->get_type();
      if (ind_type == DIR_INODE) { //print out directory
         //directory_ptr dir_ptr = get_dir_ptr(ind_ptr);
         directory_ptr dir_ptr = 
            directory_ptr_of(ind_ptr->get_contents());
         size_t dir_size = dir_ptr->size();
         cout << dir_size;
      }
      else if (ind_type == PLAIN_INODE) { // print out plain file
         //plain_file_ptr file_ptr = get_file_ptr(ind_ptr);
         plain_file_ptr file_ptr = 
            plain_file_ptr_of(ind_ptr->get_contents());
         size_t file_size = file_ptr->size();
         cout << file_size;
      }
      cout << "\t" << (iter->first) << endl;
   }
}

plain_file_ptr plain_file_ptr_of (file_base_ptr ptr) {
   plain_file_ptr pfptr = dynamic_pointer_cast<plain_file> (ptr);
   if (pfptr == nullptr) throw invalid_argument ("plain_file_ptr_of");
   return pfptr;
}

directory_ptr directory_ptr_of (file_base_ptr ptr) {
   directory_ptr dirptr = dynamic_pointer_cast<directory> (ptr);
   if (dirptr == nullptr) throw invalid_argument ("directory_ptr_of");
   return dirptr;
}


size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);

   for (size_t i = 0; i < data.size(); ++i)
      size += data.at(i).length();

   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words, int start) {
   //DEBUGF ('i', words);
   for(size_t i = start; i < words.size(); ++i)
      data.push_back(words.at(i));
}


void plain_file::print_data () {
   for (size_t i = 0; i < data.size(); ++i) {
      cout << data.at(i) << " ";
   }
   cout << endl;
}

size_t directory::size() const {
   size_t size = dirents.size();
   DEBUGF ('i', "size = " << size);

   if (name == "\\") 
      return size - 1;

   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);

   map<string, inode_ptr>::iterator iter;
   iter = dirents.find(filename);
   if (iter != dirents.end())
      dirents.erase(iter);
   else
      cout << "cannot rm, no such file" << endl;
}

inode_ptr directory::mkdir (const string & dirname) {
   //create a inode pointer
   inode_ptr dir_inode_ptr(new inode(DIR_INODE));

   // create a directory pointer
   directory_ptr dir_ptr(new directory);

   // make inode point to new directory
   dir_inode_ptr->set_contents(dir_ptr);

   // set directory
   map<string, inode_ptr> dirmap;
   dirmap["."] = dir_inode_ptr;
   dir_ptr->set_dir(dirmap, dirname);

   return dir_inode_ptr;
}

inode_ptr directory::mkfile (const string & filename) {
   //create an empty plain file

   //create a plain file pointer
   plain_file_ptr plain_file_ptr(new plain_file);
   
   //set the file's name
   plain_file_ptr->set_filename(filename);

   // create a inode pointer
   inode_ptr pfile_inode_ptr(new inode(PLAIN_INODE));

   //link the file pointer to the inode
   pfile_inode_ptr->set_contents(plain_file_ptr);

   return pfile_inode_ptr;
}

inode_ptr directory::search_dirents(const string & filename, 
      inode_t type) {
   //search the map
   map<string, inode_ptr>::iterator iter;
   iter = dirents.find(filename);
   if (iter != dirents.end()) {
      inode_ptr ind_ptr = iter->second;
      if (ind_ptr->get_type() == type) {
         // double check if it is dir or not
         return ind_ptr;
      }
   }
   return nullptr;
}

inode_ptr directory::search_dirents(const string & filename) {
   //search the map
   map<string, inode_ptr>::iterator iter;
   iter = dirents.find(filename);
   if (iter != dirents.end()) {
      inode_ptr ind_ptr = iter->second;
      return ind_ptr;
   }

   return nullptr;
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt << "\"");
   //create root directory
   
   //create root inode
   inode_ptr root_inode_ptr(new inode(DIR_INODE)); 
   set_root(root_inode_ptr);
   set_cwd(root_inode_ptr);

   //create root directory
   map<string, inode_ptr> rootmap;
   rootmap["."] = root_inode_ptr;
   rootmap[".."] = root_inode_ptr;
   rootmap["\\"] = root_inode_ptr;
   directory_ptr dir_ptr(new directory);
   dir_ptr->set_dir(rootmap, "\\");

   // connect directory with inode root
   root_inode_ptr->set_contents(dir_ptr);

}

directory_ptr inode_state::get_dir_ptr() {
   return cwd->inode2dir();
}

directory_ptr inode_state::get_dir_ptr(inode_ptr ind_ptr) {
   return ind_ptr->inode2dir();
}

plain_file_ptr inode_state::get_file_ptr(inode_ptr ind_ptr) {
   return ind_ptr->inode2file();
}

wordvec inode_state::get_path(const string & line, 
      const string & delimiters) {
   wordvec path;

   size_t end = 0;
   for (;;) {
      size_t start = line.find_first_not_of(delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      path.push_back (line.substr (start, end - start));
   }


   return path;
}

inode_ptr inode_state::get_target_ind(const wordvec & path, 
      const inode_ptr & start) {
   //given a path,
   //recursively find the target inode from the start dir
   inode_ptr ind_ptr = start;

   ////print out the result///////////////////////////////////
   //for (size_t i = 0; i < path.size(); ++i)
      //cout << i << ": " << path.at(i) << "\t";
   //cout << endl;


   //find the target inode 
   for (size_t i = 0; i < path.size(); ++i) {
      ind_ptr = ind_ptr->search_map(path.at(i));

      // if no such directory, return nullptr
      if (ind_ptr == nullptr)
         return nullptr;
   }

   return ind_ptr;
}

inode_ptr inode_state::get_target_par(const wordvec & path, 
      const inode_ptr & start) {
   //given a path,
   //recursively find the target inode from the start dir
   inode_ptr ind_ptr = start;

   //find the target inode 
   inode_ptr par_ind_ptr;
   for (size_t i = 0; i < path.size(); ++i) {
      ind_ptr = ind_ptr->search_map(path.at(i));

      //get parent inode pointer and return it later
      if (i == (path.size() - 2))
         par_ind_ptr = ind_ptr;
   }

   // if no such directory, return nullptr
   if (ind_ptr == nullptr)
      return nullptr;

   if (path.size() == 1)
      par_ind_ptr = start;

   return par_ind_ptr;
}

void inode_state::fun_cat(const string& filename) {
   //search the file
   inode_ptr file_ind_ptr = cwd->search_map(filename, PLAIN_INODE);

   if (file_ind_ptr == nullptr) // if the file doesn't exist
      cout << "No such file or directory.\n";
   else {
      //print out the file
      file_ind_ptr->printfile();
   }

}

void inode_state::fun_cd(const string& target) {
   //find the target directory and modify the cwd

   //parse the string and get the path name
   wordvec path = get_path(target, "/");

   // if path name == "/" or "//" ......
   // set the root dir as the cwd
   if (path.size() == 0) {
      cout << "path size == 0\n";
      cwd = root;
      return;
   }

   //find the inode pointer of the target directory
   inode_ptr new_cwd_ptr;
   if (target.at(0) == '/') // from root directory
      new_cwd_ptr = get_target_ind(path, root);
   else // from current working dir
      new_cwd_ptr = get_target_ind(path, cwd);

   // updata the current working directory
   if (new_cwd_ptr == nullptr)
      cout << "No such file or directory.\n";
   else
      cwd = new_cwd_ptr;
}

void inode_state::fun_ls(const string & target) {
   //parse the string and get the path name
   wordvec path = get_path(target, "/");

   //find the inode pointer of the target 
   inode_ptr new_cwd_ptr;
   if (target.at(0) == '/') // from root directory
      new_cwd_ptr = get_target_ind(path, root);
   else // from current working dir
      new_cwd_ptr = get_target_ind(path, cwd);

   if (new_cwd_ptr)
      new_cwd_ptr->print_info();
}

void inode_state::fun_ls() {
   cwd->print_info();
}

void inode_state::fun_lsr() {
   cwd->print_info();

   // print the rest of dirs recursively
   // get map
   directory_ptr dir_ptr = cwd->inode2dir();
   map<string, inode_ptr> dir_map = dir_ptr->get_dir_map();
   //print the rest of dirs
   map<string, inode_ptr>::iterator iter;
   for (iter = dir_map.begin(); iter != dir_map.end(); ++iter) {
      //sub_ind_ptr is the inode ptr in the map
      inode_ptr sub_ind_ptr = iter->second;
      if (sub_ind_ptr->isDir() and iter->first != "." 
            and iter->first != ".." and iter->first != "\\") {
         fun_lsr(sub_ind_ptr);
         //cout << "ind nr: " << sub_ind_ptr->get_inode_nr();
         //cout << " name: " << iter->first << endl;
      }
   }
}

void inode_state::fun_lsr(const string & target) {
   //parse the string and get the path name
   wordvec path = get_path(target, "/");

   //find the inode pointer of the target 
   inode_ptr ind_ptr;
   if (target.at(0) == '/') // from root directory
      ind_ptr = get_target_ind(path, root);
   else // from current working dir
      ind_ptr = get_target_ind(path, cwd);

   // print the first dir
   if (ind_ptr)
      ind_ptr->print_info();

   // print the rest of dirs recursively
   // get map
   directory_ptr dir_ptr = ind_ptr->inode2dir();
   map<string, inode_ptr> dir_map = dir_ptr->get_dir_map();
   map<string, inode_ptr>::iterator iter;
   for (iter = dir_map.begin(); iter != dir_map.end(); ++iter) {
      //sub_ind_ptr is the inode ptr in the map
      inode_ptr sub_ind_ptr = iter->second;
      if (sub_ind_ptr->isDir() and iter->first != "." 
            and iter->first != ".." and iter->first != "\\") 
         fun_lsr(sub_ind_ptr);
   }

}

void inode_state::fun_lsr(inode_ptr ind_ptr) {
   ind_ptr->print_info();

   // print the rest of dirs recursively
   // get map
   directory_ptr dir_ptr = ind_ptr->inode2dir();
   map<string, inode_ptr> dir_map = dir_ptr->get_dir_map();
   map<string, inode_ptr>::iterator iter;
   for (iter = dir_map.begin(); iter != dir_map.end(); ++iter) {
      //sub_ind_ptr is the inode ptr in the map
      inode_ptr sub_ind_ptr = iter->second;
      if (sub_ind_ptr->isDir() and iter->first != "." 
            and iter->first != ".." and iter->first != "/") 
         fun_lsr(sub_ind_ptr);
   }
}

void inode_state::fun_mkdir(const string & new_dirname) {
   //get info about current working dirctory
   directory_ptr cw_dir_ptr = get_dir_ptr();

   //create new directory
   inode_ptr new_ind_ptr = cw_dir_ptr->mkdir(new_dirname);
   directory_ptr new_dir_ptr = get_dir_ptr(new_ind_ptr);

   //update cwd map info
   map<string, inode_ptr> cw_dir_map = cw_dir_ptr->get_dir_map();
   // check if the base file exists
   if (cwd->search_map(new_dirname)) {
      cout << "file name exists.\n";
      return;
   }
   cw_dir_map[new_dirname] = new_ind_ptr;
   cw_dir_ptr->set_dir(cw_dir_map);
   
   //update new working directory map info
   map<string, inode_ptr> new_dir_map = new_dir_ptr->get_dir_map();
   new_dir_map[".."] = cwd;
   new_dir_ptr->set_dir(new_dir_map, new_dirname);
}

void inode_state::fun_mkfile(const wordvec & words) {
   //get file name
   string filename = words.at(1);

   //get current working dirctory pointer
   directory_ptr cw_dir_ptr = get_dir_ptr();

   //create an empty file inode pointer
   inode_ptr newfile_ind_ptr = cw_dir_ptr->mkfile(filename);
   //get the empty file pointer
   plain_file_ptr new_file_ptr = get_file_ptr(newfile_ind_ptr);

   //update cwd map info
   map<string, inode_ptr> cw_dir_map = cw_dir_ptr->get_dir_map();
   // check if the base file exists
   if (cwd->search_map(filename)) {
      cout << "file name exists.\n";
      return;
   }
   cw_dir_map[filename] = newfile_ind_ptr;
   cw_dir_ptr->set_dir(cw_dir_map);

   //write data to plain file
   new_file_ptr->writefile(words, 2);
}

void inode_state::fun_pwd() {
   //get info about current working dirctory
   directory_ptr cw_dir_ptr = get_dir_ptr();

   // print out the directory name
   cout << cw_dir_ptr->get_dirname() << endl;
}

void inode_state::fun_rm(const string & pathname) {
   //parse the string and get the path name
   wordvec path = get_path(pathname, "/");
   string target = path.back();

   //find the inode pointer of the target directory
   inode_ptr par_ind_ptr;
   if (pathname.at(0) == '/') // from root directory
      par_ind_ptr = get_target_par(path, root);
   else // from current working dir
      par_ind_ptr = get_target_par(path, cwd);

   // updata the current working directory
   if (par_ind_ptr == nullptr)
      cout << " No such file or directory.\n";
   else {
      directory_ptr par_dir = get_dir_ptr(par_ind_ptr);
      par_dir->remove(target);
   }

}

ostream& operator<< (ostream& out, const inode_state& state) {
   // get root name
   string rootname;

   // inode pointer -> directory pointer -> dir name
   rootname = state.root->inode2dir()->get_dirname();


   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd << ", dirname = " 
       << rootname << endl;
   return out;
}

