#pragma once

namespace stdu {
    template <class T, class... TArgs, class TFun>
    T static_make(TFun&& fun, TArgs&&... args) {
        T obj{ };
        fun(obj, args);
        return obj;
    }
}
