#include "Num.h"

#include "Debug/Logger.h"

namespace Quasi::Text {
    u32 NumberConversion::Add4Bytes(u32 a, u32 b) {
        static constexpr u32 EVEN_BYTES = 0xFF00FF00, ODD_BYTES = 0x00FF00FF;
        return (((a & EVEN_BYTES) + (b & EVEN_BYTES)) & EVEN_BYTES) |
               (((a &  ODD_BYTES) + (b &  ODD_BYTES)) &  ODD_BYTES);
    }

    bool NumberConversion::AreAllInCharRange8(u64 digits, uchar min, uchar max) {
        const u64 a = digits + 0x8080'8080'8080'8080 - max * 0x0101'0101'0101'0101;
        const u64 b = digits                         - min * 0x0101'0101'0101'0101;
        return ((a | b) & 0x8080'8080'8080'8080) == 0;
    }

    bool NumberConversion::AreAllInCharRange4(u32 digits, uchar min, uchar max) {
        const u32 a = digits + 0x8080'8080 - max * 0x0101'0101;
        const u32 b = digits               - min * 0x0101'0101;
        return ((a | b) & 0x8080'8080) == 0;
    }

    bool NumberConversion::AreAllDigits4(u32 digits) {
        return AreAllInCharRange4(digits, '0', '9' + 1);
    }

    bool NumberConversion::AreAllDigitsRadix4(u32 digits, u32 radix) {
        return AreAllInCharRange4(digits, '0', '0' + radix);
    }

    bool NumberConversion::AreAllDigits8(u64 digits) {
        return AreAllInCharRange8(digits, '0', '9' + 1);
    }

    bool NumberConversion::AreAllHexDigits4(u32 digits) {
        const u32 aLo = Add4Bytes(digits, 0x7F7F7F7F - "9999"_u32),
                  aHi = Add4Bytes(digits,            - "0000"_u32),
                  bLo = Add4Bytes(digits, 0x7F7F7F7F - "FFFF"_u32),
                  bHi = Add4Bytes(digits,            - "AAAA"_u32),
                  cLo = Add4Bytes(digits, 0x7F7F7F7F - "ffff"_u32),
                  cHi = Add4Bytes(digits,            - "aaaa"_u32);
        return (((aLo | aHi) & (bLo | bHi) & (cLo | cHi)) & 0x8080'8080) == 0;
    }

    bool NumberConversion::AreAllHexDigitsRadix4(u32 digits, u32 radix) {
        const u32 aLo = Add4Bytes(digits, 0x7F7F7F7F - "9999"_u32),
                  aHi = Add4Bytes(digits,            - "0000"_u32),
                  bLo = Add4Bytes(digits, 0x80808080 - "AAAA"_u32 - (radix - 10) * 0x0101'0101),
                  bHi = Add4Bytes(digits,            - "AAAA"_u32),
                  cLo = Add4Bytes(digits, 0x80808080 - "aaaa"_u32 - (radix - 10) * 0x0101'0101),
                  cHi = Add4Bytes(digits,            - "aaaa"_u32);
        return (((aLo | aHi) & (bLo | bHi) & (cLo | cHi)) & 0x8080'8080) == 0;
    }

    u32 NumberConversion::ConvertDigits4(u32 chars) {
        return chars - "0000"_u32;
    }

    u64 NumberConversion::ConvertDigits8(u64 chars) {
        return chars - "00000000"_u64;
    }

    u32 NumberConversion::ConvertHexDigits4(u32 chars) {
        // removes unnessecary information,
        // ranges '0'-'9' now become 0-9
        // ranges 'A'-'F'/'a'-'f' now both become 0x41-0x47
        chars &= 0x4F4F4F4F4F4F4F4F;
        // alphabet characters all have the 6th bit set
        const u64 isAlpha = chars & 0x4040404040404040;
        // removes the extra bit for alphabets, now 0x41-0x47 -> 0x01-0x07
        // adds 9 if alpha is set, 0x01-0x07 -> 10-16
        chars = (isAlpha >> 6) * 9 + (chars ^ isAlpha);
        return chars;
    }

    // adapted from https://doc.rust-lang.org/src/core/num/dec2flt/parse.rs.html
    // which was further adapted from "Fast numeric string to
    // int", available here: <https://johnnylee-sde.github.io/Fast-numeric-string-to-int/>.
    // assumes digits are all in range of '0'-'9'
    u64 NumberConversion::ParseDigits8(u64 digits) {
        static constexpr u64 MASK = 0x0000'00FF'0000'00FF;
        static constexpr u64 MUL1 = 0x000F'4240'0000'0064; // 1'00'00'00 << 32 + 1'00
        static constexpr u64 MUL2 = 0x0000'2710'0000'0001; // 1'00'00    << 32 + 1
        // digits is now an array of 8 digits (0-9) for each byte (ABCDEFGH)
        digits = ConvertDigits8(digits);
        // consecutive digits are concat-ed (A0 BA CB DC ED FE GF HG)
        digits = digits * 10 + (digits >> 8);
        // retrives digits #4 and #8
        //   ________________[d4] ________________[d8] (each 32 bits)
        //   ____________[100*d4] ____________[100*d8] -> (* 100)
        //   ________[1000000*d8] ____________________ -> (<< 32 * 1000000)
        // + +++++++++++++++++++++++++++++++++++++++++
        //   _[1000000*d8+100*d4] ____________[100*d8]
        // v1 = [HG00DC00] [0000HG00]
        const u64 v1 = (digits & MASK) * MUL1;
        // retrieves digits #2 and #6
        //   ________________[d2] ________________[d6] (each 32 bits)
        //   __________[10000*d6] ____________________ -> (<< 32 * 10000)
        // + +++++++++++++++++++++++++++++++++++++++++
        //        [10000*d6 + d2] ________________[d6]
        // v2 = [00FE00BA] [000000FE]
        const u64 v2 = ((digits >> 16) & MASK) * MUL2;
        // v1      = [HG00DC00] [0000HG00]
        // v2      = [00FE00BA] [000000FE]
        // +         +++++++++++++++++++++
        // v1 + v2 = [HGFEDCBA] [0000HGFE] <- bottom 32 bits are discarded
        return (v1 + v2) >> 32;
    }

    u32 NumberConversion::ParseDigits4(u32 digits) {
        digits = ConvertDigits4(digits);
        digits = digits * 10 + (digits >> 8);
        return ((digits & 0x00FF00FF) * ((100 << 16) + 1)) >> 16;
    }

    u32 NumberConversion::ParseHexDigits4(u32 xdigits) {
        xdigits = ConvertHexDigits4(xdigits);
        xdigits = (xdigits * 0x1001) >> 8;
        return ((xdigits & 0x00FF00FF) * ((256 << 16) + 1)) >> 16;
    }

    u32 NumberConversion::ParseDigitsTinyRadix4(u32 dig, u32 radix) {
        static constexpr u32 TINY_LOOKUP[5] = { 0, 0, 0x08'04'02'01, 0x'1B'09'03'01, 0x40'20'04'01 };
        return (dig * TINY_LOOKUP[radix]) >> 24;
    }

    u32 NumberConversion::ParseDigitsSmallRadix4(u32 dig, u32 radix) {
        dig = dig * radix + (dig >> 8);
        return ((dig & 0x00FF00FF) * ((radix * radix << 16) + 1)) >> 16;
    }

    u32 NumberConversion::ParseDigitsLargeRadix4(u32 dig, u32 radix) {
        const u32 d = (dig >> 24) & 0xFF,
                  c = (dig >> 16) & 0xFF,
                  b = (dig >> 8)  & 0xFF,
                  a = (dig >> 0)  & 0xFF;
        return ((a * radix + b) * radix + c) * radix + d;
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseBinaryInt(Str string, Out<I&> out, bool neg) {
        return ParseIntBy<I, 8>(string, out, neg, [] (I& n, const char* str) {
            u64 digs = Memory::ReadU64Little(str);
            digs = ConvertDigits8(digs);
            if (digs & 0x7E7E7E7E7E7E7E7E) return false;
            // magic multiplication to make array of 8 bits into a u8
            digs = (digs * 0x80'40'20'10'08'04'02'01) >> 56;
            n = (n << 8) + digs;
            return true;
        }, sizeof(I) * 8, true, 2);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseDecimalInt(Str string, Out<I&> out, bool neg) {
        return ParseIntBy<I, 4>(string, out, neg, [] (I& n, const char* str) {
            u32 digs = Memory::ReadU32Little(str);
            if (!AreAllDigits4(digs)) return false;
            digs = ParseDigits4(digs);
            n = n * 10'000 + digs;
            return true;
        }, sizeof(I) * Math::INV_LOG10_2_MUL >> 13, false, 10);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseHexInt(Str string, Out<I&> out, bool neg) {
        return ParseIntBy<I, 4>(string, out, neg, [] (I& n, const char* str) {
            u32 digs = Memory::ReadU32Little(str);
            if (!AreAllHexDigits4(digs)) return false;
            digs = ParseHexDigits4(digs);
            n = (n << 16) + digs;
            return true;
        }, sizeof(I) * 2, true, 16);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseTinyRadixInt(Str string, Out<I&> out, bool neg, u32 radix) {
        const bool hasInexactDigit = radix & 1; // when radix is 3 instead of 2/4, the log is an irrational
        return ParseIntBy<I, 4>(string, out, neg, [&, r4 = radix * radix * radix * radix] (I& n, const char* str) {
            u32 digs = Memory::ReadU32Little(str);
            if (!AreAllDigitsRadix4(digs, radix)) return false;
            digs = ConvertDigits4(digs);
            digs = ParseDigitsTinyRadix4(digs, radix);
            n = n * r4 + digs;
            return true;
        }, hasInexactDigit + (sizeof(I) * Math::INV_LOG2_LOOKUP[radix] >> 13), !hasInexactDigit, radix);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseSmallRadixInt(Str string, Out<I&> out, bool neg, u32 radix) {
        return ParseIntBy<I, 4>(string, out, neg, [&, r4 = radix * radix * radix * radix] (I& n, const char* str) {
            u32 digs = Memory::ReadU32Little(str);
            if (!AreAllDigitsRadix4(digs, radix)) return false;
            digs = ConvertDigits4(digs);
            digs = ParseDigitsSmallRadix4(digs, radix);
            n = n * r4 + digs;
            return true;
        }, 1 + (sizeof(I) * Math::INV_LOG2_LOOKUP[radix] >> 13), false, radix);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseAsciiRadixInt(Str string, Out<I&> out, bool neg, u32 radix) {
        return ParseIntBy<I, 4>(string, out, neg, [&, r4 = radix * radix * radix * radix] (I& n, const char* str) {
            u32 digs = Memory::ReadU32Little(str);
            if (!AreAllHexDigitsRadix4(digs, radix)) return false;
            digs = ConvertHexDigits4(digs);
            digs = ParseDigitsSmallRadix4(digs, radix);
            n = n * r4 + digs;
            return true;
        }, 1 + (sizeof(I) * Math::INV_LOG2_LOOKUP[radix] >> 13), false, radix);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseLargeRadixInt(Str string, Out<I&> out, bool neg, u32 radix) {
        return ParseIntBy<I, 4>(string, out, neg, [&, r4 = radix * radix * radix * radix] (I& n, const char* str) {
            u32 digs = Memory::ReadU32Little(str);
            if (!AreAllHexDigitsRadix4(digs, radix)) return false;
            digs = ConvertHexDigits4(digs);
            digs = ParseDigitsLargeRadix4(digs, radix);
            n = n * r4 + digs;
            return true;
        }, 1 + (sizeof(I) * Math::INV_LOG2_LOOKUP[radix] >> 13), false, radix);
    }

    template <Integer I>
    OptionUsize NumberConversion::ParseInt(Str string, Out<I&> out, IntParseOptions options) {
        bool negative = false;
        if (string.StartsWith('+')) string.Advance(1);
        if constexpr (Signed<I>)
            if (string.StartsWith('-')) { negative = true; string.Advance(1); }

        if (options.radix == IntParseOptions::ADAPTIVE) {
            if (string.StartsWith('0')) {
                switch (string[2]) {
                    case 'b': return ParseBinaryInt(string.Skip(2), out, negative);
                    case 'x': return ParseHexInt   (string.Skip(2), out, negative);
                    default:  return ParseSmallRadixInt(string, out, negative, 8);
                }
            } else {
                return string ? ParseDecimalInt(string, out, negative) : nullptr;
            }
        }
        if (string.IsEmpty()) return nullptr;
        if (options.radix == IntParseOptions::BINARY)  return ParseBinaryInt (string, out, negative);
        if (options.radix == IntParseOptions::DECIMAL) return ParseDecimalInt(string, out, negative);
        if (options.radix == IntParseOptions::HEX)     return ParseHexInt    (string, out, negative);

        if (options.radix <= 4)
            return ParseTinyRadixInt(string, out, negative, options.radix);
        else if (options.radix < 10)
            return ParseSmallRadixInt(string, out, negative, options.radix);
        else if (options.radix < 16)
            return ParseAsciiRadixInt(string, out, negative, options.radix);
        else return ParseLargeRadixInt(string, out, negative, options.radix);
    }

    template <Floating F>
    OptionUsize NumberConversion::ParseNanOrInf(Str string, Out<F&> out) {
        // utilizes u64s to optimize string comparisons
        constexpr u64 CLEAR_CASE = 0xDFDFDFDFDFDFDFDF;
        if (string.Length() < 3) return nullptr;
        if (string.Length() < 8) {
            u64 first3 = string[0] << 16 | string[1] << 8 | string[0];
            first3 &= CLEAR_CASE;
            if (first3 == "INF"_u64) {
                out = Math::Infinity;
            } else if (first3 == "NAN"_u64) {
                out = Math::NaN;
            } else return nullptr;
            return 3;
        } else {
            u64 first8 = Memory::ReadU64(string.Data());
            first8 &= CLEAR_CASE;
            if (first8 == "INFINITY"_u64) {
                out = Math::Infinity;
                return 8;
            }
        }
        return nullptr;
    }

    u64 NumberConversion::U64ToBCD2(u64 x) {
        // x          = [000] [0AB]
        // tens       = [000] [00A]
        // tens * 256 = [00A] [000]
        // tens * -10 = [000] [-A0]
        const u32 tenCarry = x * 6554 >> 16 * (256 - 10); // shifting by 16 = /65536, 6554/65536 ~ 1/10
        x += tenCarry;
        return x;
    }

    u64 NumberConversion::U64ToBCD4(u64 x) {
        // x   = [0000ABCD]
        // top = [000000AB]
        //             vvvvvvvvvvv ~1/100
        u64 top = ((x * 5243) >> 19) & 0xFF;
        // x   = [00AB00CD]
        x += top * (65536 - 100);
        // top = [000A000C]
        top = ((x * 103) >> 9) & 0xF0000F;
        // x   = [0A0B0C0D]
        x += top * (256 - 10);
        return x;
    }

    u64 NumberConversion::U64ToBCD8(u64 x) {
        // x   = [00000000ABCDEFGH]
        // top = [000000000000ABCD]
        u32 top = (x * 109951163) >> 40; // 1/10000
        // x   = [0000ABCD0000EFGH]
        x += top * ((1 << 32) - 10000);
        // top = [000000AB000000EF]
        top = ((x * 5243) >> 19) & 0xFF000000FF;
        // x   = [00AB00CD00EF00GH]
        x += top * (65536 - 100);
        // top = [000A000C000E000G]
        top = ((x * 103) >> 9) & 0x000F000F000F000F;
        // x   = [0A0B0C0D0E0F0G0H]
        x += top * (256 - 10);
        return x;
    }

    u32 NumberConversion::SmallU64ToString(u64 x, char* out) {
        if (x <= 9) {
            out[0] = (char)(x | 0x30);
            return 1;
        } else {
            x = U64ToBCD2(x);
            Memory::WriteU16(x | 0x3030, out);
            return 2;
        }
    }

    u32 NumberConversion::U64ToString(u64 x, char* out) {
        if (x <= 99)
            return SmallU64ToString(x, out);

        const u32 len = (x > 999) ? 4 : 3;
        x = U64ToBCD4(x);
        x |= 0x30303030;

        if (len == 4) {
            Memory::WriteU32(x, out);
        } else {
            Memory::WriteU16(x >> 8, out);
            out[2] = (char)(x & 0xFF);
        }
        return len;
    }

    u32 NumberConversion::U64FullToString(u64 x, char* out) {
        u32 len;
        if (x <= 9999)
            return U64ToString(x, out);
        if (x < 100000000) { // 8 digits or less, can fit in 64 bits
            if (x > 999999)
                len = x > 9999999 ? 8 : 7;
            else
                len = x > 99999 ? 6 : 5;
        } else {
            const u64 skip8 = x * 0x55E63B89 >> 57; // 10^-8
            x -= (skip8 * 100'000'000);
            // h will be at most 42
            // calc num digits
            len = U64ToString(skip8, out);
            out += 8;
            len += 8;
        }

        x = U64ToBCD8(x);
        x |= 0x3030303030303030;

        Memory::MemCopyNoOverlap(out, ((const char*)&x) + 8 - (-8 | -len), len);
        return len;
    }

    usize NumberConversion::FormatU64(StringWriter sw, u64 num, const IntFormatOptions& options, char sign) {
        if (num == 0) {
            const u32 padLen = options.totalLength - options.numLen;
            const u32 left = padLen * (usize)options.alignment / 2;
            sw.WriteRepeat(options.pad, left);
            sw.WriteRepeat(options.shouldPadZero ? '0' : ' ', options.numLen - 1);
            sw.Write('0');
            sw.WriteRepeat(options.pad, padLen - left);
            return options.totalLength;
        }

        u32 nlen;
        switch (options.base) {
            case IntFormatOptions::DECIMAL: nlen = 1 + u64s::Log10(num); break;
            case IntFormatOptions::BINARY:  nlen = u64s::BitWidth(num);  break;
            case IntFormatOptions::OCTAL:   nlen = (u64s::BitWidth(num) + 2) / 3; break;
            case IntFormatOptions::HEX:
            case IntFormatOptions::CAP_HEX: nlen = (u64s::BitWidth(num) + 3) / 4; break;
        }

        const u32 targetnLen = std::max(nlen, options.numLen) + (sign != '\0');
        const u32 padLen = options.totalLength - std::min(options.totalLength, targetnLen);
        const u32 left = padLen * (usize)options.alignment / 2;

        sw.WriteRepeat(options.pad, left);
        sw.WriteRepeat(options.shouldPadZero ? '0' : ' ', targetnLen - nlen);

        if (sign)
            sw.Write(sign);

        switch (options.base) {
            case IntFormatOptions::DECIMAL: WriteU64Decimal(sw, num);          break;
            case IntFormatOptions::BINARY:  WriteU64Binary(sw, num, nlen);     break;
            case IntFormatOptions::OCTAL:   WriteU64Octal(sw, num, nlen);      break;
            case IntFormatOptions::HEX:     WriteU64Hex(sw, num, nlen, false); break;
            case IntFormatOptions::CAP_HEX: WriteU64Hex(sw, num, nlen, true);  break;
        }

        sw.WriteRepeat(options.pad, padLen - left);

        return std::max(options.totalLength, targetnLen);
    }

    usize NumberConversion::FormatI64(StringWriter sw, i64 num, const IntFormatOptions& options) {
        const bool negative = num < 0;
        return FormatU64(sw, negative ? (u64)-num : (u64)num, options, negative ? '-' : options.showSign ? '+' : '\0');
    }

    void NumberConversion::WriteU64Decimal(StringWriter sw, u64 num) {
        char strbuf[u64s::DIGITS] = {};
        sw.Write(Str::Slice(strbuf, U64FullToString(num, strbuf)));
    }

    void NumberConversion::WriteU64Binary(StringWriter sw, u64 num, u32 bitwidth) {
        char strbuf[u64s::BITS] = {};
        u32 i = u64s::BITS - 8;
        while (num) {
            u64 x = ((num & 0x55) * 0x02040810204081) | ((num & 0xAA) * 0x02040810204081);
            x &= 0x0101010101010101;
            x |= 0x3030303030303030;
            Memory::WriteU64(x, &strbuf[i]);
            i -= 8;
            num >>= 8;
        }
        sw.Write(Str::Slice(strbuf + u64s::BITS - bitwidth, bitwidth));
    }

    void NumberConversion::WriteU64Octal(StringWriter sw, u64 num, u32 octalDigits) {
        char strbuf[24] = {};
        u32 i = sizeof(strbuf) - 4;
        while (num) {
            u32 x = (num & 7) |
                    (num & 070) << 5 |
                    (num & 0700) << 10 |
                    (num & 07000) << 15;
            x |= 0x30303030;
            Memory::WriteU32(x, &strbuf[i]);
            i -= 4;
            num >>= 4 * 3;
        }
        sw.Write(Str::Slice(strbuf + 24 - octalDigits, octalDigits));
    }

    void NumberConversion::WriteU64Hex(StringWriter sw, u64 num, u32 hexDigits, bool upperCase) {
        char strbuf[u64s::HEX_DIGITS] = {};
        for (u32 i = 0; i < 2; ++i) { // repeat twice
            u64 x = num & 0xFFFF'FFFF;
            // x = 00 00 00 00 ab cd ef gh
            x |= x << 16;
            // x = 00 00 ab cd ?? ?? ef gh
            x = (x << 8) & 0x00FF0000'00FF0000 | // 00 ab 00 00 00 ef 00 00
                (x       & 0x000000FF'000000FF); // 00 00 00 cd 00 00 00 gh
            // 00 ab 00 cd 00 ef 00 gh
            x |= x << 4;
            // 0a ?b 0c ?d 0e ?f 0g ?h
            x &= 0x0F0F0F0F'0F0F0F0F;

            // if each hex digit of x is represented by a letter (aka >= 10)
            x += 0x06060606'06060606;
            x += ((x >> 4) & 0x01010101'01010101) * (upperCase ? 7 : 39);
            x += 0x2A2A2A2A'2A2A2A2A;
            Memory::WriteU64(x, &strbuf[(1 - i) * 8]);

            x >>= 32;
            if (!x) break;
        }
        sw.Write(Str::Slice(strbuf + (u64s::BITS / 4) - hexDigits, hexDigits));
    }

    char* NumberConversion::AddSign(f64 f, char* out, bool alwaysShowSign) {
        if (f < 0) {
            *out++ = '-';
            return out;
        }
        if (alwaysShowSign) {
            *out++ = '+';
            return out;
        }
        return out;
    }

    char* NumberConversion::WriteFltDecimal(f64 f, char* out, u32 precision) {
        if (precision == ~0) {
            // builtin 3-digit max precision
            u32 dig3 = f64s::FastToIntUnsigned(f * 1000);
            if (dig3 == 0) {
                return out - 1;
            } else {
                dig3 = U64ToBCD4(dig3);
                const u32 empty = u32s::CountRightZeros(dig3) / 8;
                dig3 <<= 8;
                dig3 |= 0x30'30'30'00;
                Memory::WriteU32(dig3, out);
                out += 3 - empty;
            }
        } else {
            u32 p = 0;
            for (; p < precision; p += 2) {
                f64 dig2;
                f64s::SeparateDecimal(f * 100, dig2, f);

                u32 i = (u32)f64s::FloorToIntUnsigned(f);
                i = U64ToBCD2(i);
                Memory::WriteU16(i | 0x3030, out);
                out += 2;
            }
            if (p < precision) {
                *out++ = (char)(0x30 | f64s::FloorToIntUnsigned(f * 10));
            }
        }
        return out;
    }

    u32 NumberConversion::WriteFltSci(f64 f, char* out, u32 precision, char e, int log10) {
        const char* begin = out;
        if (f == 0) {
            Memory::WriteU16("0."_u16, out);
            out += 2;
            if (precision == ~0)
                return 1;
            while (precision --> 0)
                *out++ = '0';
            return out - begin;
        }

        f64 mant, mantDec;

        if (log10 == i32s::MIN) {
            f64 ilog;
            f64s::SeparateDecimal(f64s::Log10(f), ilog, mant);
            mant = f64s::Exp10(mant);
        } else {
            mant = f * f64s::Exp10(-log10);
        }
        f64s::SeparateDecimal(mant, mant, mantDec);

        // *out++ = '0' + (char);
        // *out++ = '.';
        Memory::WriteU16((f64s::FastToIntUnsigned(mant) << 8) | "0."_u16, out);
        out += 2;

        out = WriteFltDecimal(mantDec, out, precision);
        // ...E+... or ...E-...
        Memory::WriteU16Little((log10 < 0 ? '-' : '+') << 8 | e, out);
        out += 2;
        log10 = std::abs(log10);

        {
            u32 eStr = U64ToBCD4(log10);
            const u32 empty = u32s::CountLeftZeros(eStr) / 8;
            eStr <<= empty * 8;
            eStr |= 0x30303030;
            Memory::MemCopyNoOverlap(out, (const char*)&eStr, 4 - empty);
            out += 4 - empty;
        }

        return out - begin;
    }

    u32 NumberConversion::WriteFltFxd(f64 f, char* out, u32 width, u32 precision, int log10, char pad) {
        const char* begin = out;
        if (f == 0) {
            const u32 w = width ? width : 1;
            Memory::RangeSet(out, pad, w);
            if (precision == ~0) {
                return w;
            }

            out += w;
            *out++ = '.';

            Memory::RangeSet(out, pad, precision);
            return w + 1 + precision;
        } else if (f < 1) { // log will be negative
            const u32 w = width ? width : 1;
            Memory::RangeSet(out, pad, w);
            out += w;
            *out++ = '.';
        } else {
            log10 = (log10 == i32s::MIN ? (int)f64s::FloorToIntUnsigned(f64s::Log10(f)) : log10) + 1;

            if (log10 < width) {
                Memory::RangeSet(out, pad, width - log10);
                out += width - log10;
            }

            if (log10 > 4) {
                f64 frac;
                f *= f64s::Exp10(-f64s::FastToFloatUnsigned(log10 - 4));

                for (; log10 > 4; log10 -= 4) {
                    f64s::SeparateDecimal(f, f, frac);

                    u32 topDigits = f64s::FastToIntUnsigned(f);
                    topDigits = U64ToBCD4(topDigits);
                    topDigits |= 0x30303030;
                    Memory::WriteU32(topDigits, out);
                    out += 4;

                    f = frac * 10000;
                }
            }
            u32 d = f64s::FloorToIntUnsigned(f);
            d = U64ToBCD4(d);
            d |= 0x30303030;
            d <<= (-8 * log10) & 31;
            Memory::MemCopyNoOverlap(out, (const char*)&d, log10);
            out += log10;
            *out++ = '.';

            [[maybe_unused]] f64 ignore;
            f64s::SeparateDecimal(f, ignore, f);
        }

        out = WriteFltDecimal(f, out, precision);

        return out - begin;
    }

    u32 NumberConversion::WriteFltGen(f64 f, char* out, u32 width, u32 precision, char e) {
        const int log = (int)f64s::Log10(f);
        if (log > (int)width) {
            return WriteFltSci(f, out, precision, e);
        } else {
            return WriteFltFxd(f, out, width, precision, log);
        }
    }

    usize NumberConversion::FormatFltSci(StringWriter sw, f64 f, const FloatFormatOptions& options) {
        // -_.___E+____
        // break down:
        // sign (1)
        // first digit (1)
        // decimal point (1)
        // decimals (options.precision or 3 if none specified (~0)) -> add 4 for ~0
        // E+ (2)
        // exponent (max 4)
        char* strbuf = Memory::QAlloca$(char, 1 + 1 + 1 + (options.precision + 4) + 2 + 4);
        const usize len = WriteFltSci(
            std::abs(f),
            AddSign(f, strbuf, options.showSign),
            options.precision,
            options.mode == FloatFormatOptions::SCI_CAP ? 'E' : 'e'
        );
        return Formatter<Str>::FormatTo(sw,
            Str::Slice(strbuf, len),
            { options.totalLength, options.alignment, options.pad, false }
        );
    }

    usize NumberConversion::FormatFltFxd(StringWriter sw, f64 f, const FloatFormatOptions& options) {
        // -___.___
        // sign (1)
        // digits (max(log10, width))
        // decimal point (1)
        // decimals (~0 -> 3, anything else -> itself)
        const int log10 = (int)f64s::Log10(std::abs(f));
        const u32 w = log10 <= 0 ? 1 : (log10 + 1);
        char* strbuf = Memory::QAlloca$(char, 1 + std::max(w, options.width) + 1 + (options.precision + 4));
        const usize len = WriteFltFxd(
            std::abs(f),
            AddSign(f, strbuf, options.showSign),
            options.width,
            options.precision,
            log10,
            options.shouldPadZero ? '0' : ' '
        );
        return Formatter<Str>::FormatTo(sw,
            Str::Slice(strbuf, len),
            { options.totalLength, options.alignment, options.pad, false }
        );
    }

    usize NumberConversion::FormatFloating(StringWriter sw, f64 f, const FloatFormatOptions& options) {
        using enum FloatFormatOptions::Mode;

        static constexpr char NaNString[] = "NaN%",
                              InfString[] = "-Infinity%";

        if (f == Math::NaN) {
            return Formatter<Str>::FormatTo(sw,
                Str::Slice(NaNString, 3 + options.mode == PERCENTAGE),
                { options.totalLength, options.alignment, options.pad, false }
            );
        }
        if (f64s::IsInf(f)) {
            const bool isPositive = f > 0;
            return Formatter<Str>::FormatTo(sw,
                Str::Slice(InfString + isPositive, 9 - isPositive + options.mode == PERCENTAGE),
                { options.totalLength, options.alignment, options.pad, false }
            );
        }

        switch (options.mode) {
            case SCIENTIFIC:
            case SCI_CAP:
                return FormatFltSci(sw, f, options);
            case FIXED: {
                return FormatFltFxd(sw, f, options);
            }
            case GENERAL:
            case GEN_CAP: {
                if (std::abs(f) > f64s::Exp10(options.width) ||
                    std::abs(f) < f64s::Exp10(-options.precision)) {
                    FloatFormatOptions fopt = options;
                    fopt.mode = options.mode == GEN_CAP ? SCI_CAP : SCIENTIFIC;
                    return FormatFltSci(sw, f, fopt);
                } else {
                    return FormatFltFxd(sw, f, options);
                }
            }
            case PERCENTAGE: {
                return FormatFltFxd(sw, f * 100, options);
            }
            default: return 0;
        }
    }

    usize Formatter<bool>::FormatTo(StringWriter sw, bool input, const FormatOptions& options) {
        return Formatter<Str>::FormatTo(sw, input ? "true"_str : "false"_str, options);
    }

    IntFormatOptions IntFormatOptions::Configure(Str opt) {
        IntFormatOptions options;
        if (opt.Length() > 1 && opt[0] != '+') {
            if (opt[0] != '<' && opt[0] != '^' && opt[0] != '>') {
                options.pad = opt[0];
                opt.Advance(1);
            }
            if (opt[0] == '^') {
                options.alignment = TextFormatOptions::CENTER;
            } else {
                options.alignment = (TextFormatOptions::Alignment)(opt[0] - '<');
            }
            const auto [n, totalLen] = ParsePartial<u32>(opt.Tail());
            options.totalLength = totalLen.Assert();
            opt.Advance(*n + 1);
            if (opt.IsEmpty())
                return options;
            Debug::AssertEq(opt[0], ',');
            opt.Advance(1);
        }

        if (opt.StartsWith('+')) {
            options.showSign = true;
            opt.Advance(1);
        }
        if (opt.StartsWith('0')) {
            options.shouldPadZero = true;
            opt.Advance(1);
        }
        if (const auto i = "dboxX"_str.Find(opt.Last())) {
            options.base = (Base)*i;
            opt.Shorten(1);
        }
        options.numLen = Parse<u32>(opt).Assert();
        return options;
    }

    template <Integer N>
    usize Formatter<N>::FormatTo(StringWriter sw, N num, const FormatOptions& options) {
        if constexpr (Unsigned<N>)
            return NumberConversion::FormatU64(sw, (u64)num, options, options.showSign ? '+' : '\0');
        else
            return NumberConversion::FormatI64(sw, (i64)num, options);
    }

    FloatFormatOptions FloatFormatOptions::Configure(Str opt) {
        // ((?'pad'.?)(?'align'[<^>])(?'totalLen'[0-9]+)\,)?(?'showSign'\+?)(?'shouldPadZero'0?)(?'width'[0-9]*)\.(?'precision'[0-9]*)(?'mode'[feEgG%])
        FloatFormatOptions options;
        if (opt.Length() > 1 && opt[0] != '+') {
            if (opt[0] != '<' && opt[0] != '^' && opt[0] != '>') {
                options.pad = opt[0];
                opt.Advance(1);
            }
            if (opt[0] == '^') {
                options.alignment = TextFormatOptions::CENTER;
            } else {
                options.alignment = (TextFormatOptions::Alignment)(opt[0] - '<');
            }
            const auto [n, totalLen] = ParsePartial<u32>(opt.Tail());
            options.totalLength = totalLen.Assert();
            opt.Advance(*n + 1);
            if (opt.IsEmpty())
                return options;
            Debug::AssertEq(opt[0], ',');
            opt.Advance(1);
        }

        if (opt.StartsWith('+')) {
            options.showSign = true;
            opt.Advance(1);
        }
        if (opt.StartsWith('0')) {
            options.shouldPadZero = true;
            opt.Advance(1);
        }
        const auto [wlen, width] = ParsePartial<u32>(opt);
        if (options.shouldPadZero) {
            Debug::Assert(wlen, "float spec can't have 0 width");
        }
        options.width = width;

        Debug::AssertEq(opt[*wlen], '.');
        opt.Advance(*wlen + 1);

        if (const auto i = "efgEG%"_str.Find(opt.Last())) {
            options.mode = (Mode)*i;
            opt.Shorten(1);
        }
        options.precision = opt ? std::min(Parse<u32>(opt).Assert(), 32u) : ~0;
        return options;
    }

    template struct Formatter<bool>;

    template struct Formatter<u8>;
    template struct Formatter<i8>;
    template struct Formatter<u16>;
    template struct Formatter<i16>;
    template struct Formatter<u32>;
    template struct Formatter<i32>;
    template struct Formatter<u64>;
    template struct Formatter<i64>;

    template struct Formatter<f32>;
    template struct Formatter<f64>;

    OptionUsize Parser<bool>::ParseUntil(Str string, Out<bool&> out, ParseOptions options) {
        if (options.format & BooleanParseOptions::ALLOW_NUMERIC) {
            if (!string) return nullptr;
            const char first = string.First();
            if (first == '0' || first == '1') {
                out = first == '1';
                return 1;
            }
            return nullptr;
        }
        if (options.format & BooleanParseOptions::USE_WORDS) {
            if (string.StartsWith("true"))  { out = true;  return 4; }
            if (string.StartsWith("false")) { out = false; return 5; }
        }
        return nullptr;
    }

    template <Integer N>
    OptionUsize Parser<N>::ParseUntil(Str string, Out<N&> out, ParseOptions options) {
        return NumberConversion::ParseInt(string, out, options);
    }

    template <Floating N>
    OptionUsize Parser<N>::ParseUntil(Str string, Out<N&> out, ParseOptions options) {
        const bool isFixed = (options.format & FloatParseOptions::FIXED),
                   isSci   = (options.format & FloatParseOptions::SCIENTIFIC);

        bool negative = false;
        if (string.StartsWith('+')) string.Advance(1);
        if (string.StartsWith('-')) { negative = true; string.Advance(1); }

        N num;
        const usize integerPart = ParseInteger(string, num);

        if (string.Length() == integerPart) {
            if (isSci)
                return nullptr;
            out = negative ? -num : num;
            return integerPart;
        }

        usize decimalPart = 0;
        if (string[integerPart] == '.') {
            decimalPart = ParseDecimal(string.Skip(integerPart + 1), num);
        }

        const usize fixedPart = integerPart + decimalPart + 1;

        if (fixedPart == string.Length()) {
            return string.Length();
        }

        if (Chr::ToUpper(string[fixedPart]) == 'E' && isSci) {
            if (decimalPart == 0) return nullptr;
            const Str expStr = string.Skip(fixedPart + 1);
            const OptionUsize expPart = ParseExponent(expStr, num);
            if (!expPart) return nullptr;

            out = negative ? -num : num;
            return fixedPart + 1 + *expPart;
        }

        if (isFixed) return nullptr;

        out = negative ? -num : num;
        return fixedPart;
    }

    template <Floating N>
    usize Parser<N>::ParseInteger(Str string, Out<N&> out) {
        string = string.First(string.FindIf(Qfn$(!Chr::IsDigit)));
        const usize totalLen = string.Length();
        string = string.TrimStart('0');

        if (string.Length() > NumInfo<N>::MAX_EXP10) {
            out = Math::Infinity;
            return totalLen;
        }

        N num = 0;
        usize i = 0;
        for (; i < string.Length() & ~3; i += 4) {
            u32 dig = Memory::ReadU32Little(string.Data() + i);
            dig = NumberConversion::ParseDigits4(dig);
            num = num * N { 10'000 } + dig;
        }
        for (; i < string.Length(); ++i) {
            num = num * N { 10 } + Chr::ToDigit(string[i]);
        }
        out = num;
        return totalLen;
    }

    template <Floating N>
    usize Parser<N>::ParseDecimal(Str string, InOut<N&> out) {
        string = string.First(string.FindIf(Qfn$(!Chr::IsDigit)));
        const usize totalLen = string.Length();
        string = string.TrimEnd('0');

        N decimal = 1.0f;
        usize i = 0;
        for (; i < string.Length() & ~3; i += 4) {
            u32 dig = Memory::ReadU32Little(string.Data() + i);
            dig = NumberConversion::ParseDigits4(dig);
            decimal *= N { 0.0001 };
            const N newOut = out + decimal * dig;
            if (newOut == out) return totalLen;
            out = newOut;
        }
        for (; i < string.Length(); ++i) {
            decimal *= N { 0.1 };
            const N newOut = out + decimal * Chr::ToDigit(string[i]);
            if (newOut == out) return totalLen;
            out = newOut;
        }
        return totalLen;
    }

    template <Floating N>
    OptionUsize Parser<N>::ParseExponent(Str string, InOut<N&> out) {
        if (!string) return nullptr;

        bool negExp = false;
        if (string.StartsWith('+')) string.Advance(1);
        if (string.StartsWith('-')) { negExp = true; string.Advance(1); }
        if (!string) return nullptr;

        i32 exp = 0;
        usize i = 0;

        for (; i < string.Length(); ++i) {
            if (const Option d = Chr::TryToDigit(string[i])) {
                if (exp < 10000) exp = exp * 10 + (i32)*d;
            } else { out *= std::exp2(Math::LOG10_2 * (negExp ? -(float)exp : (float)exp)); return i; }
        }

        out *= std::exp2(Math::LOG10_2 * (negExp ? -(float)exp : (float)exp));
        return i;
    }

    template struct Parser<bool>;

    template struct Parser<u8>;
    template struct Parser<i8>;
    template struct Parser<u16>;
    template struct Parser<i16>;
    template struct Parser<u32>;
    template struct Parser<i32>;
    template struct Parser<u64>;
    template struct Parser<i64>;

    template struct Parser<f32>;
    template struct Parser<f64>;
}
