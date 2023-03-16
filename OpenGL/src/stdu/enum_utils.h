#pragma once
#include <type_traits>

namespace stdu {
#define ENUM_DY_OP(op) template <class E> requires std::is_enum_v<E> E operator op (int a, E b) { return (E)(a op (int)b); } \
                       template <class E> requires std::is_enum_v<E> E operator op (E a, int b) { return (E)((int)a op b); } \
                       template <class E> requires std::is_enum_v<E> E operator op (E a, E b) { return (E)((int)a op (int)b); }

#define ENUM_CMP(op) template <class E> requires std::is_enum_v<E> bool operator op (int a, E b) { return a op (int)b; } \
                     template <class E> requires std::is_enum_v<E> bool operator op (E a, int b) { return (int)a op b; } \

#define ENUM_MN_OP(op) template <class E> requires std::is_enum_v<E> E operator op (E x) { return (E)(op (int)x); }

#define ENUM_SE_OP(op) template <class E> requires std::is_enum_v<E> E operator op##= (E& a, int b) { a = (E)((int)a op b); return a; }
#define ENUM_SE_IN(op) template <class E> requires std::is_enum_v<E> E operator op##op (E& a) { a = (E)((int)a op 1); return a; } \
                       template <class E> requires std::is_enum_v<E> E operator op##op (E& a, int) { E t = a; a = (E)((int)a op 1); return t; }
    
    ENUM_DY_OP(+)
    ENUM_DY_OP(-)
    ENUM_DY_OP(*)
    ENUM_DY_OP(/)
    ENUM_DY_OP(%)
    ENUM_DY_OP(&)
    ENUM_DY_OP(|)
    ENUM_DY_OP(^)
    ENUM_DY_OP(<<)
    ENUM_DY_OP(>>)

    ENUM_CMP(<)
    ENUM_CMP(<=)
    ENUM_CMP(>)
    ENUM_CMP(>=)
    ENUM_CMP(==)
    ENUM_CMP(!=)

    ENUM_MN_OP(+)
    ENUM_MN_OP(-)
    ENUM_MN_OP(~)

    ENUM_SE_OP(+)
    ENUM_SE_OP(-)
    ENUM_SE_OP(*)
    ENUM_SE_OP(/)
    ENUM_SE_OP(%)
    ENUM_SE_OP(&)
    ENUM_SE_OP(|)
    ENUM_SE_OP(^)
    ENUM_SE_OP(<<)
    ENUM_SE_OP(>>)

    ENUM_SE_IN(+)
    ENUM_SE_IN(-)

#undef ENUM_DY_OP

#undef ENUM_MN_OP

#undef ENUM_SE_OP
#undef ENUM_SE_IN
}
