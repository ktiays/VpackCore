//
// Created by ktiays on 2022/8/22.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_HV_CONTAINER_HPP
#define VPACKCORE_HV_CONTAINER_HPP

#include "container.hpp"

namespace vpk::detail {

template<typename ValueType>
struct AxisSize {
    ValueType main;
    ValueType cross;

    constexpr AxisSize() = default;

    constexpr AxisSize(const ValueType& main, const ValueType& cross)
        : main(main), cross(cross) {}
};

template<typename ValueType>
struct AxisEdgeInsets {
    ValueType main_start;
    ValueType main_end;
    ValueType cross_start;
    ValueType cross_end;

    constexpr AxisEdgeInsets() = default;

    constexpr AxisEdgeInsets(const ValueType& main_start, const ValueType& main_end, const ValueType& cross_start,
                             const ValueType& cross_end)
        : main_start(main_start), main_end(main_end), cross_start(cross_start), cross_end(cross_end) {}

    ValueType main() const {
        return main_start + main_end;
    }

    ValueType cross() const {
        return cross_start + cross_end;
    }
};

template<typename Identifier, typename ValueType>
struct HVContainer : public vpk::Container<Identifier, ValueType> {
protected:
    using Element = LayoutablePointer<Identifier, ValueType>;

    using SizeType = AxisSize<ValueType>;

    /// Subclasses need to implement this method to provide information
    /// about the main and cross axes of the edge insets.
    ///
    /// \brief Returns the corresponding insets instance according to the current layout direction.
    ///
    /// \param element Pointer to a subclass of Layoutable.
    /// \return Instances of AxisEdgeInsets.
    virtual AxisEdgeInsets<ValueType> axis_edge_insets_for_element(Element element) const = 0;

    virtual ValueType min_main_for_element(Element element) const = 0;

    virtual ValueType max_main_for_element(Element element) const = 0;

    virtual ValueType min_cross_for_element(Element element) const = 0;

    virtual ValueType max_cross_for_element(Element element) const = 0;

    virtual SizeType axis_measure_for_element(Element element, const SizeType& size) const = 0;

    /// \brief Abstract measure method for horizontal and vertical containers.
    ///
    /// The calculation process does not care about the layout direction of the actual container.
    /// It uses only the main axis and the cross axis as base for the calculation.
    ///
    /// \param size The size of the container provided.
    /// \param size_handler Callback when the element size calculation is done.
    /// \return The total size to be occupied by all elements in the container.
    SizeType axis_measure(const SizeType& size,
                          std::function<SizeType(const SizeType&)>&& size_handler) const;
};

template<typename Identifier, typename ValueType>
AxisSize<ValueType>
HVContainer<Identifier, ValueType>::axis_measure(const AxisSize<ValueType>& size,
                                                 std::function<SizeType(const SizeType&)>&& size_handler) const {
    SizeType measured_size;

    using usize = typename decltype(this->children)::size_type;
    /// A map to record the width allocation of each element.
    std::unordered_map<usize, ValueType> main_map;
    std::vector<std::pair<usize, ValueType>> maximum_main_list;
    maximum_main_list.reserve(this->children.size());

    for (auto it: makeIndexed(this->children)) {
        maximum_main_list.push_back(std::make_pair(it.index(), max_main_for_element(it.value())));
    }

    // Sort the elements in ascending order by the maximum space required.
    std::sort(maximum_main_list.begin(), maximum_main_list.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    const usize list_count = maximum_main_list.size();
    for (auto it: makeIndexed(maximum_main_list)) {
        const usize index = it.index();

        const ValueType rest_main = size.main - measured_size.main;
        /// The number of remaining elements that need to be allocated additional space.
        const usize count = list_count - index;
        // The remaining elements share the remaining space equally,
        // i.e., the space occupied by each element is not allowed to exceed this value.
        const ValueType maximum_container_main = rest_main / count;
        /// The position of the element instance in the children list.
        const usize element_idx = it.value().first;
        const auto child = this->children[element_idx];
        const AxisEdgeInsets<ValueType> padding = axis_edge_insets_for_element(child);
        // Make the element size with the maximum space of available containers.
        const SizeType item_size = axis_measure_for_element(
            child,
            {
                std::min(maximum_container_main - padding.main(),
                         max_main_for_element(child)),
                std::min(size.cross - padding.cross(),
                         max_cross_for_element(child))
            }
        );
        // Size limit on the calculation result.
        const ValueType main = std::max(min_main_for_element(child),
                                        std::min(item_size.main, maximum_container_main - padding.main()));
        const ValueType cross = std::max(min_cross_for_element(child),
                                         std::min(item_size.cross, size.cross - padding.cross()));
        // The padding needs to be taken into account when counting the actual size of the occupancy.
        measured_size.main += (main + padding.main());
        measured_size.cross = std::max(measured_size.cross, cross + padding.cross());
    }

    return measured_size;
}

}

#endif //VPACKCORE_HV_CONTAINER_HPP
