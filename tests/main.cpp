#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>
#include <cstdio>

//TEST_CASE("stdout_st") {
//    constexpr statlog::pattern_t pattern("[%L][%t][%n] %v");
//    statlog::stdout_sink_st<pattern> stdout_sink;
//    statlog::sync_logger logger{ "stdout", std::move(stdout_sink)};
//    logger.info("Hello, world!");
//}
//
//TEST_CASE("stdout_mt") {
//    constexpr statlog::pattern_t pattern("[%L][%t][%n] %v");
//    statlog::sync_logger logger{ "stdout", statlog::stdout_sink_mt<pattern>()};
//    std::thread t1([&logger] {
//        for (int i = 0; i < 10000; ++i) {
//            logger.info("Hello, {}!", i);
//        }
//    });
//    std::thread t2([&logger] {
//        for (int i = 0; i < 10000; ++i) {
//            logger.info("Hello, {}!", i);
//        }
//    });
//    t1.join();
//    t2.join();
//}

TEST_CASE("file_sink") {
    constexpr statlog::pattern_t pattern("[%L][%t][%n] %v");
    statlog::sink_pointer fsink = std::make_unique<statlog::file_sink_st<pattern>>("log.txt");
    statlog::sink_pointer stdout_sink = std::make_unique<statlog::stdout_sink_st<pattern>>();
    statlog::sync_logger logger{ "file", std::move(fsink), std::move(stdout_sink) };
    for (int i = 0; i < 10000; ++i) {
        logger.warn("Hello, {}!", i);
    }
}
