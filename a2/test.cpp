#include <iostream>
#include <vector>

using namespace std;

int main() {
	bool cmp;
	char a = 'a';
	char b = '9';
        int c = 5;
        vector<unsigned char> v;
        v.push_back('2');
        //v.insert(v.begin(), 0, 3);
        for(size_t i = 0; i < v.size(); ++i)
           cout << v.at(i) << endl;


	//c = (int)b - '0';
//	cout << c << endl;
return 0;
}
