#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>

TEST_CASE("stdout_st") {
    statlog::sync_logger<statlog::stdout_sink_st> logger{ statlog::stdout_sink_st() };
    logger.trace("[trace] Hello, world!");
    logger.info("[info] Hello, world!");
    logger.warn("[warn] Hello, world!");
}

TEST_CASE("stdout_mt") {
    statlog::sync_logger<statlog::stdout_sink_mt> logger{ statlog::stdout_sink_mt() };
    std::thread t1([&logger] {
        for (int i = 0; i < 42; ++i) {
            logger.trace("[trace][t1] Hello, world!");
            logger.info("[info][t1] Hello, world!");
            logger.warn("[warn][t1] Hello, world!");
        }
    });
    std::thread t2([&logger] {
        for (int i = 0; i < 42; ++i) {
            logger.trace("[trace][t2] Hello, world!");
            logger.info("[info][t2] Hello, world!");
            logger.warn("[warn][t2] Hello, world!");
        }
    });
    t1.join();
    t2.join();
}
