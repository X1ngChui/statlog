#pragma once

#ifndef STATLOG_UTILITY_INLINE_VECTOR_INCLUDED
#define STATLOG_UTILITY_INLINE_VECTOR_INCLUDED

#include <cassert>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <memory>
#include <ranges>
#include <cstdlib>

namespace statlog {
    template <typename T, size_t Capacity>
    class inline_vector {
    public:
        using value_type = T;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;

        inline_vector() noexcept : size_(0), storage_(std::make_unique_for_overwrite<T[]>(Capacity)) {}

        ~inline_vector() { clear(); }

        inline_vector(const inline_vector& other) : inline_vector() {
            std::uninitialized_copy(other.begin(), other.end(), data());
            size_ = other.size_;
        }

        inline_vector(inline_vector&& other) noexcept = default;
        inline_vector& operator=(const inline_vector& other) {
            if (this != &other) {
                clear();
                std::uninitialized_copy(other.begin(), other.end(), data());
                size_ = other.size_;
            }
            return *this;
        }

        inline_vector& operator=(inline_vector&& other) noexcept = default;

        void push_back(const T& value) { emplace_back(value); }
        void push_back(T&& value) { emplace_back(std::move(value)); }

        template <typename... Args>
        void emplace_back(Args&&... args) {
            assert(size_ < Capacity);
            std::construct_at(data() + size_, std::forward<Args>(args)...);
            ++size_;
        }

        void pop_back() {
            assert(size_ > 0);
            std::destroy_at(data() + --size_);
        }

        iterator insert(const_iterator pos, const T& value) { return emplace(pos, value); }
        iterator insert(const_iterator pos, T&& value) { return emplace(pos, std::move(value)); }

        template <typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            assert(size_ + std::distance(first, last) <= Capacity);
            assert(pos >= begin() && pos <= end());

            iterator insert_pos = const_cast<iterator>(pos);
            size_type count = std::distance(first, last);

            if (insert_pos != end()) {
                std::move_backward(insert_pos, end(), end() + count);
            }

            std::uninitialized_copy(first, last, insert_pos);
            size_ += count;
            return insert_pos;
        }

        template <typename... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            assert(size_ < Capacity);
            assert(pos >= begin() && pos <= end());

            iterator insert_pos = const_cast<iterator>(pos);
            if (insert_pos != end()) {
                std::move_backward(insert_pos, end(), end() + 1);
            }
            std::construct_at(insert_pos, std::forward<Args>(args)...);
            ++size_;
            return insert_pos;
        }

        reference operator[](size_type pos) {
            assert(pos < size_);
            return data()[pos];
        }

        const_reference operator[](size_type pos) const {
            assert(pos < size_);
            return data()[pos];
        }

        reference front() {
            assert(!empty());
            return data()[0];
        }

        const_reference front() const {
            assert(!empty());
            return data()[0];
        }

        reference back() {
            assert(!empty());
            return data()[size_ - 1];
        }

        const_reference back() const {
            assert(!empty());
            return data()[size_ - 1];
        }

        size_type size() const noexcept { return size_; }
        bool empty() const noexcept { return size_ == 0; }
        static constexpr size_type capacity() noexcept { return Capacity; }

        iterator begin() noexcept { return data(); }
        iterator end() noexcept { return data() + size_; }
        const_iterator begin() const noexcept { return data(); }
        const_iterator end() const noexcept { return data() + size_; }
        const_iterator cbegin() const noexcept { return begin(); }
        const_iterator cend() const noexcept { return end(); }

        T* data() noexcept { return storage_.get(); }
        const T* data() const noexcept { return storage_.get(); }

        void clear() {
            std::destroy(begin(), end());
            size_ = 0;
        }

    private:
        std::unique_ptr<T[]> storage_;
        size_type size_;
    };
}

#endif
