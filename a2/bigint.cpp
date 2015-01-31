// $Id: bigint.cpp,v 1.61 2014-06-26 17:06:06-07 - - $

#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"

bigvalue_t get_reverse(const bigvalue_t &);
int abs_cmp (const bigvalue_t &, const bigvalue_t &); 
bool get_neg_reverse(const bool &);
pair<int, int> do_div (const int &, const int &); //temporary

bigint::bigint (long that): long_value (that) {
   DEBUGF ('~', this << " -> " << long_value)
}

bigint::bigint (const string& that) {
   auto itor = that.cbegin();
   bool isnegative = false;
   if (itor != that.cend() and *itor == '_') {
      isnegative = true;
      ++itor;
   }
//   int newval = 0;
//   while (itor != that.end()) newval = newval * 10 + *itor++ - '0';
//   long_value = isnegative ? - newval : + newval;
   negative = isnegative;
   while (itor != that.end()) {
      big_value.push_back(*itor++); 
   }
   big_value = get_reverse(big_value);
   DEBUGF ('~', this << " -> " << long_value)
}

bigvalue_t do_bigadd(const bigvalue_t & big, const bigvalue_t & small) {
   int carry = 0;
   bigvalue_t res;

   for(size_t i = 0; i < big.size(); ++i) {
      int b, s, sum;

      b = (int) big.at(i) - '0';
      if (i >= small.size() ) {
         sum = b + carry;
      }
      else {
         s = (int) small.at(i) - '0';
         sum = b + s + carry;
      }
          
      if (sum > 9) {
        sum -= 10;
        carry = 1;
      }
      else
         carry = 0; 
      res.push_back('0' + sum);
   }
   if (carry > 0)
      res.push_back('1');
   
   return res;
} 

bigvalue_t do_bigsub(const bigvalue_t & big, const bigvalue_t & small) {
   int carry = 0;
   bigvalue_t res;
   
   for(size_t i = 0; i < big.size(); ++i) {
      int b, s, dif;

      b = (int) big.at(i) - '0';
      if (i >= small.size() ) 
         dif = b - carry;
      else {
         s = (int) small.at(i) - '0';
         dif = b -s - carry;
      }
          
      if (dif < 0) {
        dif += 10;
        carry = 1;
      }
      else
         carry = 0; 
      res.push_back('0' + dif);
   }

   int cur = res.size();

   while (res.at(cur-1) == '0') {
      //cout << "The msb is " << res.at(cur-1) << endl; //delete
      res.pop_back();
      --cur;
   } 
//   cout << "The sz is " << res.size() << endl; //delete

   return res;
}

bigvalue_t do_bigmul(const bigvalue_t & left, const bigvalue_t & right) {
   int carry = 0;
   bigvalue_t res;
   
 /*  cout << "left: \n";
   for(size_t i = 0; i < left.size(); ++i)//delete
       cout << left.at(i) << " \t";
   cout << "\n right:\n";
   for(size_t i = 0; i < right.size(); ++i)//delete
       cout << right.at(i) << " \t";
   cout << endl; */

   for(size_t i = 0; i < left.size(); ++i) {
       int l = (int)left.at(i) - '0';
       bigvalue_t temp; 
//       cout << "i = " << i << endl; //delete
       for(size_t j = 0; j < right.size(); ++j) {
         int r = (int)right.at(j) - '0'; 
         int product = r * l + carry; 
         pair<int, int> quot_rem = do_div(product, 10);
         product = quot_rem.second;
         carry = quot_rem.first;
//         cout << "quot, rem" << product << ", " << carry << endl; //delete
         char p = '0' + product; 
         temp.push_back(p);
       //  cout << " p= " << p << endl; //delete
       }
       if (carry != 0)
          temp.push_back('0' + carry);
       
       auto it = temp.begin();
       temp.insert(it, i, '0');
 /*      cout << "this is temp for loop " << i << endl; // delete
       cout << "new temp's sz is " << temp.size() << endl; //del
       for(size_t i = 0; i < temp.size(); ++i) //delete
           cout << temp.at(i) << "  ";
       cout << endl;
*/
       carry = 0;
       if (temp.size() >= res.size())
           res = do_bigadd(temp, res);
       else if (temp.size() < res.size()) {
           cout << "Just in case.\n";
           res = do_bigadd(res, temp);
       }
/*       cout << "this is result for loop " << i << endl; // delete
       for(size_t i = 0; i < res.size(); ++i) //delete
           cout << res.at(i) << "  ";
       cout << endl; */
   }
   
/*       cout << "this is the final result " << endl; // delete
       for(size_t i = 0; i < res.size(); ++i) //delete
           cout << res.at(i) << "  ";
       cout << endl; */
   return res;
}

bigint operator+ (const bigint& left, const bigint& right) {
   bigint res;
   bigvalue_t left_bv, right_bv;

   left_bv = left.get_big_value();
   right_bv = right.get_big_value();
   if (abs_cmp(left_bv, right_bv) >= 0) {
      res.set_negative(left.get_negative());
      if (left.get_negative() == right.get_negative()) 
         res.set_big_value(do_bigadd(left_bv, right_bv));
      else
         res.set_big_value(do_bigsub(left_bv, right_bv));
   }
   else { 
      res.set_negative(right.get_negative());
      if (right.get_negative() == left.get_negative())
         res.set_big_value(do_bigadd(right_bv, left_bv));
      else
         res.set_big_value(do_bigsub(right_bv, left_bv));
   } 

   return res; 
}

bigint operator- (const bigint& left, const bigint& right) {
   bigint res;
   bigvalue_t left_bv, right_bv;

   left_bv = left.get_big_value();
   right_bv = right.get_big_value();
   if (abs_cmp(left_bv, right_bv) >= 0) {
      res.set_negative(left.get_negative());
      if (left.get_negative() == right.get_negative()) 
         res.set_big_value(do_bigsub(left_bv, right_bv));
      else
         res.set_big_value(do_bigadd(left_bv, right_bv));
   }
   else { 
      if (right.get_negative() == left.get_negative()) {
         res.set_big_value(do_bigsub(right_bv, left_bv));
         res.set_negative(get_neg_reverse(left.get_negative()));
      }
      else {
         res.set_big_value(do_bigadd(right_bv, left_bv));
         res.set_negative(left.get_negative());
      }
   } 

   return res;
}

bigint operator+ (const bigint& right) {
   //right.negative = false;
   return right;
}

bigint operator- (const bigint& right) {
//   if (right.negative == true)
//      right.negative = false;
//   else
//      right.negative = true;
   return right;
}

long bigint::to_long() const {
   if (*this <= bigint (numeric_limits<long>::min())
    or *this > bigint (numeric_limits<long>::max()))
               throw range_error ("bigint__to_long: out of range");
   return long_value;
}

bool abs_less (const long& left, const long& right) {
   return left < right;
}

int abs_cmp (const bigvalue_t & left, const bigvalue_t & right) {
   int res = 0;

   if (left.size() > right.size())
      res = 1;
   else if (left.size() < right.size())
      res = -1;
   else {
      for(size_t i = left.size(); i != 0; --i)
         if (left.at(i-1) > right.at(i-1)) {
            res = 1;
            break;
         }
         else if (left.at(i-1) < right.at(i-1)) {
            res = -1;
            break;
         }
      }

   return res; 
}

//
// Multiplication algorithm.
//
bigint operator* (const bigint& left, const bigint& right) {
   bigint res;
   bigvalue_t left_bv, right_bv;

   left_bv = left.get_big_value();
   right_bv = right.get_big_value();
   res.set_big_value(do_bigmul(left_bv, right_bv));
   if (left.get_negative() == right.get_negative())
      res.set_negative(false);
   else 
      res.set_negative(true);

   return res;
}

//
// Division algorithm.
//

void multiply_by_2 (bigint::unumber& unumber_value) {
   unumber_value *= 2;
}

void divide_by_2 (bigint::unumber& unumber_value) {
   unumber_value /= 2;
}


bigint::quot_rem divide (const bigint& left, const bigint& right) {
   if (right == 0) throw domain_error ("divide by 0");
   using unumber = unsigned long;
   static unumber zero = 0;
   if (right == 0) throw domain_error ("bigint::divide");
   unumber divisor = right.long_value;
   unumber quotient = 0;
   unumber remainder = left.long_value;
   unumber power_of_2 = 1;
   while (abs_less (divisor, remainder)) {
      multiply_by_2 (divisor);
      multiply_by_2 (power_of_2);
   }
   while (abs_less (zero, power_of_2)) {
      if (not abs_less (remainder, divisor)) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divide_by_2 (divisor);
      divide_by_2 (power_of_2);
   }
   return {quotient, remainder};
}

bigint operator/ (const bigint& left, const bigint& right) {
   return divide (left, right).first;
}

bigint operator% (const bigint& left, const bigint& right) {
   return divide (left, right).second;
}

bool operator== (const bigint& left, const bigint& right) {
   bool res = false;

   cout << "call ==\n"; //del
   if (left.get_negative() == right.get_negative())
      if (!abs_cmp(left.get_big_value(), right.get_big_value()))
           res = true;

   return res;
}

bool operator< (const bigint& left, const bigint& right) {
   if (left.get_negative() == false and right.get_negative == true)
      return true;
   else if (left.get_negative() == true and right.get_negative == false)
      return  false;
   else {
      if (abs_cmp(left.get_big_value(), right.get_big_value()) < 0) {
         if (left.get_negative() == false)
            return false;
         return true;
      }
      else if (abs_cmp(left.get_big_value(), right.get_big_value()) == 0) 
         return false;
      else {
         if (left.get_negative() == false)
            return true;
         return false;
      }
   }
}

ostream& operator<< (ostream& out, const bigint& that) {
   string long_num;

   auto rit = that.big_value.rbegin();
   for(; rit != that.big_value.rend(); ++rit)
      long_num.push_back(*rit);
   
   bool neg = that.get_negative();
   string a = "+", b = "-";
   out << (neg? b: a)<< long_num;

   return out;
}


bigint pow (const bigint& base, const bigint& exponent) {
   DEBUGF ('^', "base = " << base << ", exponent = " << exponent);
   if (base == 0) return 0;
   bigint base_copy = base;
   long expt = exponent.to_long();
   bigint result = 1;
   if (expt < 0) {
      base_copy = 1 / base_copy;
      expt = - expt;
   }
   while (expt > 0) {
      if (expt & 1) { //odd
         result = result * base_copy;
         --expt;
      }else { //even
         base_copy = base_copy * base_copy;
         expt /= 2;
      }
   }
   DEBUGF ('^', "result = " << result);
   return result;
}

bigvalue_t bigint::get_big_value() const {
   bigvalue_t big;

   for(size_t i = 0; i < big_value.size(); ++i)
      big.push_back(big_value.at(i));

   return big;
}

void bigint::set_big_value(const bigvalue_t & big) {
   big_value.clear();
   for(size_t i = 0; i < big.size(); ++i)
      big_value.push_back(big.at(i));
}

bool bigint::get_negative() const {
   return negative;
}

bool get_neg_reverse(const bool & neg) {
   if (neg == true)
      return false;
   else
      return true; 
}

void bigint::set_negative(const bool & neg) {
   negative = neg;
}

bigvalue_t get_reverse(const bigvalue_t & old) {
   bigvalue_t new_vec;

   auto rit = old.rbegin();
   for(; rit != old.rend(); ++rit)
      new_vec.push_back(*rit);

   return new_vec;
}

pair<int, int> do_div (const int & dividend, const int & divisor) {
   int quot, rem;

   pair<int, int> quot_rem;

   if (divisor != 0) {
      quot = dividend / divisor;
      rem = dividend % divisor;
      quot_rem = make_pair(quot, rem); 
   }

   return quot_rem;
}

