STL组成由以下6个部分组成
- 容器（Container）: vector, list, deque, set, map
- 空间配置器（allocator）: 负责空间配置与管理
- 算法（Algorithm）: sort, search, copy, erase ...
- 迭代器（Iterator）: 泛型指针
- 仿函数（Function object）: 行为类似函数，作为算法的某种策略。
- 适配器（Adaptor）: 一种用来修饰容器或仿函数或迭代器接口的东西，如 queue, stack，其底层全部借助deque支持。改变container接口的交container adapter；改变iterator接口的称为iterator adapter。


STL六大组件内在联系

- 空间分配器为容器分配内存空间。
- 算法只关心具体的做法，而不关心是什么容器 。所以需要迭代器作为容器与算法的桥梁，将两者连接起来。
- 算法中会常常搭配仿函数来更好的完成其工作
- 适配器将一个class的接口转换为另一个 class 的接口，使原本因接口不兼容而不能合作的 class，可以一起运作。
- 容器所使用的迭代器，其相当一部分操作是在容器内实现的。


SGI STL 文件分布与介绍
- stl_algo.h: 算法
- stl_algobase.h: 基本算法, swap, min, max, copy, fill, equal...
- stl_alloc.h: 空间分配器，std::alloc
- stl_config.h: 针对各家编译器特性定义的各种环境变量
- stl_construct.h: 构造/析构基本工具
- stl_iterator.h: 迭代器及其相关配接器，并定义迭代器常用函数advance(), distance()
- 
- stl_pair.h: 成对组合
- stl_vector.h: vector
- stl_list.h: list


# 0x01 空间配置器



# 0x02 迭代器与traits
迭代器是一种能够顺序访问容器中每个元素的方法，使用该方法不能暴露容器内部的表达方式。类型萃取技术就是为了要解决和 iterator 有关的问题的。

## 五类迭代器

- 输入迭代器：只读，不写；单遍扫描，只能递增
- 输出迭代器：只写，不读；单遍扫描，只能递增
- 前向迭代器：可读写；多遍扫描，只能递增
- 双向迭代器：可读写；多遍扫描，递增递减
- 随机访问迭代器：可读写，多遍扫描，支持全部迭代器运算

### 输入迭代器：
读取序列中元素，支持：

- 比较两个迭代器的相等和不相等运算符 ==, !=
- 推进迭代器前置和后置递增运算符 ++
- 读取元素的解引用运算符 *；解引用只会出现在赋值运算符的右侧
- 箭头运算符 -> 等价于 (*it).member，即解引用迭代器，并提取对象的成员

### 输出迭代器
可以看作输入迭代器功能上的补集，支持：
- 推进迭代器前置和后置递增运算符 ++
- 读取元素的解引用运算符 *；解引用只会出现在赋值运算符的右侧

### 前向迭代器：
可读写元素，只能在序列中沿一个方向移动。支持所有输入和输出迭代器操作，可多次读写一个元素。

### 双向迭代器：
可以正向/反向读写序列中元素。还支持递减运算符 --。reverse算法要求双向迭代器，除了forward_list，其他标准库提供符合双向迭代器要求的迭代器。

### 随机访问迭代器：
提供在常量事件内访问序列中任意元素的能力。该类迭代器支持双向迭代器的所有功能，还支持：
- 比较两个迭代器相对位置的关系运算符 <, <=, >, >=
- 迭代器和一个整数值的加减运算 +, +=, -, -=，计算结果是迭代器在序列中前进或后退给定整数个元素后的位置
- 用于两个迭代器上的减法运算符 -，得到两个迭代器的距离
- 下标运算符 (iter[n])，与*(iter[n]) 等价




## _type_traits
traits 编程弥补了C++的不足，STL只对迭代器加以规范，SGI将这种方法扩展到迭代器之外，有了 `_type_traits`。

 `_type_traits` 负责萃取type的特性。我们关注的type特性指：
 - type是否具备 non-trivial defalt cto? 是否使用默认构造函数
 - 是否具备 non-trivial copy ctor? 是否使用默认拷贝构造函数
 - 是否具备 non-trivial assignment operator? 是否使用默认赋值运算符
 - 是否具备 non-trivial dtor? 是否使用默认析构函数

 如果答案否定，我们在对这个type进行构造、析构、拷贝、赋值等操作时，就可以采用最有效率的措施。对于大规模且操作频繁的容器，效率有显著提升。


### template 参数推导
在算法中使用迭代器时，很可能会用到其相应型别（associated type）。算法设计中需要生命一个变量——以“迭代器所指向对象的型别”：
```c++
template <class I, class T>
void func_impl(I iter, T t) {
        T tmp; // 这里就是迭代器所指物的类别
        // ... 功能实现
}

template <class T>
inline void func(I iter) {
    func_impl(iter, *iter);
}

int main() {
    int i;
    func(&i);
}
```
这样就获取了指向所指向对象的类型。但函数template参数推导机制无法推导函数的返回类型。

### 声明内嵌型别
迭代器所指向对象的型别，称为迭代器的 `value_type`。

声明内嵌型别就可以直接获取。
```c++
template <class T>
struct MyIter {
    typedef T value_type; // 内嵌型别声明
    T* ptr;
    MyIter(T* p = 0) : ptr(p) {}
    T& operator*() const { return *ptr; }
};

template <class I>
typename I::value_type
func(I ite) {
    std::cout << "class version" << std::endl;
    return *ite;
}
int main() {
    // ...
    MyIter<int> ite(new int(8));
    cout << func(ite);    // 输出8
}
```
但func无法支持原生指针
```c++
int *p = new int(5);
cout<<func(p)<<endl; // error
```

### traits

如果直接使用typename I::value_type，算法就无法接收原生指针，因为原生指针根本就没有 value_type 这个内嵌类型。需要加入一个中间层对其进行判断，看它是不是原生指针。


**1、函数偏特化**

这种偏特化是针对可调用函数 func 的偏特化

```c++
template <class T>
struct MyIter {
    typedef T value_type; // 内嵌型别声明
    T* ptr;
    MyIter(T* p = 0) : ptr(p) {}
    T& operator*() const { return *ptr; }
};

template <class I>
typename I::value_type
func(I ite) {
    std::cout << "class version" << std::endl;
    return *ite;
}
template <class I>
I
func(I* ite) {
    std::cout << "pointer version" << std::endl;
    return *ite;
}
template <class I>
I func(const I* ite) {
    std::cout << "const pointer version" << std::endl;
    return *ite;
}
int main() {
    // ...
    MyIter<int> ite(new int(8));
    cout << func(ite)<<endl;
    int *p = new int(52);
    cout<<func(p)<<endl;
    const int k = 3;
    cout<<func(&k)<<endl;
}
```

输出

```
class version
8
pointer version
52
const pointer version
3
```

**2、加入中间层**

利用一个中间层iterator_traits固定了func的形式，使得重复的代码大量减少，唯一要做的就是稍稍特化一下 iterator_tartis 使其支持 pointer 和 const pointer。



# 0x0x 算法

算法形参模式
```c++
alg(beg, end, other args);
alg(beg, end, dest, other args);
alg(beg, end, beg2, other args);
alg(beg, end, beg2, end2, other args);
```

