//
// Created by ktiays on 2022/8/16.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_VIEW_HPP
#define VPACKCORE_VIEW_HPP

#include "some_view.hpp"

namespace vpkt {

struct View : public SomeView {
    View(identifier_t&& identifier, vpk::Size<value_type>&& size)
        : identifier_(std::move(identifier)), size_(size), size_property(
        {
            size.width, size.height, size.width, size.height
        }) {}

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(View)

    vpk::LayoutablePointer<identifier_t, value_type> make_view() const override {
        const vpk::LayoutParams<value_type> params{ size_property, padding_, offset_ };
        return std::make_shared<vpk::Item<identifier_t, value_type>>(
            identifier_,
            params,
            std::make_shared<vpk::AnyMeasurable<value_type>>(size_)
        );
    }

private:
    identifier_t identifier_;
    vpk::Size<value_type> size_;
    __DECL_LAYOUT_PARAMS;
};

}

#endif //VPACKCORE_VIEW_HPP
