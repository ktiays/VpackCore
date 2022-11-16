//
// Created by ktiays on 2022/8/16.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_TESTS_CONTAINER_HPP
#define VPACKCORE_TESTS_CONTAINER_HPP

#include <utility>
#include <vector>

#include "some_view.hpp"

//////////////////////////////// VStack ////////////////////////////////

namespace vpkt {

template<typename T = decltype(std::declval<SomeView>().make_view())>
class VStack : public SomeView {
public:
    VStack(std::vector<T>&& v)
        : items_(std::move(v)), alignment_(vpk::core::HorizontalAlignment::center) {}

    VStack& alignment(vpk::core::HorizontalAlignment align) {
        alignment_ = align;
        return *this;
    }

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(VStack)

    T make_view() const override {
        return std::make_shared<vpk::core::VerticalContainer<identifier_t, value_type>>(
            items_, vpk::core::LayoutParams<double>{
                size_property, padding_, offset_
            }, alignment_
        );
    }

private:
    std::vector<T> items_;
    vpk::core::HorizontalAlignment alignment_;
    __DECL_LAYOUT_PARAMS;
};

}

//////////////////////////////// HStack ////////////////////////////////

namespace vpkt {

template<typename T = decltype(std::declval<SomeView>().make_view())>
class HStack : public SomeView {
public:
    HStack(std::vector<T>&& v)
        : items_(std::move(v)), alignment_(vpk::core::VerticalAlignment::center) {}

    HStack& alignment(vpk::core::VerticalAlignment align) {
        alignment_ = align;
        return *this;
    }

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(HStack)

    T make_view() const override {
        return std::make_shared<vpk::core::HorizontalContainer<identifier_t, value_type>>(
            items_, vpk::core::LayoutParams<double>{
                size_property, padding_, offset_
            }, alignment_
        );
    }

private:
    std::vector<T> items_;
    vpk::core::VerticalAlignment alignment_;
    __DECL_LAYOUT_PARAMS;
};

}

//////////////////////////////// ZStack ////////////////////////////////

namespace vpkt {

template<typename T = decltype(std::declval<SomeView>().make_view())>
class ZStack : public SomeView {
public:
    ZStack(std::vector<T>&& v)
        : items_(std::move(v)), alignment_(vpk::core::Alignment::center) {}

    ZStack& alignment(vpk::core::Alignment align) {
        alignment_ = align;
        return *this;
    }

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(ZStack)

    T make_view() const override {
        return std::make_shared<vpk::core::StackContainer<identifier_t, value_type>>(
            items_, vpk::core::LayoutParams<double>{}, alignment_
        );
    }

private:
    std::vector<T> items_;
    vpk::core::Alignment alignment_;
    __DECL_LAYOUT_PARAMS;
};

}

//////////////////////////////// Decorated Stack ////////////////////////////////

namespace vpkt {

template<typename T = decltype(std::declval<SomeView>().make_view())>
class DStack : public SomeView {
public:
    DStack(std::vector<T>&& v, vpk::core::DecoratedStyle style)
        : items_(v), style_(style) {}

    __IMPL_LAYOUT_PARAMS_FOR_CONTAINER(DStack)

    T make_view() const override {
        return std::make_shared<vpk::core::DecoratedContainer<identifier_t, value_type>>(
            items_, vpk::core::LayoutParams<double>{}, style_
        );
    }

private:
    std::vector<T> items_;
    vpk::core::DecoratedStyle style_;
    __DECL_LAYOUT_PARAMS;
};

}

#endif //VPACKCORE_TESTS_CONTAINER_HPP
