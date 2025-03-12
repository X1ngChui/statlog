#pragma once

#ifndef STATLOG_UTILITY_BUFFER_INCLUDED
#define STATLOG_UTILITY_BUFFER_INCLUDED

#include <cassert>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <utility>

namespace statlog {
    template <typename T>
    class buffer_t {
    public:
        using value_type = T;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;

        explicit buffer_t(size_t capacity)
            : _capacity(capacity),
            _storage(std::make_unique_for_overwrite<T[]>(capacity)),
            _size(0) {
        }

        ~buffer_t() { clear(); }

        buffer_t(const buffer_t& other)
            : _capacity(other._capacity),
            _storage(std::make_unique_for_overwrite<T[]>(other._capacity)),
            _size(other._size) {
            std::uninitialized_copy(other.begin(), other.end(), _storage.get());
        }

        buffer_t(buffer_t&& other) noexcept
            : _capacity(other._capacity),
            _storage(std::move(other._storage)),
            _size(other._size) {
            other._capacity = 0;
            other._size = 0;
        }

        buffer_t& operator=(const buffer_t& other) {
            if (this != &other) {
                auto new_storage = std::make_unique_for_overwrite<T[]>(other._capacity);
                std::uninitialized_copy(other.begin(), other.end(), new_storage.get());
                clear();
                _storage = std::move(new_storage);
                _capacity = other._capacity;
                _size = other._size;
            }
            return *this;
        }

        buffer_t& operator=(buffer_t&& other) noexcept {
            if (this != &other) {
                clear();
                _storage = std::move(other._storage);
                _capacity = other._capacity;
                _size = other._size;
                other._capacity = 0;
                other._size = 0;
            }
            return *this;
        }

        void push_back(const T& value) { emplace_back(value); }
        void push_back(T&& value) { emplace_back(std::move(value)); }

        template <typename... Args>
        void emplace_back(Args&&... args) {
            assert(_size < _capacity);
            std::construct_at(data() + _size, std::forward<Args>(args)...);
            ++_size;
        }

        void pop_back() {
            assert(!empty());
            std::destroy_at(data() + --_size);
        }

        iterator insert(const_iterator pos, const T& value) { return emplace(pos, value); }
        iterator insert(const_iterator pos, T&& value) { return emplace(pos, std::move(value)); }

        template <typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            const auto count = static_cast<size_type>(std::distance(first, last));
            assert(_size + count <= _capacity);

            auto insert_pos = const_cast<iterator>(pos);
            if (insert_pos != end()) {
                std::move_backward(insert_pos, end(), end() + count);
            }

            std::uninitialized_copy(first, last, insert_pos);
            _size += count;
            return insert_pos;
        }

        template <typename... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            assert(_size < _capacity);

            auto insert_pos = const_cast<iterator>(pos);
            if (insert_pos != end()) {
                std::move_backward(insert_pos, end(), end() + 1);
            }

            std::construct_at(insert_pos, std::forward<Args>(args)...);
            ++_size;
            return insert_pos;
        }

        reference front() {
            assert(!empty());
            return *begin();
        }

        const_reference front() const {
            assert(!empty());
            return *begin();
        }

        reference back() {
            assert(!empty());
            return *(end() - 1);
        }

        const_reference back() const {
            assert(!empty());
            return *(end() - 1);
        }

        reference operator[](size_type pos) {
            assert(pos < _size);
            return data()[pos];
        }

        const_reference operator[](size_type pos) const {
            assert(pos < _size);
            return data()[pos];
        }

        size_type size() const noexcept { return _size; }
        bool empty() const noexcept { return _size == 0; }
        size_type capacity() const noexcept { return _capacity; }

        iterator begin() noexcept { return data(); }
        iterator end() noexcept { return data() + _size; }
        const_iterator begin() const noexcept { return data(); }
        const_iterator end() const noexcept { return data() + _size; }
        const_iterator cbegin() const noexcept { return begin(); }
        const_iterator cend() const noexcept { return end(); }

        T* data() noexcept { return _storage.get(); }
        const T* data() const noexcept { return _storage.get(); }

        void clear() {
            std::destroy(begin(), end());
            _size = 0;
        }

    private:
        size_type _capacity;
        std::unique_ptr<T[]> _storage;
        size_type _size;
    };

    template <typename T>
    using buffer = buffer_t<T>;
}

#endif
