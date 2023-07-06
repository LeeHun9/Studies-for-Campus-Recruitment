#include "stl_vector.h"
#include <iostream>
#include <string>

using namespace std;

using __STL_NAME::vector;

class test {
private:
    string* _ptr;
public:
    test(string id = "-1") {
        _ptr = new string[10];
        for (int i = 0; i < 10; i++) {
            _ptr[i] = id;
        }
        cout << "construct: " << id << endl;
    }

    void print() {
        cout << _ptr[0] << endl;
    }

    ~test() {
        cout << _ptr[0] << " destroy" << endl;
        delete[] _ptr;
    }
};


int main() {
    vector<test> t;
    t.push_back(test());
    t.push_back(test("hello"));
    t.begin()->print();
    return 0;
}
