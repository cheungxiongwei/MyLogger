#include "logger/logger.h"

struct LoggerContext {
    Logger::Level semantic_level{Logger::Level::Verbose};
    std::string fs_name;
    std::size_t fs_max_size{0};
    std::size_t fs_current_size{0};
    std::optional<fmt::async_logger> fs{std::nullopt};
};

Logger::Logger() : context(std::make_unique<LoggerContext>()) {

}

Logger::~Logger() {

}

auto Logger::instance() -> Logger & {
    static Logger object;
    return object;
}

auto Logger::init(Logger::Level level, const char *file_name, size_t max_file_size) -> Logger & {
    this->context->fs_name = file_name;
    this->context->semantic_level = level;
    this->context->fs.emplace(fmt::async_logger(std::format(".{:%Y%m%d_%H%M%S}.{}",
                                                            std::chrono::zoned_time{std::chrono::current_zone(),
                                                                                    std::chrono::system_clock::now()},
                                                            file_name)));
    this->context->fs_max_size = max_file_size;
    return *this;
}

auto Logger::print(const fmt::format_string &fmt, std::format_args args, const Logger::Level &level) -> void {
    auto timestamp = std::format("{:%Y-%m-%d %H:%M:%S %z}",
                                 std::chrono::zoned_time{std::chrono::current_zone(), fmt.timestamp});
    auto tag = [&level] {
        switch (level) {
            case Logger::Level::Log:
                return "[LOG]";
            case Logger::Level::Debug:
                return "[DEBUG]";
            case Logger::Level::Info:
                return "[INFO]";
            case Logger::Level::Warning:
                return "[WARNING]";
            case Logger::Level::Error:
                return "[ERROR]";
            case Logger::Level::Fatal:
                return "[FATAL]";
            case Logger::Level::Verbose:
                return "[VERBOSE]";
            default:
                return "[NONE]";
        }
    }();
    auto location = std::format("{}:{}#{}:", fmt.location.file_name(), fmt.location.line(),
                                fmt.location.function_name());
    auto payload = vformat(fmt.str, args);

    if (level > this->context->semantic_level) {
        return;
    }

    fmt::print("{} {} {} {}", timestamp, tag, location, payload);

    if (this->context->fs.has_value()) {
        if(this->context->fs_current_size > this->context->fs_max_size){
            this->context->fs.emplace(fmt::async_logger(std::format(".{:%Y%m%d_%H%M%S}.{}",
                                                                    std::chrono::zoned_time{std::chrono::current_zone(),
                                                                                            std::chrono::system_clock::now()},
                                                                    this->context->fs_name)));
            this->context->fs_current_size = 0;
        }

        auto text = std::format("{} {} {} {}", timestamp, tag, location, payload);
        this->context->fs_current_size += text.size();
        this->context->fs->print(text);
    }
}
