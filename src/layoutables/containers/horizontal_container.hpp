//
// Created by ktiays on 2022/8/13.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_HORIZONTAL_CONTAINER_HPP
#define VPACKCORE_HORIZONTAL_CONTAINER_HPP

#include <algorithm>

#include "container.hpp"
#include "../../types.hpp"

namespace vpk {

template<typename Identifier, typename ValueType>
class HorizontalContainer : public Container<Identifier, ValueType> {
public:
    HorizontalContainer(const std::vector<LayoutablePointer<Identifier, ValueType>>& children,
                        const LayoutParams<ValueType>& params, VerticalAlignment align)
        : Container<Identifier, ValueType>(children, params), alignment(align) {
        const SizeProperty<ValueType> size_property = params.size_property;
        __DEAL_MIN_WIDTH_FOR_POLICY(MinMaxPolicy::sum);
        __DEAL_MAX_WIDTH_FOR_POLICY(MinMaxPolicy::sum);
        __DEAL_MIN_HEIGHT_FOR_POLICY(MinMaxPolicy::max);
        __DEAL_MAX_HEIGHT_FOR_POLICY(MinMaxPolicy::max);
    }

    VerticalAlignment alignment;

    void layout(const Rect<ValueType>& frame, LayoutResult<Identifier, ValueType>& result) const override;

    Size<ValueType> measure(const Size<ValueType>& size) override;
};

template<typename Identifier, typename ValueType>
void HorizontalContainer<Identifier, ValueType>::layout(const Rect<ValueType>& frame,
                                                        LayoutResult<Identifier, ValueType>& result) const {
    // Layout in terms of the actual space occupied by the elements.
    const Point<ValueType> origin = {
        frame.x + (frame.width - this->cached_measured_size.width) / 2,
        frame.y + (frame.height - this->cached_measured_size.height) / 2
    };
    const Size<ValueType> size = {
        std::max(frame.width, this->cached_measured_size.width),
        std::max(frame.height, this->cached_measured_size.height)
    };

    ValueType used_width = 0;
    for (int i = 0; i < this->children.size(); ++i) {
        const auto child_ptr = this->children[i];

        const auto item_size = this->size_list[i];
        const auto item_padding = child_ptr->padding();
        /// The total size of the accommodating elements.
        ///
        /// The actual size of the element plus the element's own padding.
        const Size<ValueType> item_container_size = {
            item_size.width + item_padding.horizontal(),
            item_size.height + item_padding.vertical()
        };
        const auto y_offset = [this, &size, &item_container_size]() -> ValueType {
            switch (this->alignment) {
                case VerticalAlignment::top:
                    return 0;
                case VerticalAlignment::center:
                    return (size.height - item_container_size.height) / 2;
                case VerticalAlignment::bottom:
                    return size.height - item_container_size.height;
            }
        }();
        const auto item_offset = child_ptr->offset();
        const auto layout_frame_for_child = Rect<ValueType>(
            origin.x + used_width + item_offset.x + item_padding.left,
            origin.y + y_offset + item_offset.y + item_padding.top,
            item_size.width,
            item_size.height
        );
        child_ptr->layout(layout_frame_for_child, result);
        used_width += item_container_size.width;
    }
}

template<typename Identifier, typename ValueType>
Size<ValueType> HorizontalContainer<Identifier, ValueType>::measure(const Size<ValueType>& size) {
    Size<ValueType> measured_size;

    using usize = typename decltype(this->children)::size_type;
    /// A map to record the width allocation of each element.
    std::unordered_map<usize, ValueType> width_map;
    std::vector<std::pair<usize, ValueType>> maximum_width_list;
    maximum_width_list.reserve(this->children.size());

    for (auto it: makeIndexed(this->children)) {
        maximum_width_list.push_back(std::make_pair(it.index(), it.value()->max_width()));
    }

    // Sort the elements in ascending order by the maximum space required.
    std::sort(maximum_width_list.begin(), maximum_width_list.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    const usize list_count = maximum_width_list.size();
    for (auto it: makeIndexed(maximum_width_list)) {
        const usize index = it.index();

        const ValueType rest_width = size.width - measured_size.width;
        /// The number of remaining elements that need to be allocated additional space.
        const usize count = list_count - index;
        // The remaining elements share the remaining space equally,
        // i.e., the space occupied by each element is not allowed to exceed this value.
        const ValueType maximum_container_width = rest_width / count;
        /// The position of the element instance in the children list.
        const usize element_idx = it.value().first;
        const auto child = this->children[element_idx];
        const EdgeInsets<ValueType> padding = child->padding();
        // Make the element size with the maximum space of available containers.
        const Size<ValueType> item_size = child->measure(
            {
                std::min(maximum_container_width - padding.horizontal(), child->max_width()),
                std::min(size.height - padding.vertical(), child->max_height())
            }
        );
        // Size limit on the calculation result.
        const ValueType width = std::max(child->min_width(),
                                         std::min(item_size.width, maximum_container_width - padding.horizontal()));
        const ValueType height = std::max(child->min_height(),
                                          std::min(item_size.height, size.height - padding.vertical()));
        // The size of the element after subtracting padding is the actual size of the element.
        this->size_list.at(element_idx) = { width, height };
        // The padding needs to be taken into account when counting the actual size of the occupancy.
        measured_size.width += (width + padding.horizontal());
        measured_size.height = std::max(measured_size.height, height + padding.vertical());
    }

    this->cached_measured_size = measured_size;
    return Size<ValueType>{
        std::min(std::max(measured_size.width, this->min_width()), this->max_width()),
        std::min(std::max(measured_size.height, this->min_height()), this->max_height()),
    };
}

}

#endif //VPACKCORE_HORIZONTAL_CONTAINER_HPP
