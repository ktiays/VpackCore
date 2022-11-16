//
// Created by ktiays on 2022/8/16.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_SOME_VIEW_HPP
#define VPACKCORE_SOME_VIEW_HPP

#include <string>
#include <random>

#include "uuid.h"
#include "../../VpackCore.hpp"

#define __IMPL_PADDING_FOR_CONTAINER(container) \
container& padding(vpk::core::EdgeInsets<value_type>&& padding) { \
    padding_ = padding;                         \
    return *this;                               \
}

#define __IMPL_OFFSET_FOR_CONTAINER(container) \
container& offset(vpk::core::Point<value_type>&& offset) { \
    offset_ = offset;                          \
    return *this;                              \
}

#define __DECL_LAYOUT_PARAMS \
vpk::core::SizeProperty<value_type> size_property; \
vpk::core::EdgeInsets<value_type> padding_;        \
vpk::core::Point<value_type> offset_

#define __IMPL_SIZE_FOR_CONTAINER(property, container) \
container& property(value_type&& property) {           \
    size_property.property = property;                 \
    return *this;                                      \
}

#define __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(container) \
__IMPL_SIZE_FOR_CONTAINER(min_width, container)       \
__IMPL_SIZE_FOR_CONTAINER(max_width, container)       \
__IMPL_SIZE_FOR_CONTAINER(min_height, container)      \
__IMPL_SIZE_FOR_CONTAINER(max_height, container)      \
__IMPL_PADDING_FOR_CONTAINER(container)               \
__IMPL_OFFSET_FOR_CONTAINER(container)

namespace vpkt {

const double infinity = std::numeric_limits<double>::infinity();

static std::string spacer_identifier_prefix() {
    return "__spacer";
}

static std::string uuid() {
    std::random_device rd;
    std::mt19937 gen{ rd() };
    return uuids::to_string(uuids::uuid_random_generator{ gen }());
}

struct SomeView {
    using identifier_t = std::string;
    using value_type = double;

    virtual vpk::core::LayoutablePointer<identifier_t, value_type> make_view() const = 0;

    vpk::core::LayoutResult<identifier_t, value_type> compute(vpk::core::Rect<value_type>&& frame) const {
        const auto computer = vpk::core::LayoutComputer<identifier_t, value_type>(make_view());
        auto result = computer.compute(frame);
        auto& map = result.map;
        for (auto iter = result.map.begin(); iter != result.map.end();) {
            if (iter->first.find(spacer_identifier_prefix()) != std::string::npos) {
                iter = result.map.erase(iter);
            } else { ++iter; }
        }
        return result;
    };
};

}

#endif //VPACKCORE_SOME_VIEW_HPP
