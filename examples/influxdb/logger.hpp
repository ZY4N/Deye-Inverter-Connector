#pragma once

#include <iostream>

namespace logger {
    enum class level {
        ERROR = 0, WARN = 1, INFO = 2, LOG = 3, DEBUG = 4
    };
    
    constexpr const char *level_name(level lvl) {
        switch (lvl) {
            case level::ERROR: return "error";
            case level::WARN : return "warn";
            case level::INFO : return "info";
            case level::LOG  : return "log";
            case level::DEBUG: return "debug";
            default: return nullptr;
        }
    }
    
    inline static level lvl = level::DEBUG;
    
    static inline void write(const level required_level, const auto &msg, const auto&... tags) {
        if (lvl >= required_level) {
            ((std::cout << '[' << tags << ']'), ...);
            std::cout << ": " << msg << std::endl;
        }
    }
    static inline void debug(const auto &msg, const auto&... tags) {
        write(level::DEBUG, msg, "\u001b[35mdebug\u001b[0m", tags...);
    }
    static inline void log(const auto &msg, const auto&... tags) {
        write(level::LOG, msg, "\u001b[34;1mlog\u001b[0m", tags...);
    }
    static inline void info(const auto &msg, const auto&... tags) {
        write(level::INFO, msg, "\u001b[32;1minfo\u001b[0m", tags...);
    }
    static inline void warn(const auto &msg, const auto&... tags) {
        write(level::WARN, msg, "\u001b[33;1mwarn\u001b[0m", tags...);
    }
    static inline void error(const std::error_code &e, const auto&... tags) {
        if (e) {
            write(level::ERROR, e.message(), "\u001b[31;1merror\u001b[0m", tags..., e.category().name());
        }
    }
}
