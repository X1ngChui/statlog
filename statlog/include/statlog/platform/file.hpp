#pragma once

#ifndef STATLOG_PLATFORM_FILE_INCLUDED
#define STATLOG_PLATFORM_FILE_INCLUDED

#include <filesystem>
#include <stdexcept>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace statlog {

    class file_t {
    public:
        explicit file_t(const std::filesystem::path& path, std::ios_base::openmode mode) {
#ifdef _WIN32
            DWORD access = 0;
            DWORD creation = OPEN_EXISTING;
            DWORD share = FILE_SHARE_READ;

            if (mode & std::ios::app) {
                access = FILE_APPEND_DATA;
                creation = OPEN_ALWAYS;
            }
            else if (mode & std::ios::out) {
                access = GENERIC_WRITE;
                if (mode & std::ios::trunc) {
                    creation = CREATE_ALWAYS;
                }
                else {
                    creation = OPEN_ALWAYS;
                }
            }

            if (mode & std::ios::in) {
                access |= GENERIC_READ;
            }

            _handle = CreateFileW(
                path.c_str(),
                access,
                share,
                nullptr,
                creation,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                nullptr
            );

            if (_handle == INVALID_HANDLE_VALUE) {
                throw std::runtime_error("Failed to open file: " + path.string());
            }

            if ((mode & std::ios::trunc) && !(mode & std::ios::app)) {
                SetFilePointer(_handle, 0, nullptr, FILE_BEGIN);
                SetEndOfFile(_handle);
            }
#else
            int flags = O_CLOEXEC;
            mode_t perm = 0644;

            if (mode & std::ios::in && mode & std::ios::out) {
                flags |= O_RDWR;
            }
            else if (mode & std::ios::in) {
                flags |= O_RDONLY;
            }
            else if (mode & std::ios::out) {
                flags |= O_WRONLY;
            }

            if (mode & std::ios::app) {
                flags |= O_APPEND;
                if (mode & std::ios::out) {
                    flags |= O_CREAT;
                }
            }
            else if (mode & std::ios::trunc) {
                flags |= O_TRUNC;
                if (mode & std::ios::out) {
                    flags |= O_CREAT;
                }
            }
            else if (mode & std::ios::out) {
                flags |= O_CREAT;
            }

            _fd = ::open(path.c_str(), flags, perm);
            if (_fd == -1) {
                throw std::runtime_error("Failed to open file: " + path.string());
            }

            if ((mode & std::ios::app) && (lseek(_fd, 0, SEEK_END) == -1)) {
                ::close(_fd);
                throw std::runtime_error("Failed to seek to end");
            }
#endif
        }

        file_t(file_t&& other) noexcept {
#ifdef _WIN32
            _handle = other._handle;
            other._handle = INVALID_HANDLE_VALUE;
#else
            _fd = other._fd;
            other._fd = -1;
#endif
        }

        file_t& operator=(file_t&& other) noexcept {
            if (this != &other) {
#ifdef _WIN32
                if (_handle != INVALID_HANDLE_VALUE) {
                    CloseHandle(_handle);
                }
                _handle = other._handle;
                other._handle = INVALID_HANDLE_VALUE;
#else
                if (_fd != -1) {
                    ::close(_fd);
                }
                _fd = other._fd;
                other._fd = -1;
#endif
            }
            return *this;
        }

        ~file_t() {
#ifdef _WIN32
            if (_handle != INVALID_HANDLE_VALUE) {
                CloseHandle(_handle);
            }
#else
            if (_fd != -1) {
                ::close(_fd);
            }
#endif
        }

        void write(const char* data, size_t size) {
#ifdef _WIN32
            DWORD written = 0;
            BOOL success = WriteFile(_handle, data, static_cast<DWORD>(size), &written, nullptr);
            if (!success || written != static_cast<DWORD>(size)) {
                throw std::runtime_error("Failed to write to file");
            }
#else
            ssize_t result = ::write(_fd, data, size);
            if (result == -1 || static_cast<size_t>(result) != size) {
                throw std::runtime_error("Failed to write to file");
            }
#endif
        }

        void flush() {
#ifdef _WIN32
            if (!FlushFileBuffers(_handle)) {
                throw std::runtime_error("Failed to flush file buffers");
            }
#else
            if (::fsync(_fd) == -1) {
                throw std::runtime_error("Failed to flush file buffers");
            }
#endif
        }

    private:
#ifdef _WIN32
        HANDLE _handle = INVALID_HANDLE_VALUE;
#else
        int _fd = -1;
#endif
    };
    using file = file_t;
}

#endif