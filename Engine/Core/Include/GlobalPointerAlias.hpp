#pragma once

#include <memory>


// Alias functions

// Unique Pointer
template<typename T>
using Unique = std::unique_ptr<T>;

// Shared  Pointer
template<typename T>
using Shared = std::shared_ptr<T>;

// Shared  Pointer
template<typename T>
using Weak = std::weak_ptr<T>;

// Make Unique Handler
template<typename T, typename ... Args>
constexpr Unique<T> newU(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Make Shared Handler
template<typename T, typename ... Args>
constexpr Shared<T> newS(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}




