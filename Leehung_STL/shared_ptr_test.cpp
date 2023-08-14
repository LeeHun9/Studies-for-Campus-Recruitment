#include <iostream>
#include <memory>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <list>

using namespace std;

void biggies(vector<string> &words, vector<string>::size_type sz) {
  sort(words.begin(), words.end());
  stable_sort(words.begin(), words.end(), [](const string &a, const string &b) {return a.size() > b.size();});
  auto wc = find_if(words.begin(), words.end(), [sz] (const string &a) {return a.size() > sz; });

  auto count = words.end() - wc;
  //std::cout << count << " " << make_plural(count, "word", "s") << 

  for_each (wc, words.end(), [] (const string &s) {std::cout << s << " ";});

  std::cout << endl;
}

void bbb(vector<string> &words, vector<string>::size_type sz, std::ostream &os = std::cout, char c = ' ') {
  int a = 10;
  for_each(words.begin(), words.end(), [&, c, a] (const string &s) { os << s << c << a;});
}


class strBlob {
public:
  typedef vector<string>::size_type size_type;
  strBlob();
  strBlob(initializer_list<string> il);
  size_type size() const {return data->size(); }
  bool empty() const { return data->empty(); }

  void push_back(const string &t) {
    data->push_back(t);
  }

  void pop_back();
  string& front();
  string& back();
private:
  shared_ptr<vector<string>> data;
  void check(size_type i, const string &msg) const;
};

strBlob::strBlob():data(make_shared<vector<string>>()) {}
strBlob::strBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)) {}

void strBlob::check(size_type i, const string &msg) const {
  if (i >= data->size()) throw out_of_range(msg);
}

string& strBlob::front() {
  check(0, "front on empty blob");
  return data->front();
}

string& strBlob::back() {
  check(0, "back on empty blob");
  return data->back();
}

void strBlob::pop_back() {
  check(0, "pop_back on empty blob");
  data->pop_back();
}



int main() {
  shared_ptr<string> p1;
  shared_ptr<list<int>> p2;

  if (p1 && p1->empty()) *p1 = "hi";

  shared_ptr<int> p3 = make_shared<int>(32);
  shared_ptr<string> p4 = make_shared<string>(10, 'a');

  shared_ptr<int> p5 = make_shared<int> ();

  auto p6 = make_shared<vector<string>>();

  auto p = make_shared<int>(32);
  p = p3;
  auto q(p);


  cout << *p5 << endl;

  return 0;
}