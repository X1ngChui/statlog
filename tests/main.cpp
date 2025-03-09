#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>

TEST_CASE("stdout_st") {
    constexpr statlog::pattern_t pattern("[%L][%t][%n] %v\n");
    statlog::stdout_sink_st<pattern> stdout_sink;
    statlog::sync_logger logger{ "stdout", std::move(stdout_sink)};
    logger.trace("Hello, world!");
    logger.info("Hello, world!");
    logger.warn("Hello, world!");
}

TEST_CASE("stdout_mt") {
    constexpr statlog::pattern_t pattern("[%L][%t][%n] %v\n");
    statlog::sync_logger logger{ "stdout", statlog::stdout_sink_mt<pattern>()};
    std::thread t1([&logger] {
        for (int i = 0; i < 42; ++i) {
            logger.trace("Hello, {}!", i);
            logger.info("Hello, {}!", i);
            logger.warn("Hello, {}!", i);
        }
    });
    std::thread t2([&logger] {
        for (int i = 0; i < 42; ++i) {
            logger.trace("Hello, world!");
            logger.info("Hello, world!");
            logger.warn("Hello, world!");
        }
    });
    t1.join();
    t2.join();
}

