//
// Created by ktiays on 2022/8/16.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_VIEW_HPP
#define VPACKCORE_VIEW_HPP

#include "some_view.hpp"

namespace vpkt {

class View : public SomeView {
public:
    View(identifier_t&& identifier, vpk::core::Size<value_type>&& size)
        : identifier_(std::move(identifier)), size_(size), size_property(
        {
            size.width, size.height, size.width, size.height
        }) {}

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(View)

    vpk::core::LayoutablePointer<identifier_t, value_type> make_view() const override {
        const vpk::core::LayoutParams<value_type> params{ size_property, padding_, offset_ };
        return std::make_shared<vpk::core::Item<identifier_t, value_type>>(
            identifier_,
            params,
            std::make_shared<vpk::core::AnyMeasurable<value_type>>(size_)
        );
    }

private:
    identifier_t identifier_;
    vpk::core::Size<value_type> size_;
    __DECL_LAYOUT_PARAMS;
};

class InfView : public SomeView {
public:
    InfView(identifier_t&& identifier)
        : identifier_(std::move(identifier)),
          size_property({ 0, 0, infinity, infinity }) {}

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(InfView)

    vpk::core::LayoutablePointer<identifier_t, value_type> make_view() const override {
        const vpk::core::LayoutParams<value_type> params{ size_property, padding_, offset_ };
        return std::make_shared<vpk::core::Item<identifier_t, value_type>>(
            identifier_,
            params,
            std::make_shared<vpk::core::AnyMeasurable<value_type>>()
        );
    }

private:
    identifier_t identifier_;
    __DECL_LAYOUT_PARAMS;
};

}

#endif //VPACKCORE_VIEW_HPP
