/*
容器使用适配器alloctor，用来进行内存空间的分配和释放

空间分配器有两种：
    1、__malloc_alloc_template，即时分配即时释放
    2、__default_alloc_template，小型内存池

容器通过配置器的静态函数来分配、释放内存，配置器底层调用malloc和free来满足需求
*/ 

#pragma once
#include "stl_config.h"
#include "stl_construct.h"
#include <cstdlib>
#include <cstdio>

__STL_BEGIN_NAMESPACE

template <int __inst>
class __malloc_alloc_template {
public:
    static void* allocate(size_t n) {
        void* __result = malloc(n);
        if (0 == __result) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        return __result;
    }

    static void* deallocate(void* __p, size_t) {
        free(__p);
    }

    static void* reallocate(void* __p, size_t, size_t __new_sz) {
        void* __result = realloc(__p, __new_sz);
        if (0 == __result) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        return __result;
    }
};

typedef __malloc_alloc_template<0> malloc_alloc;

// _Tp 是将要存储在向量中元素的类型, _Alloc是将用于为向量分配和释放内存的分配器类型
template<class _Tp, class _Alloc>
class simple_alloc {
public:
    static _Tp* allocate(size_t __n) {
        return 0 == __n ? 0 : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
    }

    static _Tp* allocate(void) {
        return (_Tp*)_Alloc::allocate(sizeof(_Tp));
    }

    static void deallocate(_Tp* __p, size_t __n) {
        if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(_Tp));
    }

    static void deallocate(_Tp* __p)
	{
		_Alloc::deallocate(__p, sizeof(_Tp));
	}
};

typedef malloc_alloc alloc;


__STL_END_NAMESPACE
