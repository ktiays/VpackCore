//
// Created by ktiays on 2022/8/18.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_MATH_HPP
#define VPACKCORE_MATH_HPP

#include <limits>
#include <type_traits>

namespace vpk {

template<typename T, typename = std::enable_if<std::numeric_limits<T>::has_infinity>>
static bool is_infinity(const T& n) {
    return n == std::numeric_limits<T>::infinity();
}

template<typename T, typename = std::enable_if<std::numeric_limits<T>::has_infinity>>
static bool is_infinity(T&& n) {
    return n == std::numeric_limits<T>::infinity();
}

}

#endif //VPACKCORE_MATH_HPP
