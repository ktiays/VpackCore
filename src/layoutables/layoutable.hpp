//
// Created by ktiays on 2022/8/12.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_LAYOUTABLE_HPP
#define VPACKCORE_LAYOUTABLE_HPP

#include "../layout_result.hpp"
#include "../types.hpp"

namespace vpk {

enum class LayoutType {
    item,
    container,
};

template<typename ValueType>
struct SizeProperty {
    optional<ValueType> min_width;
    optional<ValueType> min_height;
    optional<ValueType> max_width;
    optional<ValueType> max_height;
};

template<typename ValueType>
struct LayoutParams {
    SizeProperty<ValueType> size_property;
    EdgeInsets<ValueType> padding;
    Point<ValueType> offset;
};

template<
    typename Identifier,
    typename ValueType,
    typename = std::enable_if<std::numeric_limits<ValueType>::has_infinity>
>
class Layoutable {
public:
    explicit Layoutable(LayoutParams<ValueType> p)
        : params(p) {}

    virtual LayoutType type() const = 0;

    virtual void layout(const Rect<ValueType>& frame, LayoutResult<Identifier, ValueType>& result) const = 0;

    virtual Size<ValueType> measure(const Size<ValueType>& size) { return {}; }

    /* The minimum or maximum values here indicate the element's own size attribute, excluding padding. */

    ValueType min_width() const { return min_width_; }

    ValueType min_height() const { return min_height_; }

    ValueType max_width() const { return max_width_; }

    ValueType max_height() const { return max_height_; }

    inline EdgeInsets<ValueType> padding() const { return params.padding; }

    inline Point<ValueType> offset() const { return params.offset; }

    virtual ~Layoutable() = default;

protected:
    LayoutParams<ValueType> params;

    ValueType min_width_;
    ValueType min_height_;
    ValueType max_width_;
    ValueType max_height_;
};

template<typename Identifier, typename ValueType>
using LayoutablePointer = std::shared_ptr<Layoutable<Identifier, ValueType>>;

}

#endif //VPACKCORE_LAYOUTABLE_HPP
