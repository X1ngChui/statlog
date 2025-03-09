#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <statlog/statlog.hpp>

#include <thread>
#include <cstdio>

TEST_CASE("file_sink") {
    constexpr statlog::pattern_t pattern("[%L][%t][%n] %v");
    statlog::file_sink_st<pattern> file_sink("file.log");
    //statlog::stdout_sink_mt<pattern> stdout_sink{};

    statlog::sync_logger logger{ "file", std::move(file_sink) };
    
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
