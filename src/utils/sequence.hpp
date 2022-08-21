//
// Created by ktiays on 2022/8/13.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_SEQUENCE_HPP
#define VPACKCORE_SEQUENCE_HPP

#include <algorithm>

#include "../optional.hpp"

namespace vpk {

template<typename InputIterator, typename OutputIterator, typename Operation>
OutputIterator filter_map(InputIterator first, InputIterator last, OutputIterator result, Operation op) {
    for (; first != last; ++first, (void) ++result) {
        auto r = op(*first);
        if (!r.has_value()) continue;
        *result = *r;
    }
    return result;
}

}

#endif //VPACKCORE_SEQUENCE_HPP
