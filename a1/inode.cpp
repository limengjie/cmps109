// $Id: inode.cpp,v 1.12 2014-07-03 13:29:57-07 - - $

#include <iostream>
#include <stdexcept>
#include <map>

using namespace std;

#include "debug.h"
#include "inode.h"

int inode::next_inode_nr {1};

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case PLAIN_INODE:
           contents = make_shared<plain_file>();
	   cout << "create a file\n";
           break;
      case DIR_INODE:
           contents = make_shared<directory>();
           cout << "create a dir\n";
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
   
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

/*
string inode::get_dirname() {
   directory_ptr pdir = directory_ptr_of(contents);
   string dirname = pdir->get_name();
   return dirname;
}
*/

inode_t inode::get_type() const {
   return type;
}

void inode::inc_inode_nr() {
   ++inode_nr;
}

directory_ptr inode::get_dir_ptr() const {
   directory_ptr pdir = directory_ptr_of(contents);
   return pdir;
}

plain_file_ptr inode::get_file_ptr() const {
   plain_file_ptr pfile = plain_file_ptr_of(contents);
   return pfile;
}

file_base_ptr inode::get_cts() const {
   return contents;
}

void inode::set_cts(const file_base_ptr new_contents) {
   contents = new_contents;
}


void inode::set_cwd(inode_state & is) {
   is.cwd = (inode_ptr)this;
   cout << "call set cwd\n";
}

void inode::set_root(inode_state & is) {
   is.root = (inode_ptr)this;
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
   size = data.size();
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   for(size_t i = 0; i < words.size(); i++) {
	data.push_back(words.at(i));
//	cout << "write to data "<< words.at(i) << endl;
   }
}

void plain_file::set_filename(const string & filename) {
   name = filename;
}

string plain_file::get_name() const {
   return name;
}

void directory::set_dir(const map<string, inode_ptr> root_map, const string & dirname) {
   dirents = root_map;
   name = dirname;
}

void directory::set_dir(const string & dirname) {
   name = dirname;
}

string directory::get_name() const {
   return name;
}

map<string, inode_ptr> directory::get_dir_map() const {
   return dirents;
}

size_t directory::size() const {
   size_t size {0};
   size = dirents.size();
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string & dirname) {
   inode_ptr pinode = (inode_ptr) new inode(DIR_INODE);
   directory_ptr pdir = (directory_ptr) new directory;
   pinode->set_cts(pdir);
   map<string, inode_ptr> dir_map;
   dir_map["."] = pinode;
   pdir->set_dir(dir_map, dirname);

   return pinode;
}

inode_ptr  directory::mkfile (const string & filename) {
   plain_file_ptr pfile = (plain_file_ptr) new plain_file;
   pfile->set_filename(filename);
   inode_ptr pinode = (inode_ptr) new inode(PLAIN_INODE);
   pinode->set_cts(pfile); 
   //cout << "inode_nr= " << pinode->get_inode_nr() << endl;
   
   return pinode;
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt << "\"");
   root = nullptr;
   cwd = nullptr;
   prompt = "%"; 
}

void inode_state::set_prompt(const string& p) {
   prompt = p;
}

string inode_state::get_prompt() const {
   return prompt;
}

inode_ptr inode_state::get_cwd() const {
   return cwd ;
}

inode_ptr inode_state::get_root() const {
   return root;
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd << ", prompt = " 
       << state.prompt;
   return out;
}
