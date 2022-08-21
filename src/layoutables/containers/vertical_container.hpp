//
// Created by ktiays on 2022/8/14.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_VERTICAL_CONTAINER_HPP
#define VPACKCORE_VERTICAL_CONTAINER_HPP

#include <algorithm>

#include "container.hpp"
#include "../../utils/indexed.hpp"

namespace vpk {

template<typename Identifier, typename ValueType>
class VerticalContainer : public Container<Identifier, ValueType> {
public:
    VerticalContainer(const std::vector<LayoutablePointer<Identifier, ValueType>>& children,
                      const LayoutParams<ValueType>& params, HorizontalAlignment align)
        : Container<Identifier, ValueType>(children, params), alignment(align) {
        const SizeProperty<ValueType> size_property = params.size_property;
        __DEAL_MIN_WIDTH_FOR_POLICY(MinMaxPolicy::max);
        __DEAL_MAX_WIDTH_FOR_POLICY(MinMaxPolicy::max);
        __DEAL_MIN_HEIGHT_FOR_POLICY(MinMaxPolicy::sum);
        __DEAL_MAX_HEIGHT_FOR_POLICY(MinMaxPolicy::sum);
    }

    HorizontalAlignment alignment;

    void layout(const Rect<ValueType>& frame, LayoutResult<Identifier, ValueType>& result) const override;

    Size<ValueType> measure(const Size<ValueType>& size) override;
};

template<typename Identifier, typename ValueType>
void VerticalContainer<Identifier, ValueType>::layout(const Rect<ValueType>& frame,
                                                      LayoutResult<Identifier, ValueType>& result) const {
    ValueType used_height = 0;
    for (int i = 0; i < this->children.size(); ++i) {
        const auto child_ptr = this->children[i];

        const auto item_size = this->size_list[i];
        const auto x_offset = [this, &frame, &item_size]() -> ValueType {
            switch (this->alignment) {
                case HorizontalAlignment::leading:
                    return 0;
                case HorizontalAlignment::center:
                    return (frame.width - item_size.width) / 2;
                case HorizontalAlignment::trailing:
                    return frame.width - item_size.width;
            }
        }();
        const auto item_padding = child_ptr->padding();
        const auto item_offset = child_ptr->offset();
        const auto layout_frame_for_child = Rect<ValueType>(
            frame.x + x_offset + item_offset.x + item_padding.left,
            frame.y + used_height + item_offset.y + item_padding.top,
            item_size.width - item_padding.horizontal(),
            item_size.height - item_padding.vertical()
        );
        child_ptr->layout(layout_frame_for_child, result);
        used_height += item_size.height;
    }
}

template<typename Identifier, typename ValueType>
Size<ValueType> VerticalContainer<Identifier, ValueType>::measure(const Size<ValueType>& s) {
    const auto size = Size<ValueType>(
        std::max(s.width, this->min_width()),
        std::max(s.height, this->min_height())
    );
    Size<ValueType> measured_size;

    using usize = typename decltype(this->children)::size_type;
    std::unordered_map<usize, Size<ValueType>> size_map;
    std::vector<std::pair<usize, ValueType>> greedy_height_list;

    for (auto it: makeIndexed(this->children)) {
        const auto child = it.value();
        Size<ValueType> pre_child_size;
        const auto child_padding = child->padding();
        if (child->max_height().has_value()) {
            pre_child_size = Size<ValueType>{
                0,
                child->min_width() + child_padding.horizontal(),
            };
            greedy_height_list.push_back(std::make_pair(it.index(), *(child->max_height())));
        } else {
            const Size<ValueType> m_size = child->measure(Size<ValueType>{
                std::max(
                    std::min(child->max_width().value_or(size.width), size.width) - child_padding.horizontal(),
                    static_cast<ValueType>(0)),
                std::max(size.height - child_padding.vertical(), static_cast<ValueType>(0)),
            });
            pre_child_size = Size<ValueType>{
                m_size.width + child_padding.horizontal(),
                m_size.height + child_padding.vertical(),
            };
        }
        size_map[it.index()] = pre_child_size;
        measured_size.height += pre_child_size.height;
    }

    if (measured_size.height < size.height) {
        std::sort(greedy_height_list.begin(), greedy_height_list.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        for (auto it: makeIndexed(greedy_height_list)) {
            ValueType rest_height = size.height - measured_size.height;
            if (rest_height <= 0) break;
            const auto index = it.value().first;
            rest_height += [&greedy_height_list, &size_map, &it, &index]() {
                ValueType sum = 0;
                for (auto iter = greedy_height_list.begin() + it.index(); iter < greedy_height_list.end(); ++iter) {
                    sum += size_map.at(index).height;
                }
                return sum;
            }();
            const auto& child = this->children[index];
            const auto child_padding = child->padding();

            const ValueType alloc_height = rest_height / (greedy_height_list.size() - 1) - child_padding.vertical();
            const ValueType calculated_width =
                std::min(child->max_width().value_or(size.width), size.width) - child_padding.horizontal();

            Size<ValueType> child_size = Size<ValueType>{
                std::max(calculated_width, static_cast<ValueType>(0)),
                std::max(std::min(it.value().second, alloc_height), static_cast<ValueType>(0)),
            };
            const ValueType calculated_height = size_map.at(index).height;
            child_size = Size<ValueType>{
                child_size.width + child_padding.horizontal(),
                std::max(calculated_height,
                         std::min(child_size.height, alloc_height) + child_padding.vertical()),
            };
            size_map[index] = child_size;
            measured_size.height += (child_size.height - calculated_height);
        }
    }

    measured_size.width = std::max_element(size_map.cbegin(), size_map.cend(),
                                           [](const std::pair<usize, Size<ValueType>>& a,
                                              const std::pair<usize, Size<ValueType>>& b) {
                                               return a.second.width < b.second.width;
                                           })->second.width;
    for (const auto& it: size_map) {
        const usize index = it.first;
        const Size<ValueType> child_size = it.second;

        auto child = this->children[index];
        const EdgeInsets<ValueType> child_padding = child->padding();
        this->size_list.at(index) = Size<ValueType>{
            std::max(child_size.width - child_padding.horizontal(), child->min_width()) +
            child_padding.horizontal(),
            std::max(child_size.height - child_padding.vertical(),
                     child->min_height() + child_padding.vertical()),
        };
    }
    return Size<ValueType>{
        std::min(measured_size.width, size.width),
        std::min(measured_size.height, size.height),
    };
}

}

#endif //VPACKCORE_VERTICAL_CONTAINER_HPP
