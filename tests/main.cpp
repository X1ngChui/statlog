#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>
#include <cstdio>

TEST_CASE("exclusive sinks") {
    constexpr statlog::pattern pattern("[%L][%t][%n] %v");
    statlog::file_sink_mt<pattern> file_sink("file.log");
    statlog::stdout_sink_mt<pattern> stdout_sink;

    statlog::sync_logger logger{ "file", std::move(file_sink), std::move(stdout_sink)};
    
    std::thread t1([&logger] {
        for (int i = 0; i < 1000; ++i) {
            logger.info("Hello from thread 1");
        }
    });
    std::thread t2([&logger] {
        for (int i = 0; i < 1000; ++i) {
            logger.info("Hello from thread 2");
        }
    });
    
    t1.join();
    t2.join();
}

TEST_CASE("shared sinks") {
    constexpr statlog::pattern pattern("[%L][%t][%n] %v");
    statlog::file_sink_st<pattern> file_sink("shared_file.log");
    statlog::sync_logger<decltype(file_sink)&> logger1{ "logger_one", file_sink };
    statlog::sync_logger<decltype(file_sink)&> logger2{ "logger_two", file_sink };

    for (int i = 0; i < 1000; i++) {
        logger1.info("Hello, {}", i);
        logger2.info("Hello, {}", i);
    }
}
