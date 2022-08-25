//
// Created by ktiays on 2022/8/16.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_SOME_VIEW_HPP
#define VPACKCORE_SOME_VIEW_HPP

#include <string>

#include "../../VpackCore.hpp"

#define __IMPL_PADDING_FOR_CONTAINER(container) \
container& padding(vpk::EdgeInsets<value_type>&& padding) { \
    padding_ = padding;                         \
    return *this;                               \
}

#define __IMPL_OFFSET_FOR_CONTAINER(container) \
container& offset(vpk::Point<value_type>&& offset) { \
    offset_ = offset;                          \
    return *this;                              \
}

#define __DECL_LAYOUT_PARAMS \
vpk::SizeProperty<value_type> size_property; \
vpk::EdgeInsets<value_type> padding_;        \
vpk::Point<value_type> offset_

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

static std::string spacer_identifier() {
    return "__spacer";
}

struct SomeView {
    using identifier_t = std::string;
    using value_type = double;

    virtual vpk::LayoutablePointer<identifier_t, value_type> make_view() const = 0;

    vpk::LayoutResult<identifier_t, value_type> compute(vpk::Rect<value_type>&& frame) const {
        const auto computer = vpk::LayoutComputer<identifier_t, value_type>(make_view());
        auto result = computer.compute(frame);
        auto iter = result.map.find(spacer_identifier());
        if (iter != result.map.end()) result.map.erase(iter);
        return result;
    };
};

}

#endif //VPACKCORE_SOME_VIEW_HPP
