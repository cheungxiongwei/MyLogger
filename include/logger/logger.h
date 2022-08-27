#ifndef MYLOGGER_LOGGER_H
#define MYLOGGER_LOGGER_H

#include <memory>
#include "source_location.hpp"

class Logger {
public:
    enum class Level{
        None,
        Log,
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
        Verbose
    };
    Logger();;

    ~Logger();

    auto instance() -> Logger &;

    auto init(Logger::Level level,const char *file_name,size_t max_file_size) -> Logger &;

    template<typename... Args>
    auto println(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt::format_string{"{}\n",fmt.location,fmt.timestamp},std::make_format_args(std::vformat(fmt.str,std::make_format_args(args...))),Level::None);
    }

    template<typename... Args>
    auto log(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt,std::make_format_args(args...),Level::Log);
    }

    template<typename... Args>
    auto debug(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt,std::make_format_args(args...),Level::Debug);
    }

    template<typename... Args>
    auto info(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt,std::make_format_args(args...),Level::Info);
    }

    template<typename... Args>
    auto warning(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt,std::make_format_args(args...),Level::Warning);
    }

    template<typename... Args>
    auto error(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt,std::make_format_args(args...),Level::Error);
    }

    template<typename... Args>
    auto fatal(const fmt::format_string &fmt, Args &&... args) -> void {
        print(fmt, std::make_format_args(args...), Level::Fatal);
    }
private:
    auto print(const fmt::format_string &fmt,std::format_args args,const Logger::Level &level) -> void;
private:
    std::unique_ptr<struct LoggerContext> context;
};

#endif //MYLOGGER_LOGGER_H
