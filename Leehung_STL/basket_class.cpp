#include <cstdio>
#include <iostream>
#include <set>
#include <memory>
#include <string>

using namespace std;

// 定义基类
class Quote {
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) {}

    std::string const isbn() const { return bookNo; }

    // 基类希望派生类进行覆盖的函数通常定义为虚函数。关键字virtual使得函数执行动态绑定

    // 派生类负责改写并使用不同折扣算法
    virtual double net_price(size_t n) const { return n * price; }

    // virtual对析构函数进行动态绑定
    virtual ~Quote() = default;

    static void statmem();

private:
    std::string bookNo;

// 只许派生类访问的成员
protected:
    double price = 0.0;
};

template<class T>
double print_total(ostream& os, T& __t, size_t cnt) {
    os << __t.net_price(cnt) << endl;
    return __t.net_price(cnt);
}

// 定义派生类
class Bulk_quote : public Quote {
public:
    Bulk_quote() = default;
    Bulk_quote(const string& book, double p, size_t qty, double disc) : 
    Quote(book, p), min_qty(qty), discount(disc) {}

    // 隐式包含isbn函数

    // 定义新版本
    double net_price(size_t) const override;

private:
    // 新增数据成员
    size_t min_qty = 0;
    double discount = 0.0;
};

double Bulk_quote::net_price(size_t cnt) const {
    if (cnt >= min_qty) return cnt * (1- discount) * price;
    else return cnt * price;
}

class Basket {
public:
    void add_item(const std::shared_ptr<Quote> &sale) {
        items.insert(sale);
    }

    double total_receipt(std::ostream&) const;

private:
    static bool compare(const std::shared_ptr<Quote> &lhs, const std::shared_ptr<Quote> &rhs) {
        return lhs->isbn() < rhs->isbn();
    }

    // 定义一个multiset，将Quote和compare成员类型相同的函数对其中过的元素进行排序
    std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};
};

double Basket::total_receipt(std::ostream &os) const {
    double sum = 0.0;
    for (auto iter = items.cbegin(); 
              iter != items.cend(); 
              iter = items.upper_bound(*iter)) {
        sum += print_total(os, **iter, items.count(*iter));
    }
}

int main () {

    Basket bsk;
    bsk.add_item(make_shared<Quote>("123", 35));
    bsk.add_item(make_shared<Bulk_quote>("123", 35, 3, 0.3));
    


    return 0;
}