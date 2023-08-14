
#include <iostream>
#include <cstdio>

using namespace std;

template<class T>
void print_total(ostream& os, T& __t, size_t cnt) {
    os << __t.net_price(cnt) << endl;
}

// 定义基类
class Quote {
public:
    Quote() = default;
    Quote(const string &book, double sales_price) : bookNo(book), price(sales_price) {}

    string const isbn() const { return bookNo; }

    // 基类希望派生类进行覆盖的函数通常定义为虚函数。关键字virtual使得函数执行动态绑定

    // 派生类负责改写并使用不同折扣算法
    virtual double net_price(size_t n) const { return n * price; }

    // virtual对析构函数进行动态绑定
    virtual ~Quote() = default;

    static void statmem();

private:
    string bookNo;

// 只许派生类访问的成员
protected:
    double price = 0.0;
};

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


// Bulk_quote 是BBB的直接基类，Quote是BBB的间接基类
class BBB : public Bulk_quote {

};

// 使用 final 定义不能作为基类；
// 还可以使用final定义函数，之后尝试覆盖该函数的操作都将引发错误。
class A {
    virtual void func(int a) const {
        cout << a << endl;
    }  
    
};

class No_Derived final : public A{
    void func(int a) const final;
};

class Disc_quote : public Quote {
public: 
    Disc_quote() = default;
    Disc_quote(const string& book, double price, size_t qty, double disc) :
        Quote(book, price), quantity(qty), discount(disc) {}

    // 声明语句处（分号之前）加 =0，就可将一个虚函数说明为纯虚函数
    double net_price(size_t) const = 0; 

private:
    size_t quantity = 0;
    double discount = 0.0;
};

int main() {

    // 基类指针和引用绑定到派生类对象中的基类部分上
    Quote item("qwerqw", 6);
    Bulk_quote bulk;
    Quote* q = &item;
    q = &bulk;
    Quote& r = bulk;

    Bulk_quote bb("sdfsaf", 4.2, 3, 0.9);
    cout << bb.net_price(5) << endl;

    print_total(cout, bb, 10);
    print_total(cout, item, 10);


    // 含有纯虚函数的类称为抽象基类，不能直接定义抽象基类的对象
    //Disc_quote nn; error


    return 0;
}