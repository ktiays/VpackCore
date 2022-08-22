//
// Created by ktiays on 2022/8/14.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_VERTICAL_CONTAINER_HPP
#define VPACKCORE_VERTICAL_CONTAINER_HPP

#include <algorithm>

#include "hv_container.hpp"
#include "../../utils/indexed.hpp"

namespace vpk {

template<typename Identifier, typename ValueType>
class VerticalContainer : public detail::HVContainer<Identifier, ValueType> {
public:
    VerticalContainer(const std::vector<LayoutablePointer<Identifier, ValueType>>& children,
                      const LayoutParams<ValueType>& params, HorizontalAlignment align)
        : detail::HVContainer<Identifier, ValueType>(children, params), alignment(align) {
        const SizeProperty<ValueType> size_property = params.size_property;
        __DEAL_MIN_WIDTH_FOR_POLICY(MinMaxPolicy::max);
        __DEAL_MAX_WIDTH_FOR_POLICY(MinMaxPolicy::max);
        __DEAL_MIN_HEIGHT_FOR_POLICY(MinMaxPolicy::sum);
        __DEAL_MAX_HEIGHT_FOR_POLICY(MinMaxPolicy::sum);
    }

    HorizontalAlignment alignment;

    using Element = typename detail::HVContainer<Identifier, ValueType>::Element;

    detail::AxisEdgeInsets<ValueType> axis_edge_insets_for_element(Element element) const override {
        const EdgeInsets<ValueType> padding = element->padding();
        return { padding.left, padding.right, padding.top, padding.bottom };
    }

    ValueType min_main_for_element(Element element) const override { return element->min_height(); }

    ValueType max_main_for_element(Element element) const override { return element->max_height(); }

    ValueType min_cross_for_element(Element element) const override { return element->min_width(); }

    ValueType max_cross_for_element(Element element) const override { return element->max_width(); }

    Point<ValueType> point_from_axis_point(const detail::AxisPoint<ValueType>& point) const override {
        return { point.cross, point.main };
    }

    detail::AxisPoint<ValueType> axis_point_from_point(const Point<ValueType>& point) const override {
        return { point.y, point.x };
    }

    Size<ValueType> size_from_axis_size(const detail::AxisSize<ValueType>& size) const override {
        return { size.cross, size.main };
    };

    detail::AxisSize<ValueType> axis_size_from_size(const Size<ValueType>& size) const override {
        return { size.height, size.width };
    };

    detail::AxisAlignment axis_alignment() const override {
        switch (alignment) {
            case HorizontalAlignment::leading:
                return detail::AxisAlignment::start;
            case HorizontalAlignment::center:
                return detail::AxisAlignment::center;
            case HorizontalAlignment::trailing:
                return detail::AxisAlignment::end;
        }
    }

};

}

#endif //VPACKCORE_VERTICAL_CONTAINER_HPP
