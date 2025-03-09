#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>

TEST_CASE("stdout_st") {
    statlog::stdout_sink_st stdout_sink;
    statlog::sync_logger logger{ std::move(stdout_sink) };
    logger.trace("[trace] Hello, world!\n");
    logger.info("[info] Hello, world!\n");
    logger.warn("[warn] Hello, world!\n");
}

TEST_CASE("stdout_mt") {
    statlog::sync_logger logger{ statlog::stdout_sink_mt() };
    std::thread t1([&logger] {
        for (int i = 0; i < 42; ++i) {
            logger.trace("[trace][t1] Hello, {}!\n", i);
            logger.info("[info][t1] Hello, {}!\n", i);
            logger.warn("[warn][t1] Hello, {}!\n", i);
        }
    });
    std::thread t2([&logger] {
        for (int i = 0; i < 42; ++i) {
            logger.trace("[trace][t2] Hello, world!\n");
            logger.info("[info][t2] Hello, world!\n");
            logger.warn("[warn][t2] Hello, world!\n");
        }
    });
    t1.join();
    t2.join();
}

TEST_CASE("formatter") {
    statlog::formatter<statlog::pattern_t("%t")> formatter;
    statlog::formatter_buffer buffer;
    formatter.format(buffer);
    CHECK_EQ(buffer, "thread_id");
}
