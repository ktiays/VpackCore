//
// Created by ktiays on 2022/8/13.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_CONTAINER_HPP
#define VPACKCORE_CONTAINER_HPP

#include <vector>
#include <memory>

#include "../layoutable.hpp"
#include "../../utils/indexed.hpp"
#include "../utils/size_extractor.hpp"

#define __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(property, policy) \
if (size_property.property.has_value()) this->property##_ = *size_property.property; \
else this->property##_ = calculate_min_max_dimension<SizeExtractor::property, Identifier, ValueType>(children, policy)

#define __DEAL_MIN_WIDTH_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(min_width, policy)
#define __DEAL_MAX_WIDTH_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(max_width, policy)
#define __DEAL_MIN_HEIGHT_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(min_height, policy)
#define __DEAL_MAX_HEIGHT_FOR_POLICY(policy) __DEAL_SIZE_FOR_POLICY_WITH_PROPERTY(max_height, policy)

namespace vpk {

template<typename Identifier, typename ValueType>
struct HVLayoutProxy;

template<typename Identifier, typename ValueType>
class Container : public Layoutable<Identifier, ValueType> {
public:
    Container(const std::vector<LayoutablePointer<Identifier, ValueType>>& items, const LayoutParams<ValueType>& params)
        : Layoutable<Identifier, ValueType>(params), children(items) {
        size_list.resize(items.size());
    }

    LayoutType type() const override { return LayoutType::container; }

protected:
    friend struct HVLayoutProxy<Identifier, ValueType>;

    std::vector<LayoutablePointer<Identifier, ValueType>> children;
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
