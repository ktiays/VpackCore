//
// Created by ktiays on 2022/8/21.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_SPACER_HPP
#define VPACKCORE_SPACER_HPP

#include <numeric>

#include "some_view.hpp"

namespace vpkt {

struct Spacer : public SomeView {

    constexpr Spacer() = default;

    vpk::LayoutablePointer<identifier_t, value_type> make_view() const override {
        const vpk::LayoutParams<value_type> params{
            layout_params(),
            {},
            {}, -1 };
        return std::make_shared<vpk::Item<identifier_t, value_type>>(
            spacer_identifier_prefix() + uuid(),
            params,
            std::make_shared<vpk::AnyMeasurable<value_type>>()
        );
    }

protected:
    virtual vpk::SizeProperty<value_type> layout_params() const {
        return { 0, 0, infinity, infinity };
    };
};

struct VSpacer : public Spacer {

    constexpr VSpacer()
        : Spacer() {};

    vpk::SizeProperty<value_type> layout_params() const override {
        return { 0, 0, 0, infinity };
    };
};

struct HSpacer : public Spacer {

    constexpr HSpacer()
        : Spacer() {};

    vpk::SizeProperty<value_type> layout_params() const override {
        return { 0, 0, infinity, 0 };
    };
};

}

#endif //VPACKCORE_SPACER_HPP
