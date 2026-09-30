/**
 * Copyright (c) 2023 Boston Dynamics, Inc.  All rights reserved.
 *
 * Downloading, reproducing, distributing or otherwise using the SDK Software
 * is subject to the terms and conditions of the Boston Dynamics Software
 * Development Kit License (20191101-BDSDK-SL).
 */


// Boston Dynamics, Inc. Confidential Information.
// Copyright 2025. All Rights Reserved.
#pragma once

/// This is a minimalistic logging API meant to be easily back-ended by whatever logging system
///  is used by the codebase integrating the SDK code.

#include <cstdarg>

#include <atomic>

#include "bosdyn/common/compiler_definitions.h"

namespace bosdyn {
namespace common {
namespace textlog {

/// Importance level of logged messages.
enum Level : int {
    kUndef = -1,
    kAlways = 0,
    kFatal = 1,
    kError = 2,
    kWarn = 3,
    kInfo = 4,
    kDebug = 5,
    kIgnore = 6,
};

enum Constants : int {
    kMaxMessageLength = 2048,  // Buffer size used to render formatted log messages.
};

/// An interface to which different backend logging systems might be attached.
/// By default, all it does is print log messages to stderr.
class TextLog {
 public:
    virtual ~TextLog() = default;

    /// The level of a message compared to the current level of the TextLog object determines
    ///  whether the log message will be printed.
    virtual void SetLevel(Level level) { m_level = level; }
    virtual bool ShouldLog(Level level) { return level <= m_level; }

    /// The default implementation prints msg, with a trailing newline, if ShouldLog().
    virtual void LogFrom(Level level, const char* filename, int line_number, const char* msg);
    void LogvFrom(Level level, const char* filename, int line_number, const char* format,
                  va_list valist);
    void LogfFrom(Level level, const char* filename, int line_number, const char* format, ...)
        BOSDYN_CHECK_PRINTF_FORMAT(5);

    void Log(Level level, const char* msg);
    void Logf(Level level, const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(3);
    void Logv(Level level, const char* format, va_list valist);

    void Ignore(const char* msg) { Log(kIgnore, msg); }
    void Ignoref(const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(2);
    void Debug(const char* msg) { Log(kDebug, msg); }
    void Debugf(const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(2);
    void Info(const char* msg) { Log(kInfo, msg); }
    void Infof(const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(2);
    void Warn(const char* msg) { Log(kWarn, msg); }
    void Warnf(const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(2);
    void Error(const char* msg) { Log(kError, msg); }
    void Errorf(const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(2);
    void Fatal(const char* msg) { Log(kFatal, msg); }
    void Fatalf(const char* format, ...) BOSDYN_CHECK_PRINTF_FORMAT(2);

 private:
    std::atomic<Level> m_level = Level::kInfo;
};

// calling this from BOS_FATAL makes static analyzers understand that those are assertions.
[[noreturn]] inline void NoreturnHelperFunction() {
#ifdef __GNUC__
    __builtin_unreachable();
#endif
}

#ifdef __clang__
#    define BOSDYN_PRAGMA_PUSH _Pragma("clang diagnostic push")
#    define BOSDYN_PRAGMA_POP _Pragma("clang diagnostic pop")
#    define BOSDYN_PRAGMA_IGNORED_NONNULL _Pragma("clang diagnostic ignored \"-Wnonnull\"")
#elif defined(__GNUC__)
#    define BOSDYN_PRAGMA_PUSH _Pragma("GCC diagnostic push")
#    define BOSDYN_PRAGMA_POP _Pragma("GCC diagnostic pop")
#    define BOSDYN_PRAGMA_IGNORED_NONNULL _Pragma("GCC diagnostic ignored \"-Wnonnull-compare\"")
#else
#    define BOSDYN_PRAGMA_PUSH
#    define BOSDYN_PRAGMA_POP
#    define BOSDYN_PRAGMA_IGNORED_NONNULL
#endif

#define BOSDYN_FATAL(LOGGER, ...)                                                               \
    do {                                                                                        \
        (LOGGER)->LogfFrom(::bosdyn::common::textlog::kFatal, __FILE__, __LINE__, __VA_ARGS__); \
        ::bosdyn::common::textlog::NoreturnHelperFunction();                                    \
    } while (false)

// clang-format off
#define BOSDYN_CHECK_FATAL(LOGGER, CHECK, ...)  \
    do {                                        \
        BOSDYN_PRAGMA_PUSH                      \
        BOSDYN_PRAGMA_IGNORED_NONNULL           \
        if (!(CHECK)) {                         \
            BOSDYN_FATAL(LOGGER, __VA_ARGS__);  \
        }                                       \
        BOSDYN_PRAGMA_POP                       \
    } while (false)

#define BOSDYN_ASSERT(LOGGER, CHECK)                              \
    do {                                                          \
        BOSDYN_PRAGMA_PUSH                                        \
        BOSDYN_PRAGMA_IGNORED_NONNULL                             \
        if (!(CHECK)) {                                           \
            BOSDYN_FATAL(LOGGER, "Failed assertion: %s", #CHECK); \
        }                                                         \
        BOSDYN_PRAGMA_POP                                         \
    } while (false)
// clang-format on

#define BOSDYN_ASSERT_NOT_REACHED(LOGGER)                                  \
    do {                                                                   \
        BOSDYN_FATAL(LOGGER, "This code location should not be reached."); \
    } while (false)

#define BOSDYN_ERROR(LOGGER, ...) \
    (LOGGER)->LogfFrom(::bosdyn::common::textlog::kError, __FILE__, __LINE__, __VA_ARGS__)

#define BOSDYN_WARN(LOGGER, ...) \
    (LOGGER)->LogfFrom(::bosdyn::common::textlog::kWarn, __FILE__, __LINE__, __VA_ARGS__)

#define BOSDYN_INFO(LOGGER, ...) \
    (LOGGER)->LogfFrom(::bosdyn::common::textlog::kInfo, __FILE__, __LINE__, __VA_ARGS__)

}  // namespace textlog
}  // namespace common
}  // namespace bosdyn
