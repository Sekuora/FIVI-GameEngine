
#pragma once
/**
 * Type Casting Wrappers
 */

// Static cast wrapper for compile-time checked conversion
template<typename To, typename From>
constexpr To scast(From&& value) {
    return static_cast<To>(std::forward<From>(value));
}

// Dynamic cast wrapper for runtime polymorphic conversion (Pointers)
template<typename To, typename From>
inline To dcast(From* value) {
    return dynamic_cast<To>(value);
}

// Reinterpret cast wrapper for low-level bit reinterpretation
template<typename To, typename From>
constexpr To rcast(From&& value) {
    return reinterpret_cast<To>(std::forward<From>(value));
}

// Const cast wrapper to add or remove const-qualifiers
template<typename To, typename From>
constexpr To ccast(From&& value) {
    return const_cast<To>(std::forward<From>(value));
}

// Bit cast wrapper for safe bit-level reinterpretation (C++20)
template<typename To, typename From>
constexpr To bcast(const From& value) noexcept {
    return std::bit_cast<To>(value);
}