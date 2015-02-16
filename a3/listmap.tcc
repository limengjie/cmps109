// $Id: listmap.tcc,v 1.5 2014-07-09 11:50:34-07 - - $

#include "listmap.h"
#include "trace.h"
#include "xpair.h"

string trim (const string &);
//xpair<string, string> get_key_val (const string &);
//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
}

//
// listmap::empty()
//
template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::empty() const {
   return anchor_.next == anchor_.prev;
}

//
// listmap::iterator listmap::begin()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::begin() {
   return iterator (anchor_.next);
}

//
// listmap::iterator listmap::end()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::end() {
   return iterator (anchor());
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   
   iterator pos, it;
   node* prev_ptr;
   node* next_ptr;
   node* pnew_node;
   if (empty() and end() == begin() ) {
      prev_ptr = anchor();
      next_ptr = anchor();
      pnew_node = new listmap<Key,Value,Less>::
                  node(next_ptr, prev_ptr, pair);
      anchor_.prev = pnew_node;
      anchor_.next = pnew_node;
      pos = end();
   }
   else {
      pos = get_pos(pair);
      prev_ptr = pos->prev;
      next_ptr = (pos->prev)->next;
      pnew_node = new listmap<Key,Value,Less>::
                  node(next_ptr, prev_ptr, pair);
      prev_ptr->next = pnew_node;
      next_ptr->prev = pnew_node;

   }
//   cout << pnew_node->value << endl;
   
   return pos;
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) const {
   TRACE ('l', that);
   return iterator();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);
   return iterator();
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::node*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return where;
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

/*
template <typename Key, typename Value, class Less>
void listmap<Key, Value, Less>::set_anchor
     (const node & new_node) {
   if (this->empty())
      anchor_.next = &new_node;
   anchor_.prev = &new_node; 
}
*/

// find the position to insert a new node
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
         listmap<Key, Value, Less>::get_pos
         (const value_type& that) {
   Less less;
   iterator begin_ = begin();
   iterator end_ = end();
   
   if (begin_ == end_)
      return end_;

   while (less(that.first, begin_->value.first) and begin_ != end_) 
      ++begin_;
  
   return begin_;
}

string trim (const string & str) {
   size_t first = str.find_first_not_of (" \t");
   if (first == string::npos) return "";
   size_t last = str.find_last_not_of (" \t");

   return str.substr (first, last - first + 1);
}

/*
xpair<string, string> get_key_val (const string & line) {
   size_t pos = line.find_first_of ("=");
   string key, value;
   
   if (pos == string::npos) {
      cout << "key = \"" << line << "\" \n";
      cout << "No value in this line.\n";
      xpair<string, string> (key, "");
   }
   else {
      key = trim (line.substr 
                    (0, pos == 0 ? 0: pos-1));
      value = trim (line.substr (pos + 1));
      cout << "key = \"" << key << "\" \n";
      cout << "value = \"" << value << "\" \n";
      xpair<string, string> (key, value);
   }
}
*/
