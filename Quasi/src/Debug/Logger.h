#pragma once
#include <chrono>
#include <iostream>
#include <source_location>

#include "ConsoleColor.h"
#include "Text.h"
#include "Timer.h"
#include "Utils/Enum.h"

namespace Quasi::Debug {
    void DebugBreak();

    struct SeverityData {
        Str name;
        ConsoleColor color;

        ColoredText ColoredName() const { return { color, name }; }

        QDefineEnum$(Severity,
            (OFF,      ("OFF",      ConsoleColor::RESET))
            (TRACE,    ("TRACE",    FgColor(ConsoleColor::GREEN)))
            (DEBUG,    ("DEBUG",    FgColor(ConsoleColor::CYAN)))
            (INFO,     ("INFO",     ConsoleColor::RESET))
            (WARN,     ("WARN",     FgColor(ConsoleColor::YELLOW)))
            (ERROR,    ("ERROR",    FgColor(ConsoleColor::RED)))
            (CRITICAL, ("CRITICAL", Bold(ConsoleColor::FG_RED))),
        COMPARABLE, ("NEVER", ConsoleColor::GRAY))
    };

    using SourceLoc = std::source_location;

    // captures source location
    struct FmtStr {
        Str fmt;
        SourceLoc loc;
        FmtStr(const char* f, const SourceLoc& l = SourceLoc::current()) : fmt(f), loc(l) {}
        FmtStr(Str f, const SourceLoc& l = SourceLoc::current()) : fmt(f), loc(l) {}
        // consteval FmtStr(const char* f, const SourceLoc& l = SourceLoc::current()) : fmt(f), loc(l) {}
    };

    struct LogEntry {
        String log;
        Severity severity;
        DateTime time;
        SourceLoc fileLoc;
    };

    class Logger {
        Ref<OutStream> logOut;
        ColoredText name = { RESET, "LOG" };
        Severity filterLevel = Severity::OFF, breakLevel = Severity::ERROR;
        Vec<LogEntry> logs;

        bool shortenFileNames : 1 = true;
        bool includeFunction : 1 = true;
        bool alwaysFlush : 1 = false;
        bool recordLogs : 1 = false;
        int lPad = 50;

    public:
        static Logger InternalLog;
#ifdef NDEBUG
        static constexpr bool DEBUG = false;
#else
        static constexpr bool DEBUG = true;
#endif
        explicit Logger(OutStream& out = std::cout) : logOut(out) {}

        static bool Overrides(Severity filter, Severity log) { return log >= filter; }
        bool Overrides(Severity s) const { return Overrides(filterLevel, s); }
        void SetFilter(Severity s) { filterLevel = s; }
        void SetBreakLevel(Severity s) { breakLevel = s; }

        void SetName(Str newName) { name.string = newName; }
        void SetNameColor(const ConsoleColor col) { name.color = col; }
        void SetShortenFile(const bool flag) { shortenFileNames = flag; }
        void SetIncludeFunc(const bool flag) { includeFunction = flag; }
        void SetAlwaysFlush(const bool flag) { alwaysFlush = flag; }
        void SetRecordLogs(const bool flag) { recordLogs = flag; }
        void SetLocPad(const int pad) { lPad = pad; }

        String FmtLog(const LogEntry& log) const;
        String FmtLog(Str log, Severity severity, DateTime time, const SourceLoc& fileLoc) const;
        Str FmtFile(Str fullname) const;
        String FmtSourceLoc(const SourceLoc& loc) const;
        void LogNoOut  (Severity sv, Str s, const SourceLoc& loc = SourceLoc::current());
        void ConsoleLog(Severity sv, Str s, const SourceLoc& loc = SourceLoc::current());
        void Log       (Severity sv, Str s, const SourceLoc& loc = SourceLoc::current());

        void AssertMsg(bool assert, Str msg, const SourceLoc& loc = SourceLoc::current());

        void Write(OutStream& out, Severity filter = Severity::NONE);

        template <class ...Ts> void LogFmt(Severity s, const FmtStr& fmt, Ts&&... args) {
            this->Log(s, Text::Format(fmt.fmt, std::forward<Ts>(args)...), fmt.loc);
        }

        template <class ...Ts> void Assert(bool assert, const FmtStr& fmt, Ts&&... args) {
            if (assert) return;
            this->AssertMsg(true, Text::Format(fmt.fmt, std::forward<Ts>(args)...), fmt.loc);
        }

        template <class T>
        void AssertEq(const T& val, const T& cmp, const SourceLoc& loc = SourceLoc::current()) {
            this->Assert(val == cmp,
                { "Left operand {0}({1}) is not equal to Right Operand {0}({2})", loc },
                Text::TypeName<T>(), val, cmp
            );
        }

        template <class T>
        void AssertNeq(const T& val, const T& cmp, const SourceLoc& loc = SourceLoc::current()) {
            this->Assert(val != cmp,
                { "Left operand {0}({1}) is equal to Right Operand {0}({2})", loc },
                Text::TypeName<T>(), val, cmp
            );
        }

        template <class ...Ts> void Trace   (const FmtStr& fmt, Ts&&... args) { this->LogFmt(Severity::TRACE,    fmt, std::forward<Ts>(args)...); }
        template <class ...Ts> void Debug   (const FmtStr& fmt, Ts&&... args) { this->LogFmt(Severity::DEBUG,    fmt, std::forward<Ts>(args)...); }
        template <class ...Ts> void Info    (const FmtStr& fmt, Ts&&... args) { this->LogFmt(Severity::INFO,     fmt, std::forward<Ts>(args)...); }
        template <class ...Ts> void Warn    (const FmtStr& fmt, Ts&&... args) { this->LogFmt(Severity::WARN,     fmt, std::forward<Ts>(args)...); }
        template <class ...Ts> void Error   (const FmtStr& fmt, Ts&&... args) { this->LogFmt(Severity::ERROR,    fmt, std::forward<Ts>(args)...); }
        template <class ...Ts> void Critical(const FmtStr& fmt, Ts&&... args) { this->LogFmt(Severity::CRITICAL, fmt, std::forward<Ts>(args)...); }

        void Flush();
        void NoOp() const {}

        static Logger& GetInternalLog();
    };

    inline void SetFilter(Severity s) { Logger::GetInternalLog().SetFilter(s); }
    inline void SetBreakLevel(Severity s) { Logger::GetInternalLog().SetBreakLevel(s); }

    inline void SetName(std::string newName) { Logger::GetInternalLog().SetName(std::move(newName)); }
    inline void SetNameColor(const ConsoleColor col) { Logger::GetInternalLog().SetNameColor(col); }
    inline void SetShortenFile(const bool flag) { Logger::GetInternalLog().SetShortenFile(flag); }
    inline void SetIncludeFunc(const bool flag) { Logger::GetInternalLog().SetIncludeFunc(flag); }
    inline void SetLocPad(const int pad) { Logger::GetInternalLog().SetLocPad(pad); }

    inline void Log(Severity sv, Str s, const SourceLoc& loc = SourceLoc::current()) { Logger::GetInternalLog().Log(sv, s, loc); }

    inline void AssertMsg(bool assert, Str msg, const SourceLoc& loc = SourceLoc::current()) { Logger::GetInternalLog().AssertMsg(assert, msg, loc); }

    inline void Write(OutStream& out, Severity filter = Severity::NONE) { Logger::GetInternalLog().Write(out, filter); }

    template <class ...Ts> void LogFmt(Severity s, const FmtStr& fmt, Ts&&... args) {
        Logger::GetInternalLog().LogFmt(s, fmt, std::forward<Ts>(args)...);
    }

    template <class ...Ts> void Assert(bool assert, const FmtStr& fmt, Ts&&... args) {
        Logger::GetInternalLog().Assert(assert, fmt.fmt, std::forward<Ts>(args)...);
    }

    template <class T>
    void AssertEq(const T& val, const T& cmp, const SourceLoc& loc = SourceLoc::current()) { Logger::GetInternalLog().AssertEq(val, cmp, loc); }

    template <class T>
    void AssertNeq(const T& val, const T& cmp, const SourceLoc& loc = SourceLoc::current()) { Logger::GetInternalLog().AssertNeq(val, cmp, loc); }

    template <class ...Ts> void Trace   (const FmtStr& fmt, Ts&&... args) { Logger::GetInternalLog().Trace   (fmt, std::forward<Ts>(args)...); }
    template <class ...Ts> void Debug   (const FmtStr& fmt, Ts&&... args) { Logger::GetInternalLog().Debug   (fmt, std::forward<Ts>(args)...); }
    template <class ...Ts> void Info    (const FmtStr& fmt, Ts&&... args) { Logger::GetInternalLog().Info    (fmt, std::forward<Ts>(args)...); }
    template <class ...Ts> void Warn    (const FmtStr& fmt, Ts&&... args) { Logger::GetInternalLog().Warn    (fmt, std::forward<Ts>(args)...); }
    template <class ...Ts> void Error   (const FmtStr& fmt, Ts&&... args) { Logger::GetInternalLog().Error   (fmt, std::forward<Ts>(args)...); }
    template <class ...Ts> void Critical(const FmtStr& fmt, Ts&&... args) { Logger::GetInternalLog().Critical(fmt, std::forward<Ts>(args)...); }
    inline void NoOp() {}

#ifdef NDEBUG
    #define QTrace$    NoOp() Q_EAT
    #define QDebug$    NoOp() Q_EAT
    #define QInfo$     NoOp() Q_EAT
    #define QWarn$     NoOp() Q_EAT
    #define QError$    NoOp() Q_EAT
    #define QCritical$ NoOp() Q_EAT
#else
    #define QTrace$    Trace
    #define QDebug$    Debug
    #define QInfo$     Info
    #define QWarn$     Warn
    #define QError$    Error
    #define QCritical$ Critical
#endif

    void Flush();
}

namespace Quasi {
    template <class T, class Super> T& INullable<T, Super>::Assert() { return QGetterMut$(Assert); }
    template <class T, class Super> const T& INullable<T, Super>::Assert() const {
        Debug::Assert(HasValue(), "{} doesn't have a value", Text::TypeName<Super>());
        return Unwrap();
    }

    template <class T, class Super> T& INullable<T, Super>::Assert(Str msg) { return QGetterMut$(Assert, msg); }
    template <class T, class Super> const T& INullable<T, Super>::Assert(Str msg) const {
        Debug::Assert(HasValue(), msg);
        return Unwrap();
    }

    template <class T, class Super> T& INullable<T, Super>::Assert(auto&& assertfn) { return QGetterMut$(Assert, assertfn); }
    template <class T, class Super> const T& INullable<T, Super>::Assert(auto&& assertfn) const {
        if (IsNull()) assertfn();
        return Unwrap();
    }
}

namespace Quasi::Text {
    template <>
    struct Formatter<Debug::DateTime> {
        Str fmt;
        bool AddOption(Str args) {
            for (u32 i = 0; i < args.size(); ++i) {
                if (args[i] == '%' && args[i + 1] != '%') {
                    switch (args[i + 1]) {
                        case 'y':
                        case 'M': case 'N': case 'n':
                        case 'd': case 'a':
                        case 'h': case 'H': case 'g':
                        case 'm':
                        case 's': case 'u': break;
                        default: return false;
                    }
                }
            }
            fmt = args;
            return true;
        }
        void FormatTo(const Debug::DateTime& time, StringOutput output) {
            const auto d = floor<std::chrono::days>(time);
            const std::chrono::year_month_day ymd = d;
            const std::chrono::hh_mm_ss       hms { floor<std::chrono::milliseconds>(time - d) };
            constexpr Str MONTH_NAMES[] = {
                "January",
                "Febuary",
                "March",
                "April",
                "May",
                "June",
                "July",
                "August",
                "September",
                "October",
                "November",
                "December",
            };
            constexpr Str WEEKDAY_NAMES[] = {
                "Monday",
                "Tuesday",
                "Wednesday",
                "Thurday",
                "Friday",
                "Satarday",
                "Sunday",
            };
            using std::chrono::operator ""h;
            for (u32 i = 0; i < fmt.size(); ++i) {
                if (fmt[i] != '%') {
                    output(fmt[i]);
                    continue;
                }
                switch (fmt[i + 1]) {
                    case '%': output('%'); ++i; continue;
                    case 'y': FormatOnto(output, (i32)ymd.year());                   ++i; continue;
                    case 'M': FormatOnto(output, "{:02}", (u32)ymd.month());         ++i; continue;
                    case 'N': output(MONTH_NAMES[(u32)ymd.month()]);                 ++i; continue;
                    case 'n': output(MONTH_NAMES[(u32)ymd.month()].substr(0, 3));    ++i; continue;
                    case 'd': FormatOnto(output, "{:02}", (u32)ymd.day());           ++i; continue;
                    case 'A': output(WEEKDAY_NAMES[(u32)ymd.day()]);                 ++i; continue;
                    case 'a': output(WEEKDAY_NAMES[(u32)ymd.day()].substr(0, 3));    ++i; continue;
                    case 'h': FormatOnto(output, "{:02}", hms.hours().count() % 12); ++i; continue;
                    case 'H': FormatOnto(output, "{:02}", hms.hours().count());      ++i; continue;
                    case 'g': output(hms.hours() >= 12h ? "PM" : "AM");              ++i; continue;
                    case 'm': FormatOnto(output, "{:02}", hms.minutes().count());    ++i; continue;
                    case 's': FormatOnto(output, "{:02}", hms.seconds().count());    ++i; continue;
                    case 'u': FormatOnto(output, "{:03}", hms.subseconds().count()); ++i; continue;
                    default:;
                }
            }
        }
    };
}