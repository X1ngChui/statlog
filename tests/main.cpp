#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>
#include <cstdio>

TEST_CASE("exclusive sinks") {
    constexpr statlog::pattern pattern("%^[%L][%T][%n][%t] %v%$");
    statlog::file_sink_mt file_sink("file.log");
    statlog::stdout_sink_mt stdout_sink;

    auto logger = statlog::make_async_logger<pattern>("file", statlog::make_sink_list(std::move(file_sink), std::move(stdout_sink)));
    
    std::thread t1([&] {
        for (int i = 0; i < 100000; i++) {
            logger.info("Hello, {}", i);
        }
        });

    std::thread t2([&] {
        for (int i = 0; i < 100000; i++) {
            logger.info("Hello, {}", i);
        }
        });
    t1.join();
    t2.join();
}

TEST_CASE("shared sinks") {
    constexpr statlog::pattern pattern("[%L][%t][%n] %v");
    statlog::file_sink_st file_sink("shared_file.log");
    auto logger1 = statlog::make_sync_logger<pattern>("logger_one", statlog::make_sink_list(file_sink));
    auto logger2 = statlog::make_sync_logger<pattern>("logger_two", statlog::make_sink_list(file_sink));

    for (int i = 0; i < 1000; i++) {
        logger1.info("Hello, {}", i);
        logger2.info("Hello, {}", i);
    }
}
