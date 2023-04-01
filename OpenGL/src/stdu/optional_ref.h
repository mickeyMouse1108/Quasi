#pragma once
#include <optional>

namespace stdu {
    template <class T>
    struct optional_ref {
        T* obj = nullptr;
        
        optional_ref() : obj(nullptr) {}
        optional_ref(const T& t) : obj(&t) {}
        optional_ref(const T* t) : obj( t) {}
        optional_ref(const std::optional<T>& opt) : obj(&*opt) {}
        optional_ref(std::nullptr_t) : obj(nullptr) {}
        optional_ref(std::nullopt_t) : obj(nullptr) {}

        T& value() { return *obj; }
        [[nodiscard]] const T& value() const { return *obj; }
        [[nodiscard]] const T& value_or(const T& none) const { return obj == nullptr ? none : *obj; }
        [[nodiscard]] bool has_value() const { return obj != nullptr; }

        T& operator*() { return *obj; }
        const T& operator*() const { return *obj; }
        T* operator->() { return obj; }
        const T* operator->() const { return obj; }

        operator bool() { return obj != nullptr; }
        operator T() { return *obj; }
    };
}
