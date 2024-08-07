#pragma once
#include "Ref.h"

namespace Quasi {
    template <class T>
    struct Option {
        T value;
        bool is_some = true;

        Option() : Option(nullptr) {}
        Option(std::nullptr_t) : is_some(false) {}
        Option(const T& value) : value(value) {}

        [[nodiscard]] bool IsNull()   const { return !is_some; }
        [[nodiscard]] bool HasValue() const { return is_some; }
        [[nodiscard]] operator bool() const { return HasValue(); }
        const T& ValueOr(const T& val) const { return is_some ? value : val; }
        const Option& Or(const Option& val) const { return is_some ? *this : val; }

        [[nodiscard]] Ref<const T> Value() const { return is_some ? Refer(value) : nullptr; }
        Ref<T> Value() { return is_some ? Refer(value) : nullptr; }

        [[nodiscard]] const T& operator*() const { return Value(); }
        T& operator*() { return Value(); }
        [[nodiscard]] const T* operator->() const { return Value().Address(); }
        T* operator->() { return Value().Address(); }

        T& Assert();
        [[nodiscard]] const T& Assert() const;
        template <class Asrt> T& Assert(Asrt&& assertfn);
        template <class Asrt> [[nodiscard]] const T& Assert(Asrt&& assertfn) const;
    };

    template <class T>
    Option<std::remove_cvref_t<T>> Some(T&& value) {
        return Option<std::remove_cvref_t<T>> { std::forward<T>(value) };
    }
}