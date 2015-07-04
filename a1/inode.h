// $Id: inode.h,v 1.13 2014-06-12 18:10:25-07 - - $

#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
using namespace std;

#include "util.h"

//
// inode_t -
//    An inode is either a directory or a plain file.
//

enum inode_t {PLAIN_INODE, DIR_INODE};
class inode;
class file_base;
class plain_file;
class directory;
using inode_ptr = shared_ptr<inode>;
using file_base_ptr = shared_ptr<file_base>;
using plain_file_ptr = shared_ptr<plain_file>;
using directory_ptr = shared_ptr<directory>;

//
// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.
//

class inode_state {
   friend class inode;
   friend ostream& operator<< (ostream& out, const inode_state&);
   private:
      inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
      inode_ptr root {nullptr};
      inode_ptr cwd {nullptr};
      string prompt {"% "};
   public:
      inode_state();
      // new methods 
      string get_prompt() { return prompt;};
      void set_prompt(string p) { prompt = p;};
      inode_ptr get_root() { return root;};
      void set_root(inode_ptr r) { root = r;};
      inode_ptr get_cwd() { return cwd;};
      void set_cwd(inode_ptr c) { cwd = c;};
      directory_ptr get_dir_ptr();
      directory_ptr get_dir_ptr(inode_ptr ind_ptr);
      plain_file_ptr get_file_ptr(inode_ptr ind_ptr);
      wordvec get_path(const string & path, const string & delimiters);
      inode_ptr get_target_ind(const wordvec & path, 
            const inode_ptr & start);
      inode_ptr get_target_par(const wordvec & path, 
            const inode_ptr & start);
      // command functs
      void fun_cat(const string & filename);
      void fun_cd() {cwd = root;};
      void fun_cd(const string & dirname);
      void fun_ls();
      void fun_ls(const string & filename);
      void fun_lsr();
      void fun_lsr(const string & filename);
      void fun_lsr(inode_ptr ind_ptr);
      void fun_mkdir(const string & dirname);
      void fun_mkfile(const wordvec &);
      void fun_pwd();
      void fun_rm(const string & filename);
};


//
// class inode -
//
// inode ctor -
//    Create a new inode of the given type.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
//    

class inode {
   friend class inode_state;
   private:
      static int next_inode_nr;
      int inode_nr;
      inode_t type;
      file_base_ptr contents;
   public:
      inode (inode_t init_type);
      int get_inode_nr() const;
      // new methods
      file_base_ptr get_contents() {return contents;};
      void set_contents(const file_base_ptr & con)
           {contents = con;}
      inode_t get_type() {return type;};
      directory_ptr inode2dir();
      plain_file_ptr inode2file();
      inode_ptr search_map(const string & name, inode_t type);
      inode_ptr search_map(const string & name);
      void printfile();
      void print_info();
      void print_file_info();
      void print_dir_info();
      inode_ptr get_par_ind();
      bool isDir() {return type == DIR_INODE;};
};

//
// class file_base -
//
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.
//

class file_base {
   protected:
      file_base () = default;
      file_base (const file_base&) = default;
      file_base (file_base&&) = default;
      file_base& operator= (const file_base&) = default;
      file_base& operator= (file_base&&) = default;
      virtual ~file_base () = default;
      virtual size_t size() const = 0;
   public:
      friend plain_file_ptr plain_file_ptr_of (file_base_ptr);
      friend directory_ptr directory_ptr_of (file_base_ptr);
};


//
// class plain_file -
//
// Used to hold data.
// synthesized default ctor -
//    Default vector<string> is a an empty vector.
//home/parallels/wm109/phase10
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
//    Throws an yshell_exn for a directory.
// writefile -
//    Replaces the contents of a file with new contents.
//    Throws an yshell_exn for a directory.
//

class plain_file: public file_base {
   private:
      wordvec data;
      string name;
   public:
      size_t size() const override;
      const wordvec& readfile() const;
      void writefile (const wordvec& newdata, int start);
      //new
      void set_filename(const string & fn) { name = fn;}
      void print_data();
      string get_name() { return name;};
      //void print_file_info();
};

//
// class directory -
//
// Used to map filenames onto inode pointers.
// default ctor -
//    Creates a new map with keys "." and "..".
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an yshell_exn if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.

class directory: public file_base {
   private:
      map<string,inode_ptr> dirents;
      string name;
   public:
      size_t size() const override;
      void remove (const string& filename);
      inode_ptr mkdir (const string& dirname);
      inode_ptr mkfile (const string& filename);
      //new
      string get_dirname() { return name; };
      void set_dir(const map<string, inode_ptr> & dirmap, 
              const string & dirname) { 
         dirents = dirmap; 
         name = dirname; 
      } 
      void set_dir(const map<string, inode_ptr> & dirmap) { 
         //only update the map
         dirents = dirmap; 
      } 
      map<string, inode_ptr> get_dir_map() { return dirents; };
      inode_ptr search_dirents(const string & name);
      inode_ptr search_dirents(const string & name, inode_t type);
      //void print_dir_info();
};

#endif

