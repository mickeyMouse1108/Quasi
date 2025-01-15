#include "Str.h"

#include "Span.h"
#include "String.h"

namespace Quasi {
    namespace Chr {
        char         FromDigit   (u32 digit) { return (char)('0' + digit); }
        char         FromHexDigit(u32 digit) { return (char)(digit < 10 ? '0' + digit : 'A' + digit); }
        Option<char> TryFromDigit   (u32 digit)              { return digit < 10    ? Options::Some(FromDigit(digit))    : nullptr; }
        Option<char> TryFromHexDigit(u32 digit)              { return digit < 16    ? Options::Some(FromHexDigit(digit)) : nullptr; }
        Option<char> TryFromDigitRadix(u32 digit, u32 radix) { return digit < radix ? Options::Some(FromHexDigit(digit)) : nullptr; }

        u32         ToDigit      (char digit) { return (u32)(digit - '0'); }
        u32         ToHexDigit   (char digit) { return IsNumeric(digit) ? digit - '0' : IsUpper(digit) ? digit - 'A' : digit - 'a'; }
        Option<u32> TryToDigit   (char digit) { return IsDigit(digit)    ? Options::Some(ToDigit(digit))    : nullptr; }
        Option<u32> TryToHexDigit(char digit) { return IsHexDigit(digit) ? Options::Some(ToHexDigit(digit)) : nullptr; }
        Option<u32> TryToDigitRadix(char digit, u32 radix) { return IsDigitRadix(digit, radix) ? Options::Some(ToHexDigit(digit)) : nullptr; }

        bool IsDigit   (char digit) { return '0' <= digit && digit <= '9'; }
        bool IsHexDigit(char digit) { return IsDigit(digit) || ('a' <= ToLower(digit) && ToLower(digit) <= 'f') ; }
        bool IsDigitRadix(char digit, u32 radix) {
            if (radix <= 10) return '0' <= digit && digit < '0' + radix;
            return IsDigit(digit) || ('a' <= ToLower(digit) && ToLower(digit) < 'a' + digit - 10);
        }

        bool ShouldEscape(char c) { return c == '\t' || c == '\n' || c == '\r' || c == '\'' || c == '\"' || c == '\\'; }
        char EscapedRepr(char c) {
            switch (c) {
                case '\t': return 't';
                case '\n': return 'n';
                case '\r': return 'r';
                default: return c;
            }
        }

        bool IsAscii(char c) { return ' ' <= c && c <= '~'; }
        bool IsNonAscii(char c) { return !IsAscii(c); }

        char ToLower(char c) { return IsUpper(c) ? (char)(c |  32) : c; }
        char ToUpper(char c) { return IsLower(c) ? (char)(c & ~32) : c; }
        bool IsLower(char c) { return 'a' <= c && c <= 'z'; }
        bool IsUpper(char c) { return 'A' <= c && c <= 'Z'; }

        bool IsBlank     (char c) { return c == ' ' || c == '\t'; }
        bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }
        bool IsAlpha     (char c) { return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'); }
        bool IsNumeric   (char c) { return IsDigit(c); }
        bool IsAlphaNum  (char c) { return IsAlpha(c) || IsNumeric(c); }
        bool IsPrintable (char c) { return 0x1f < c && c < 0x7f; }
        bool IsControl   (char c) { return (0x00 <= c && c <= 0x1f) || c == 0x7f; }
        bool IsGraphic   (char c) { return IsPrintable(c) && c != ' '; }
        bool IsPunct     (char c) { return IsGraphic(c) && !IsAlphaNum(c); }


        Set::Set(Span<const char> chars) {
            u64 bitmask[4] {};
            for (const char c : chars) {
                bitmask[(uchar)c / 64] |= 1 << ((uchar)c & 63);
            }
        }

        bool Set::operator()(char c) const {
            return bitmask[(uchar)c / 64] & 1 << ((uchar)c & 63);
        }
        bool Set::operator()(Str str) const { return operator()(str[0]); }
    }

#define strdef template <class Char, class Super>
#define strcls StringHolder<Char, Super>

    strdef BufferIterator<const char&> strcls::Iter()    const        { return { Data(), Length() }; }
    strdef BufferIterator<char&>       strcls::IterMut() requires mut { return { Data(), Length() }; }

    strdef Str              strcls::AsStr()      const        { return Str   ::Slice(Data(),    Length()); }
    strdef StrMut           strcls::AsStrMut()   requires mut { return StrMut::Slice(DataMut(), Length()); }
    strdef Span<const char> strcls::AsSpan()     const        { return Span<const char>::FromBuffer(Data(),    Length()); }
    strdef Span<char>       strcls::AsSpanMut()  requires mut { return Span<char>      ::FromBuffer(DataMut(), Length()); }
    strdef Span<const byte> strcls::AsBytes()    const        { return Span<const byte>::FromBuffer((const byte*)Data(), Length()); }
    strdef Span<byte>       strcls::AsBytesMut() requires mut { return Span<byte>      ::FromBuffer((byte*)DataMut(),    Length()); }
    strdef strcls::operator Str()    const        { return AsStr(); }
    strdef strcls::operator StrMut() requires mut { return AsStrMut(); }

    strdef Span<const char> strcls::Subspan   (usize start) const              { return Subspan(start, Length() - start); }
    strdef Str              strcls::Substr    (usize start) const              { return Substr (start, Length() - start); }
    strdef Span<const char> strcls::Subspan   (usize start, usize count) const { return Span<const char>::FromBuffer(Data() + start, count); }
    strdef Str              strcls::Substr    (usize start, usize count) const { return Str             ::Slice     (Data() + start, count); }
    strdef Span<char>       strcls::SubspanMut(usize start) requires mut { return SubspanMut(start, Length() - start); }
    strdef StrMut           strcls::SubstrMut (usize start) requires mut { return SubstrMut (start, Length() - start); }
    strdef Span<char>       strcls::SubspanMut(usize start, usize count) requires mut { return Span<char>::FromBuffer(DataMut() + start, count); }
    strdef StrMut           strcls::SubstrMut (usize start, usize count) requires mut { return StrMut    ::Slice     (DataMut() + start, count); }

    strdef StrMut strcls::FirstMut(usize num) requires mut { return SubstrMut(0, num); }
    strdef StrMut strcls::SkipMut(usize len)  requires mut { return SubstrMut(len); }
    strdef StrMut strcls::TailMut()           requires mut { return SubstrMut(1); }
    strdef StrMut strcls::LastMut(usize num)  requires mut { return SubstrMut(Length() - num, num); }
    strdef StrMut strcls::TruncMut(usize len) requires mut { return SubstrMut(0, Length() - len); }
    strdef StrMut strcls::InitMut()           requires mut { return SubstrMut(Length() - 1); }
    strdef Tuple<char&,  StrMut>        strcls::SplitFirstMut()          requires mut { return { FirstMut(),   TailMut() }; }
    strdef Tuple<StrMut, char&>         strcls::SplitLastMut()           requires mut { return { InitMut(),    LastMut() }; }
    strdef Tuple<StrMut, StrMut>        strcls::SplitAtMut(usize at)     requires mut { return { FirstMut(at), SkipMut(at) }; }
    strdef Tuple<StrMut, char&, StrMut> strcls::PartitionAtMut(usize at) requires mut { return { FirstMut(at), At(at), SkipMut(at + 1) }; }
    strdef Str strcls::First(usize num) const { return Substr(0, num); }
    strdef Str strcls::Skip(usize len)  const { return Substr(len); }
    strdef Str strcls::Tail()           const { return Substr(1); }
    strdef Str strcls::Last(usize num)  const { return Substr(Length() - num, num); }
    strdef Str strcls::Trunc(usize len) const { return Substr(0, Length() - len); }
    strdef Str strcls::Init()           const { return Substr(Length() - 1); }
    strdef Tuple<const char&, Str>      strcls::SplitFirst()          const { return { First(),   Tail() }; }
    strdef Tuple<Str, const char&>      strcls::SplitLast()           const { return { Init(),    Last() }; }
    strdef Tuple<Str, Str>              strcls::SplitAt(usize at)     const { return { First(at), Skip(at) }; }
    strdef Tuple<Str, const char&, Str> strcls::PartitionAt(usize at) const { return { First(at), At(at), Skip(at + 1) }; }

    strdef bool strcls::RefEquals     (Str other) const { return Data() == other.Data() && Length() == other.Length(); }
    strdef bool strcls::ContainsBuffer(Str buf)   const { return buf.Data() >= Data() && Data() + Length() >= buf.Data() + buf.Length(); }
    strdef bool strcls::OverlapsBuffer(Str buf)   const {
        const char* end = Data() + Length(), *bufEnd = buf.Data() + buf.Length();
        return end >= buf.Data() && bufEnd >= Data();
    }
    strdef bool strcls::Equals(Str other) const {
        if (Length() != other.Length()) return false;
        usize i = 0;
        for (; i < Length(); i += 8)
            if (Memory::ReadU64(Data() + i) != Memory::ReadU64(other.Data() + i)) return false;

        for (usize j = 0; j < Length() & 7; ++j)
            if (At(i - 8 + j) != other.At(i - 8 + j)) return false;
        return true;
    }
    strdef bool strcls::operator==(Str other) const { return Equals(other); }

    strdef Comparison strcls::Cmp(Str other) const {
        usize i = 0;
        for (; i < Length(); i += 8) {
            const auto cmp = Cmp::Between(Memory::ReadU64(Data() + i), Memory::ReadU64(other.Data() + i));
            if (cmp != Cmp::EQUAL) return cmp;
        }
        for (usize j = 0; j < Length() & 7; ++j) {
            const auto cmp = Cmp::Between(At(i - 8 + j), other.At(i - 8 + j));
            if (cmp != Cmp::EQUAL) return cmp;
        }
        return Cmp::Between(Length(), other.Length());
    }
    strdef Comparison strcls::CmpSized(Str other) const {
        if (Length() != other.Length()) return Cmp::Between(Length(), other.Length());
        usize i = 0;
        for (; i < Length(); i += 8) {
            const auto cmp = Cmp::Between(Memory::ReadU64(Data() + i), Memory::ReadU64(other.Data() + i));
            if (cmp != Cmp::EQUAL) return cmp;
        }
        for (usize j = 0; j < Length() & 7; ++j) {
            const auto cmp = Cmp::Between(At(i - 8 + j), other.At(i - 8 + j));
            if (cmp != Cmp::EQUAL) return cmp;
        }
        return Cmp::EQUAL;
    }
    strdef Comparison strcls::operator<=>(Str other) const { return Cmp(other); }

    strdef void  strcls::Reverse() requires mut { AsSpanMut().Reverse(); }

    strdef usize strcls::Find       (char c)  const { for (usize i = 0; i < Length(); ++i) if (At(i) == c) return i; return USIZE_MAX; }
    strdef usize strcls::RevFind    (char c)  const { for (usize i = Length(); i --> 0; )  if (At(i) == c) return i; return USIZE_MAX; }
    strdef bool  strcls::Contains   (char c)  const { return Find   (c) != USIZE_MAX; }
    strdef bool  strcls::RevContains(char c)  const { return RevFind(c) != USIZE_MAX; }
    strdef usize strcls::Find       (Str str) const {
        for (usize i = 0; i < Length() - str.Length(); ++i)
            if (Substr(i, str.Length()) == str) return i;
        return USIZE_MAX;
    }
    strdef usize strcls::RevFind    (Str str) const {
        for (usize i = Length() - str.Length(); i --> 0; )
            if (Substr(i, str.Length()) == str) return i;
        return USIZE_MAX;
    }
    strdef bool  strcls::Contains   (Str str) const { return Find   (str) != USIZE_MAX; }
    strdef bool  strcls::RevContains(Str str) const { return RevFind(str) != USIZE_MAX; }
    strdef Tuple<usize, usize> strcls::FindOneOf(Span<const char> anyc) const {
        const usize i = FindIf(Chr::Set { anyc });
        return { i, i == USIZE_MAX ? USIZE_MAX : anyc.Find(At(i)) };
    }
    strdef Tuple<usize, usize> strcls::RevFindOneOf(Span<const char> anyc) const {
        const usize i = RevFindIf(Chr::Set { anyc });
        return { i, i == USIZE_MAX ? USIZE_MAX : anyc.Find(At(i)) };
    }
    strdef usize strcls::ContainsOneOf   (Span<const char> anyc) const { const usize [i, m] = FindOneOf(anyc);    return i == USIZE_MAX ? USIZE_MAX : m; }
    strdef usize strcls::RevContainsOneOf(Span<const char> anyc) const { const usize [i, m] = RevFindOneOf(anyc); return i == USIZE_MAX ? USIZE_MAX : m; }
    strdef Tuple<usize, usize> strcls::FindOneOf       (Span<const Str> anystr) const {
        for (usize i = 0; i < Length(); ++i) {
            for (usize j = 0; j < anystr.Length(); ++j)
                if (Skip(i).StartsWith(anystr)) return { i, j };
        }
        return { USIZE_MAX, USIZE_MAX };
    }
    strdef Tuple<usize, usize> strcls::RevFindOneOf    (Span<const Str> anystr) const {
        for (usize i = Length(); i --> 0; ) {
            for (usize j = 0; j < anystr.Length(); ++j)
                if (Skip(i).StartsWith(anystr)) return { i, j };
        }
        return { USIZE_MAX, USIZE_MAX };
    }
    strdef usize strcls::ContainsOneOf   (Span<const Str> anystr) const { const usize [i, m] = FindOneOf(anystr);    return i == USIZE_MAX ? USIZE_MAX : m; }
    strdef usize strcls::RevContainsOneOf(Span<const Str> anystr) const { const usize [i, m] = RevFindOneOf(anystr); return i == USIZE_MAX ? USIZE_MAX : m; }

    strdef bool  strcls::StartsWith(char prefix) const { return Length() >= 1 && First() == prefix; }
    strdef bool  strcls::EndsWith  (char suffix) const { return Length() >= 1 && Last()  == suffix; }
    strdef bool  strcls::StartsWith(Str prefix)  const { return Length() >= prefix.Length() && First(prefix.Length()) == prefix; }
    strdef bool  strcls::EndsWith  (Str suffix)  const { return Length() >= suffix.Length() && Last (suffix.Length()) == suffix; }
    strdef usize strcls::StartsWithOneOf(Span<const char> anyprefix) const { return Length() >= 1 ? anyprefix.Find(First()) : USIZE_MAX; }
    strdef usize strcls::EndsWithOneOf  (Span<const char> anysuffix) const { return Length() >= 1 ? anysuffix.Find(Last())  : USIZE_MAX; }
    strdef usize strcls::StartsWithOneOf(Span<const Str> anyprefix)  const { for (usize i = 0; i < anyprefix.Length(); ++i) if (StartsWith(anyprefix[i])) return i; return USIZE_MAX; }
    strdef usize strcls::EndsWithOneOf  (Span<const Str> anysuffix)  const { for (usize i = 0; i < anysuffix.Length(); ++i) if (StartsWith(anysuffix[i])) return i; return USIZE_MAX; }


    strdef Str strcls::Trim     () const { return TrimIf     (Chr::IsWhitespace); }
    strdef Str strcls::TrimStart() const { return TrimStartIf(Chr::IsWhitespace); }
    strdef Str strcls::TrimEnd  () const { return TrimEndIf  (Chr::IsWhitespace); }
    strdef Str strcls::Trim     (char c) const { return TrimIf     (Cmp::Equals { c }); }
    strdef Str strcls::TrimStart(char c) const { return TrimStartIf(Cmp::Equals { c }); }
    strdef Str strcls::TrimEnd  (char c) const { return TrimEndIf  (Cmp::Equals { c }); }
    strdef Str strcls::TrimOneOf     (Span<const char> anyc) const { return TrimIf     (Chr::Set { anyc }); }
    strdef Str strcls::TrimStartOneOf(Span<const char> anyc) const { return TrimStartIf(Chr::Set { anyc }); }
    strdef Str strcls::TrimEndOneOf  (Span<const char> anyc) const { return TrimEndIf  (Chr::Set { anyc }); }
    strdef StrMut strcls::TrimMut     () requires mut { return TrimIfMut     (Chr::IsWhitespace); }
    strdef StrMut strcls::TrimStartMut() requires mut { return TrimStartIfMut(Chr::IsWhitespace); }
    strdef StrMut strcls::TrimEndMut  () requires mut { return TrimEndIfMut  (Chr::IsWhitespace); }
    strdef StrMut strcls::TrimMut     (char c) requires mut { return TrimIfMut     (Cmp::Equals { c }); }
    strdef StrMut strcls::TrimStartMut(char c) requires mut { return TrimStartIfMut(Cmp::Equals { c }); }
    strdef StrMut strcls::TrimEndMut  (char c) requires mut { return TrimEndIfMut  (Cmp::Equals { c }); }
    strdef StrMut strcls::TrimOneOfMut     (Span<const char> anyc) requires mut { return TrimIfMut     (Chr::Set { anyc }); }
    strdef StrMut strcls::TrimStartOneOfMut(Span<const char> anyc) requires mut { return TrimStartIfMut(Chr::Set { anyc }); }
    strdef StrMut strcls::TrimEndOneOfMut  (Span<const char> anyc) requires mut { return TrimEndIfMut  (Chr::Set { anyc }); }

    strdef Str strcls::RemovePrefix(char prefix) const { return Skip (StartsWith(prefix)); }
    strdef Str strcls::RemoveSuffix(char suffix) const { return Trunc(EndsWith(suffix));   }
    strdef Str strcls::RemovePrefix(Str prefix)  const { return Skip (StartsWith(prefix) ? prefix.Length() : 0); }
    strdef Str strcls::RemoveSuffix(Str suffix)  const { return Trunc(EndsWith(suffix)   ? suffix.Length() : 0); }
    strdef Str strcls::RemovePrefixOneOf(Span<const char> prefix) const { const usize i = StartsWithOneOf(prefix); return Skip (i != USIZE_MAX); }
    strdef Str strcls::RemoveSuffixOneOf(Span<const char> suffix) const { const usize i = EndsWithOneOf  (suffix); return Trunc(i != USIZE_MAX); }
    strdef Str strcls::RemovePrefixOneOf(Span<const Str> prefix)  const { const usize i = StartsWithOneOf(prefix); return Skip (i == USIZE_MAX ? 0 : prefix.Length()); }
    strdef Str strcls::RemoveSuffixOneOf(Span<const Str> suffix)  const { const usize i = EndsWithOneOf  (suffix); return Trunc(i == USIZE_MAX ? 0 : suffix.Length()); }
    strdef StrMut strcls::RemovePrefixMut(char prefix) requires mut { return SkipMut (StartsWith(prefix)); }
    strdef StrMut strcls::RemoveSuffixMut(char suffix) requires mut { return TruncMut(EndsWith(suffix));   }
    strdef StrMut strcls::RemovePrefixMut(Str prefix)  requires mut { return SkipMut (StartsWith(prefix) ? prefix.Length() : 0); }
    strdef StrMut strcls::RemoveSuffixMut(Str suffix)  requires mut { return TruncMut(EndsWith(suffix)   ? suffix.Length() : 0); }
    strdef StrMut strcls::RemovePrefixOneOfMut(Span<const char> prefix) requires mut { const usize i = StartsWithOneOf(prefix); return SkipMut (i != USIZE_MAX); }
    strdef StrMut strcls::RemoveSuffixOneOfMut(Span<const char> suffix) requires mut { const usize i = EndsWithOneOf  (suffix); return TruncMut(i != USIZE_MAX); }
    strdef StrMut strcls::RemovePrefixOneOfMut(Span<const Str> prefix)  requires mut { const usize i = StartsWithOneOf(prefix); return SkipMut (i == USIZE_MAX ? 0 : prefix.Length()); }
    strdef StrMut strcls::RemoveSuffixOneOfMut(Span<const Str> suffix)  requires mut { const usize i = EndsWithOneOf  (suffix); return TruncMut(i == USIZE_MAX ? 0 : suffix.Length()); }

    strdef String strcls::ToString() const {
        return String::FromStr(*this);
    }

    strdef String strcls::Reversed() const {
        String rev = String::WithCap(Length());
        for (usize i = Length(); i --> 0; )
            rev.Append(At(i));
        return rev;
    }

    strdef String strcls::ToUpper() const {
        String upper = String::WithCap(Length());
        for (const char c : super()) upper.Append(Chr::ToUpper(c));
        return upper;
    }
    strdef String strcls::ToLower() const {
        String lower = String::WithCap(Length());
        for (const char c : super()) lower.Append(Chr::ToLower(c));
        return lower;
    }

    strdef void strcls::MakeUpper() requires mut {
        for (char& c : super()) c = Chr::ToUpper(c);
    }

    strdef void strcls::MakeLower() requires mut {
        for (char& c : super()) c = Chr::ToLower(c);
    }

    strdef String strcls::Repeat(usize n) const {
        String rep = String::WithCap(Length() * n);
        for (usize i = 0; i < n; ++i)
            for (const char c : super())
                rep.Append(c);
        return rep;
    }

    strdef String strcls::Replace(char from, Str to) const {
        return ReplaceIf([=] (Str str) { return str[0] == from ? 1 : 0; }, to);
    }

    strdef String strcls::Replace(Str from, Str to) const {
        if (from.IsEmpty()) return ToString();
        return ReplaceIf([&] (Str str) { return str.StartsWith(from) ? from.Length() : 0; }, to);
    }

    strdef String strcls::ReplaceAnyOf(Span<const char> from, Str to) const {
        if (from.IsEmpty()) return ToString();
        const Chr::Set set = { from };
        return ReplaceIf([&] (Str str) { return set(str[0]) ? 1 : 0; }, to);
    }

    strdef String strcls::ReplaceAnyOf(Span<const Str> from, Str to) const {
        if (from.IsEmpty()) return ToString();
        return ReplaceIf([&] (Str str) {
            const usize i = str.StartsWithOneOf(from);
            return i == -1 ? 0 : from[i].Length();
        }, to);
    }

    Str& Str::Advance(usize num) {
        data += num;
        size -= num;
        return *this;
    }

    Str& Str::Shorten(usize amount) {
        size -= amount;
        return *this;
    }

    Str Str::TakeFirst(usize num) {
        const Str first = First(num);
        Advance(num);
        return first;
    }

    Str Str::TakeLast(usize num) {
        const Str last = Last(num);
        Shorten(num);
        return last;
    }

    const char& Str::TakeFirst() {
        const char& first = First();
        Advance(1);
        return first;
    }

    const char& Str::TakeLast() {
        const char& last = Last();
        Shorten(1);
        return last;
    }

    Str Str::TakeAfter(usize i) {
        const Str after = Skip(i);
        size = i;
        return after;
    }

    StrMut Str::AsMut() { return StrMut::Slice(Memory::AsMutPtr(data), size); }

    StrMut& StrMut::Advance(usize num) {
        data += num;
        size -= num;
        return *this;
    }

    StrMut& StrMut::Shorten(usize amount) {
        size -= amount;
        return *this;
    }

    StrMut StrMut::TakeFirstMut(usize num) {
        const StrMut first = FirstMut(num);
        Advance(num);
        return first;
    }

    StrMut StrMut::TakeLastMut(usize num) {
        const StrMut last = LastMut(num);
        Shorten(num);
        return last;
    }

    char& StrMut::TakeFirstMut() {
        char& first = FirstMut();
        Advance(1);
        return first;
    }

    char& StrMut::TakeLastMut() {
        char& last = LastMut();
        Shorten(1);
        return last;
    }

    StrMut StrMut::TakeAfterMut(usize i) {
        const StrMut after = SkipMut(i);
        size = i;
        return after;
    }

    Str StrMut::TakeFirst(usize num) {
        const Str first = First(num);
        Advance(num);
        return first;
    }

    Str StrMut::TakeLast(usize num) {
        const Str last = Last(num);
        Shorten(num);
        return last;
    }

    const char& StrMut::TakeFirst() {
        const char& first = First();
        Advance(1);
        return first;
    }

    const char& StrMut::TakeLast() {
        const char& last = Last();
        Shorten(1);
        return last;
    }

    Str StrMut::TakeAfter(usize i) {
        const Str after = Skip(i);
        size = i;
        return after;
    }
} // Quasi