#include <iostream>
#include <memory>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

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


int main() {
  map<string, size_t> word_count;
  string word;
  while (cin >> word) {
    ++word_count[word];
    for (const auto &w:word_count) {
    cout << w.first << " occurs " << w.second << endl;
    }
  }

  


  return 0;
}