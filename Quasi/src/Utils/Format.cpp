// #include "Format.h"
//
// #include "Logger.h"
//
// namespace Quasi::Text {
//     FormatString FormatString::From(Str fstr) {
//         FormatString f = { .rawstr = fstr };
//         for (u32 i = 0; i < fstr.size(); ++i) {
//             const char c = fstr[i];
//             if (c != '{') continue;
//             if (i + 1 < fstr.size() && fstr[i + 1] == '{') {
//                 f.brackSkips.Push(i);
//                 continue;
//             }
//
//             for (u32 j = i + 1; j < fstr.size(); ++j) {
//                 const char jc = fstr[j];
//                 if (jc != '}') continue;
//                 if (j + 1 < fstr.size() && fstr[j + 1] == '}') {
//                     f.brackSkips.Push(j);
//                     continue;
//                 }
//
//                 Str subsyn = fstr.substr(i + 1, j - i - 1);
//                 f.subs.Push({});
//                 auto& sb = f.subs.LastMut();
//                 sb.position = i;
//                 sb.skipPosition = j;
//                 sb.index = f.subs.Length() - 1;
//                 if (subsyn.empty()) break;
//                 if (const usize sep = subsyn.find(':'); sep != Str::npos) {
//                     const Str iStr = subsyn.substr(0, sep),
//                               sStr = subsyn.substr(sep + 1);
//                     if (!iStr.empty()) {
//                         sb.index = Text::Parse<u32>(iStr).Assert();
//                     }
//                     sb.specifier = sStr;
//                     break;
//                 }
//                 sb.index = Text::Parse<u32>(subsyn).Assert();
//             }
//         }
//         return f;
//     }
//
//     void CheckFormatOption(Str tname, bool success, Str spec) {
//         // if (!success) {
//         //     String message = "format specifier for <";
//         //     message += tname;
//         //     message += "> failed ('";
//         //     message += spec;
//         //     message += "')";
//         //     Debug::Log(Debug::Severity::ERROR, message);
//         // }
//         Debug::Assert(success,
//             "format specifier for <{}> failed ('{}')", tname, spec);
//     }
// }
