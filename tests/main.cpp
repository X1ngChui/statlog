#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>
#include <cstdio>

TEST_CASE("exclusive sinks") {
    using statlog::operator"" _KB;

    static constexpr statlog::pattern pattern("%^[%L][%t][%n] %v%$");
    statlog::file_sink_mt file_sink("file.log");
    statlog::colorful_stdout_sink_mt stdout_sink(32_KB);

    auto logger = statlog::make_async_logger<pattern>("file", statlog::make_sink_list(std::move(file_sink), std::move(stdout_sink)));
    
    std::thread t1([&logger] {
        for (int i = 0; i < 100000; ++i) {
            logger.info("Hello from thread 1");
            logger.trace("Hello from thread 1");
        }
    });
    std::thread t2([&logger] {
        for (int i = 0; i < 100000; ++i) {
            logger.info("Hello from thread 2");
        }
    });
    
    t1.join();
    t2.join();
}

TEST_CASE("shared sinks") {
    static constexpr statlog::pattern pattern("[%L][%t][%n] %v");
    statlog::file_sink_st file_sink("shared_file.log");
    auto logger1 = statlog::make_sync_logger<pattern>("logger_one", statlog::make_sink_list(file_sink));
    auto logger2 = statlog::make_sync_logger<pattern>("logger_two", statlog::make_sink_list(file_sink));

    for (int i = 0; i < 1000; i++) {
        logger1.info("Hello, {}", i);
        logger2.info("Hello, {}", i);
    }
}
