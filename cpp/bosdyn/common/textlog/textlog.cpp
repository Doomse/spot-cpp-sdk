/**
 * Copyright (c) 2023 Boston Dynamics, Inc.  All rights reserved.
 *
 * Downloading, reproducing, distributing or otherwise using the SDK Software
 * is subject to the terms and conditions of the Boston Dynamics Software
 * Development Kit License (20191101-BDSDK-SL).
 */


#include "textlog.h"

#include <cstdio>

#include <cstdlib>

namespace bosdyn {
namespace common {
namespace textlog {

// This is the one overridable message-handler.
void TextLog::LogFrom(Level level, const char* filename BOSDYN_UNUSED,
                      int line_number BOSDYN_UNUSED, const char* msg) {
    if (!ShouldLog(level)) {
        if (level == kFatal) {
            std::abort();
        }
        return;
    }
    if (filename) {
        fprintf(stderr, "%s:%d  %s\n", filename, line_number, msg);
    } else {
        fprintf(stderr, "%s\n", msg);
    }
    if (level == kFatal) {
        std::abort();
    }
}

void TextLog::LogvFrom(Level level, const char* filename, int line_number, const char* format,
                       va_list valist) {
    if (!ShouldLog(level)) {
        return;
    }
    char msg[Constants::kMaxMessageLength];
    vsnprintf(msg, sizeof(msg) - 1, format, valist);
    LogFrom(level, filename, line_number, msg);
}

void TextLog::Log(Level level, const char* msg) { LogFrom(level, nullptr, -1, msg); }

void TextLog::Logf(Level level, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(level, format, ap);
    va_end(ap);
}

void TextLog::LogfFrom(Level level, const char* filename, int line_number, const char* format,
                       ...) {
    va_list ap;
    va_start(ap, format);
    LogvFrom(level, filename, line_number, format, ap);
    va_end(ap);
}

void TextLog::Logv(Level level, const char* format, va_list args) {
    LogvFrom(level, nullptr, -1, format, args);
}

void TextLog::Ignoref(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(kIgnore, format, ap);
    va_end(ap);
}
void TextLog::Debugf(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(kDebug, format, ap);
    va_end(ap);
}
void TextLog::Infof(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(kInfo, format, ap);
    va_end(ap);
}
void TextLog::Warnf(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(kWarn, format, ap);
    va_end(ap);
}
void TextLog::Errorf(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(kError, format, ap);
    va_end(ap);
}
void TextLog::Fatalf(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    Logv(kFatal, format, ap);
    va_end(ap);
}

}  // namespace textlog
}  // namespace common
}  // namespace bosdyn
