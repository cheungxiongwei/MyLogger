#ifndef MYLOGGER_SOURCE_LOCATION_HPP
#define MYLOGGER_SOURCE_LOCATION_HPP

#include <cstdint>
#include <string_view>
#include <format>
#include <chrono>
#include <syncstream>
#include <fstream>
#include <iostream>

namespace fmt {
    // Filename
    // Function Name
    // Line Number
    // Semantic Level
    // Timestamp
    // Payload
    // c++ 20
    struct source_location {
        [[nodiscard]] static consteval source_location current(const uint_least32_t _Line_ = __builtin_LINE(),
                                                               const char *const _File_ = __builtin_FILE(),
                                                               const char *const _Function_ = __builtin_FUNCTION()) noexcept {
            source_location _Result;
            _Result._Line = _Line_;
            _Result._File = _File_;
            _Result._Function = _Function_;
            return _Result;
        }

        [[nodiscard]] constexpr source_location() noexcept = default;

        [[nodiscard]] constexpr uint_least32_t line() const noexcept {
            return _Line;
        }

        [[nodiscard]] constexpr const char *file_name() const noexcept {
            return _File;
        }

        [[nodiscard]] constexpr const char *function_name() const noexcept {
            return _Function;
        }

    private:
        uint_least32_t _Line{};
        const char *_File = "";
        const char *_Function = "";
    };

    // c++ 23
    // default to stdout
    template<typename... Args>
    constexpr void print(const std::string_view fmt, Args &&... args) {
        fputs(std::vformat(fmt, std::make_format_args(args...)).c_str(), stdout);
    }

    // send to FILE*
    template<typename... Args>
    constexpr void print(FILE *stream, const std::string_view fmt, Args &&... args) {
        fputs(std::vformat(fmt, std::make_format_args(args...)).c_str(), stream);
    }

    // send to ostream
    template<typename... Args>
    constexpr void print(std::ostream &os, const std::string_view fmt, Args &&... args) {
        os << std::vformat(fmt, std::make_format_args(args...));
    }

    struct format_string {
        const std::string_view str;
        fmt::source_location location;
        std::chrono::time_point<std::chrono::system_clock> timestamp;

        format_string(const char *str, const fmt::source_location &location = fmt::source_location::current(),
                      const std::chrono::time_point<std::chrono::system_clock> &timestamp = std::chrono::system_clock::now())
                : str(
                str), location(location), timestamp(timestamp) {
        }
    };

    class async_logger {
    public:
        explicit async_logger(const std::string &filename) : file(filename, std::ios::out | std::ios::trunc),
                                                             stream(file) {
        }

        explicit async_logger(async_logger &&object) : file(std::move(object.file)), stream(file) {

        }

        ~async_logger() {
            stream.emit();
            file.close();
        }

        void print(const std::string_view &str) {
            stream.write(str.data(), str.size());
        }

    private:
        std::fstream file;
        std::osyncstream stream;
    };
}

#endif //MYLOGGER_SOURCE_LOCATION_HPP
