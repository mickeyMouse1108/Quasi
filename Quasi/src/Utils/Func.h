#pragma once
#include <utility>

#include "Memory.h"
#include "Macros.h"
#include "Tuple.h"

namespace Quasi {
    template <class Res, class... Args>
    using FuncPtr = Res(*)(Args...);

    template <class Fn>
    struct FuncRef {};

    template <class Result, class... Args>
    struct FuncRef<Result(Args...)> {
    private:
        void* userPtr = nullptr;
        FuncPtr<Result, void*, Args...> functionPtr = nullptr;

        FuncRef(void* uptr, FuncPtr<Result, void*, Args...> fptr) : userPtr(uptr), functionPtr(fptr) {}
    public:
        Result Invoke(Args... args) const { return functionPtr(userPtr, (Args&&)args...); }
        Result operator()(Args... args) const { return Invoke((Args&&)args...); }

        FuncRef() = default;
        FuncRef(Nullptr) : FuncRef() {}
        FuncRef(FuncPtr<Result, Args...> fptr) : userPtr(fptr), functionPtr(+[] (void* f, Args... args) {
            return (*Memory::UpcastPtr<FuncPtr<Result, Args...>>(f))((Args&&)args...);
        }) {}

        static FuncRef FromRaw(void* uptr, FuncPtr<Result, void*, Args...> fptr) { return { uptr, fptr }; }

        FuncRef(const FuncRef& f) = default;
        FuncRef(FuncRef&& f) noexcept = default;
        FuncRef& operator=(const FuncRef& f) = default;
        FuncRef& operator=(FuncRef&& f) noexcept = default;

        template <class Lamb> requires DistantTo<Lamb, FuncRef>
        FuncRef(Lamb&& lamb)
            : userPtr(&lamb),
            functionPtr(+[] (void* functionObj, Args... args) {
                return (*Memory::UpcastPtr<std::decay_t<Lamb>>(functionObj))((Args&&)args...);
            })
        {}

        operator bool() const { return functionPtr; }

        template <class Lamb>
        static FuncRef Recursive(Lamb&& lamb) {
            return {
                &lamb,
                [] (void* functionObj, Args... args) {
                    auto* f = Memory::UpcastPtr<std::decay_t<Lamb>>(functionObj);
                    return (*f)(f, (Args&&)args...);
                }
            };
        }

        template <class OtherFn>
        FuncRef<OtherFn> Transmute() const {
            return Memory::Transmute<FuncRef<OtherFn>>(*this);
        }
    };

    namespace FuncRefs {
        template <class O, class... Is>
        FuncRef<O(Is...)> FromRaw(void* user, O(*fptr)(void*, Is...)) { return FuncRef<O(Is...)>::FromRaw(user, fptr); }
    }

    template <class T> struct Vec;

    namespace Combinate {
        struct Identity {
            template <class T> T&& operator()(T&& in) { return std::forward<T>(in); }
        };

        template <class F>
        struct Y {
            F func;
            auto operator()(auto&&... args) { return func(*this, std::forward<decltype(args)>(args)...); }
        };

        template <class F>
        struct Negate { F func; bool operator()(auto&&... args) { return !func(std::forward<decltype(args)>(args)...); } };

        template <class T>
        struct Collect {
            Vec<T> elements;
            void operator()(const T& val) { elements.Push(val); }
            void operator()(T&& val) { elements.Push(std::move(val)); }
        };

        template <class T, class F>
        struct Bind {
            T first;
            const F& func;
            auto operator()(auto&&... rest) const { return func(first, (decltype(rest))rest...); }
        };

        template <class T>
        struct Constructor {
            T operator()(auto&&... args) const { return T { (decltype(args))args... }; }
        };

        template <class... Fs>
        struct Overload : Fs... {
            using Fs::operator()...;
        };
        template <class... Fs>
        Overload(Fs&&...) -> Overload<Fs...>;
    }

    namespace Operators {
        template <auto Indirect>
        struct Member      { auto& operator()(const auto& x) const { return x.*Indirect; } };
        template <auto Indirect>
        struct MemberArrow { auto& operator()(const auto& x) const { return x->*Indirect; } };

        template <class T>
        struct Create {
            T operator()(auto&&... args) const { return T { args... }; }
        };

        struct Destructor {
            template <class T>
            void operator()(T& t) const { t.~T(); }
        };

#define Q_CREATE_UNARY_OPERATOR_FUNCTOR(NAME, OP) struct NAME { auto operator()(auto&& x) const { return OP; } };
#define Q_CREATE_OPERATOR_FUNCTOR(NAME, OP) struct NAME { auto operator()(auto&& x, auto&& y) const { return OP; } };
        // arithmetic
        Q_CREATE_UNARY_OPERATOR_FUNCTOR(UPos, +x)
        Q_CREATE_UNARY_OPERATOR_FUNCTOR(UNeg, -x)
        Q_CREATE_OPERATOR_FUNCTOR(Add, x + y) Q_CREATE_OPERATOR_FUNCTOR(AddAssign, x += y)
        Q_CREATE_OPERATOR_FUNCTOR(Sub, x - y) Q_CREATE_OPERATOR_FUNCTOR(SubAssign, x -= y)
        Q_CREATE_OPERATOR_FUNCTOR(Mul, x * y) Q_CREATE_OPERATOR_FUNCTOR(MulAssign, x *= y)
        Q_CREATE_OPERATOR_FUNCTOR(Div, x / y) Q_CREATE_OPERATOR_FUNCTOR(DivAssign, x /= y)
        Q_CREATE_OPERATOR_FUNCTOR(Mod, x % y) Q_CREATE_OPERATOR_FUNCTOR(ModAssign, x %= y)
        Q_CREATE_UNARY_OPERATOR_FUNCTOR(Inc, ++x) Q_CREATE_UNARY_OPERATOR_FUNCTOR(PostInc, x++)
        Q_CREATE_UNARY_OPERATOR_FUNCTOR(Dec, --x) Q_CREATE_UNARY_OPERATOR_FUNCTOR(PostDec, x--)
        // bitwise
        Q_CREATE_OPERATOR_FUNCTOR(BitAnd, x &  y) Q_CREATE_OPERATOR_FUNCTOR(BitAndAssign, x &=  y)
        Q_CREATE_OPERATOR_FUNCTOR(BitOr,  x |  y) Q_CREATE_OPERATOR_FUNCTOR(BitOrAssign,  x |=  y)
        Q_CREATE_OPERATOR_FUNCTOR(BitXor, x ^  y) Q_CREATE_OPERATOR_FUNCTOR(BitXorAssign, x ^=  y)
        Q_CREATE_OPERATOR_FUNCTOR(BitShl, x << y) Q_CREATE_OPERATOR_FUNCTOR(BitShlAssign, x <<= y)
        Q_CREATE_OPERATOR_FUNCTOR(BitShr, x >> y) Q_CREATE_OPERATOR_FUNCTOR(BitShrAssign, x >>= y)
        Q_CREATE_UNARY_OPERATOR_FUNCTOR(Compl, ~x)
        // logical
        Q_CREATE_UNARY_OPERATOR_FUNCTOR(Not, !x)
        Q_CREATE_OPERATOR_FUNCTOR(And, x && y)
        Q_CREATE_OPERATOR_FUNCTOR(Or,  x || y)
        // other
        Q_CREATE_OPERATOR_FUNCTOR(Index, x[y])
#undef Q_CREATE_OPERATOR_FUNCTOR
#undef Q_CREATE_UNARY_OPERATOR_FUNCTOR

        // the difference between mod and numericModulo is that modulo supports fmod.
        struct NumericModulo { template <class T> T operator()(T x, T y) const { return NumInfo<T>::Modulo(x, y); } };
    }

    template <class F, class O, class... I>
    concept Fn = requires (F& f, I&&... args) {
        { f(std::forward<I>(args)...) } -> ConvTo<O>;
    };

    template <class F, class... I>
    concept FnArgs = requires (F& f, I&&... args) {
        { f(std::forward<I>(args)...) } -> AlwaysTrue;
    };

    template <class Fn, class... Ts> using FuncResult = decltype(std::declval<Fn>()(std::declval<Ts>()...));

    template <class Fn> struct ArgTypes {};
    template <class Res, class... Args> struct ArgTypes<Res(*)(Args...)> { using Result = Tuple<Args...>; };
    template <class Fn> using ArgumentsOf = typename ArgTypes<Fn>::Result;

    template <class F, class T>
    concept Predicate = Fn<F, bool, const T&>;

#define Q_LMB_TAKES_ARGS(...) , Q_TINY_LAMBDA_SIMPLE,
#define Q_TINY_LAMBDA_ARGS(...) [&] (__VA_ARGS__) { return
#define Q_TINY_LAMBDA_SIMPLE(...) Q_TINY_LAMBDA_ARGS __VA_ARGS__ ; }
#define Q_TINY_LAMBDA_FORWARD_TO(M) [&] (auto&&... args) { return M((decltype(args))args...); }
#define Q_TINY_LAMBDA(...) Q_INVOKE(Q_ARGS_SECOND, Q_LMB_TAKES_ARGS __VA_ARGS__, Q_TINY_LAMBDA_FORWARD_TO) (__VA_ARGS__)

#define Qfn$(...) Q_TINY_LAMBDA(__VA_ARGS__)
} // Q
