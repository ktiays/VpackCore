//
// Created by ktiays on 2022/8/13.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_CONTAINER_HPP
#define VPACKCORE_CONTAINER_HPP

#include <map>
#include <vector>
#include <utility>
#include <algorithm>

#include "../layoutable.hpp"
#include "../../utils/indexed.hpp"
#include "../utils/size_extractor.hpp"

#define __DEAL_MIN_WIDTH_ELSE(property) \
if (size_property.property.has_value()) this->property##_ = *size_property.property; \
else this->property##_ =

#define __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(property, policy) \
__DEAL_MIN_WIDTH_ELSE(property) calculate_min_max_dimension<SizeExtractor::property, Identifier, ValueType>(children, policy)

#define __DEAL_MIN_WIDTH_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(min_width, policy)
#define __DEAL_MAX_WIDTH_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(max_width, policy)
#define __DEAL_MIN_HEIGHT_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(min_height, policy)
#define __DEAL_MAX_HEIGHT_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(max_height, policy)

namespace vpk {

template<typename Identifier, typename ValueType>
class Container : public Layoutable<Identifier, ValueType> {
private:
    using ElementPointer = LayoutablePointer<Identifier, ValueType>;
    using ElementListType = std::vector<ElementPointer>;
    using ElementSizeType = typename ElementListType::size_type;

public:
    Container(const std::vector<LayoutablePointer<Identifier, ValueType>>& items, const LayoutParams<ValueType>& params)
        : Layoutable<Identifier, ValueType>(params), children(items) {
        size_list.resize(items.size());

        std::vector<ElementPointer> copied_items = items;
        std::sort(copied_items.begin(), copied_items.end(), [](const ElementPointer& x, const ElementPointer& y) {
            return x->params.priority < y->params.priority;
        });

        optional<int> current_priority = nullopt;
        typename decltype(children_priority_map)::mapped_type* current_elements_ptr = nullptr;
        for (auto it: makeIndexed(copied_items)) {
            const ElementPointer& ptr = it.value();
            const int priority = ptr->params.priority;
            if (!current_priority.has_value() || *current_priority != priority) {
                current_priority = priority;
                current_elements_ptr = &children_priority_map[priority];
            }
            current_elements_ptr->push_back(std::make_pair(it.index(), ptr));
        }
    }

    LayoutType type() const override { return LayoutType::container; }

protected:
    ElementListType children;
    std::map<int, std::vector<std::pair<ElementSizeType, ElementPointer>>> children_priority_map;
    // The size list of the element calculated by the cache.
    // The size indicates the actual display size of the element, i.e., the size without padding.
    std::vector<Size<ValueType>> size_list;

    /// A cache of the results of the last size calculation.
    ///
    /// It represents the actual total size that all child elements need to occupy.
    /// Default value of this property is 0.
    Size<ValueType> cached_measured_size;
};

}

#endif //VPACKCORE_CONTAINER_HPP
