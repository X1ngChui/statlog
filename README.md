# statlog - A Static, Fast & Lightweight C++ Logging Library

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

**statlog** is a C++ logging library designed with static configuration and high performance in mind. It emphasizes compile-time pattern resolution and avoids dynamic polymorphism, making it suitable for performance-critical applications.

## Features

- **Compile-time pattern parsing** - Format patterns resolved at compile time
- **Thread-safe sinks** - Both multi-thread and single-thread sink implementations
- **Low overhead** - Minimal runtime cost through template metaprogramming
- **Synchronous logging** - Predictable logging behavior for system programming
- **Asynchronous logging** - High performace logging that don't block the main thread
- **Flexible sink architecture** - Multiple logger-sink configurations supported

## Getting Started

### Prerequisites
- C++23 or later
- CMake 3.14+

### Installation

```bash
git clone --recursive https://github.com/X1ngChui/statlog.git
# Build using your preferred C++ build system
```

## Basic Usage

### Exclusive Sinks (1 Logger → Multiple Sinks)

```cpp
constexpr statlog::pattern common_pattern("[%L][%t][%n] %v");
constexpr statlog::pattern colorful_pattern("%^[%L][%t][%n] %v%$");

statlog::file_sink_mt<pattern> file_sink("app.log");
statlog::stdout_sink_mt<colorful_pattern> console_sink{};

statlog::async_logger logger{"main", 
	statlog::make_sink_list(std::move(file_sink), std::move(console_sink)),
	statlog::level::info, 4
};

logger.info("System initialized");
logger.warn("Low memory: {}MB free", 512);
```

### Shared Sinks (Multiple Loggers → 1 Sink)

```cpp
constexpr statlog::pattern pattern("[%L][%t][%n] %v");
statlog::file_sink_st<pattern> shared_sink("debug.log");

statlog::sync_logger net_logger{"network", statlog::make_sink_list(shared_sink)};
statlog::sync_logger db_logger{"database", statlog::make_sink_list(shared_sink)};

net_logger.debug("Packet received: {} bytes", 1500);
db_logger.info("Connection pool: {} active", 8);
```

## Pattern Specification

Current supported format specifiers:

| Specifier | Description             |
|-----------|-------------------------|
| `%v`      | Actual log message      |
| `%t`      | Thread ID               |
| `%n`      | Logger name             |
| `%l`      | Log level in lowercase  |
| `%L`      | Log level in uppercase  |
| `%^`      | Log level color start   |
| `%$`      | Log level color end     |
| `%%`      | Literal `%` character   |


More specifiers coming soon! Welcome for contribution!

## Sink Types

| Sink Type      | Thread Safety | Description                     |
|----------------|----------------|---------------------------------|
| `file_sink_mt` | Multi-thread    | Thread-safe file logging       |
| `file_sink_st` | Single-thread   | Single-thread file IO          |
| `stdout_sink_mt`| Multi-thread   | Thread-safe console output     |
| `stdout_sink_st`| Single-thread  | Single-thread console writing  |

## Performance Considerations

**statlog** achieves its speed through:

- Compile-time pattern validation
- Template-based sink composition
- Lock-free design for single-thread sinks

## Contributing

We welcome contributions! Current development priorities:

- **New format specifiers**: Add function names (`%F`), source locations (`%@`), and custom timestamp formats.
- **Sink improvements**: Focus on efficiency through raw syscalls, add new sink types like UDP/network and circular buffers.

Please see our [Contribution Guidelines](CONTRIBUTING.md) for details.

## License

MIT License - See [LICENSE](LICENSE) for details.

**Note:** Don’t forget to clone with `--recursive` to get required dependencies!




