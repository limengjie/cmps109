// $Id: bigint.h,v 1.16 2014-07-02 20:01:17-07 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <exception>
#include <iostream>
#include <utility>
using namespace std;

#include "debug.h"

using digit_t = unsigned char;
using bigvalue_t = vector<digit_t>;
using quot_rem = pair<bigvalue_t, bigvalue_t>;
//
// Define class bigint
//
class bigint {
      friend ostream& operator<< (ostream&, const bigint&);
   private:
      long long_value {}; //delete
      bool negative;
      bigvalue_t big_value;
 //     using quot_rem = pair<bigint,bigint>;
      using unumber = unsigned long;
  //    friend quot_rem divide (const bigint&, const bigint&);
      friend bigvalue_t multiply_by_2 (bigvalue_t&);
      friend bigvalue_t divide_by_2 (bigvalue_t&);
   public:

      //
      // Ensure synthesized members are genrated.
      //
      bigint() = default;
      bigint (const bigint&) = default;
      bigint (bigint&&) = default;
      bigint& operator= (const bigint&) = default;
      bigint& operator= (bigint&&) = default;
      ~bigint() = default;
      //friend void show(const bigint &);

      //
      // Extra ctors to make bigints.
      //
      bigint (const long);
      bigint (const string&);

      //
      // Basic add/sub operators.
      //
      friend bigint operator+ (const bigint&, const bigint&);//modified
      friend bigint operator- (const bigint&, const bigint&);
      friend bigint operator+ (const bigint&);
      friend bigint operator- (const bigint&);
      long to_long() const;

      //
      // Extended operators implemented with add/sub.
      //
      friend bigint operator* (const bigint&, const bigint&);
      friend bigint operator/ (const bigint&, const bigint&);
      friend bigint operator% (const bigint&, const bigint&);

      //
      // Comparison operators.
      //
      friend bool operator== (const bigint&, const bigint&);
      friend bool operator<  (const bigint&, const bigint&);
     
      friend bigvalue_t do_bigadd (const bigvalue_t&, const bigvalue_t&);
      friend bigvalue_t do_bigsub (const bigvalue_t&, const bigvalue_t&);
      friend bigvalue_t do_bigmul (const bigvalue_t&, const bigvalue_t&);
      friend quot_rem   do_bigdiv (const bigvalue_t&, const bigvalue_t&);
      bigvalue_t get_big_value() const;
      void set_big_value(const bigvalue_t &);
      bool get_negative() const;
      //bool get_neg_reverse(const bool &);
      void set_negative(const bool &);
};

//
// The rest of the operators do not need to be friends.
// Make the comparisons inline for efficiency.
//

/*
bigint pow (const bigint& base, const bigint& exponent);

inline bool operator!= (const bigint &left, const bigint &right) {
   return not (left == right);
}
inline bool operator>  (const bigint &left, const bigint &right) {
   return right < left;
}
inline bool operator<= (const bigint &left, const bigint &right) {
   return not (right < left);
}
inline bool operator>= (const bigint &left, const bigint &right) {
   return not (left < right);
}
*/

#endif

