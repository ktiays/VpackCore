//
// Created by ktiays on 2022/8/21.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_TEXT_HPP
#define VPACKCORE_TEXT_HPP

#include <cmath>

#include "some_view.hpp"

namespace vpkt {

struct Text : public SomeView {
    Text(identifier_t&& identifier, int length)
        : identifier_(std::move(identifier)), text_length_(length) {}

    __IMPL_PADDING_FOR_CONTAINER(Text)

    __IMPL_OFFSET_FOR_CONTAINER(Text)

    static constexpr vpk::Size<value_type> character_size() {
        return { 5, 8 };
    }

    vpk::LayoutablePointer<identifier_t, value_type> make_view() const override {
        const vpk::LayoutParams<value_type> params{
            { character_size().width, character_size().height,
              character_size().width * text_length_, character_size().height * text_length_ },
            padding_, offset_ };
        return std::make_shared<vpk::Item<identifier_t, value_type>>(
            identifier_,
            params,
            std::make_shared<vpk::AnyMeasurable<value_type>>(
                [this](const vpk::Size<value_type>& size) -> vpk::Size<value_type> {
                    const int number_of_char_in_line =
                        std::max(1, static_cast<int>(size.width) / static_cast<int>(character_size().width));
                    if (number_of_char_in_line >= this->text_length_)
                        return { this->text_length_ * character_size().width, character_size().height };
                    return {
                        number_of_char_in_line * character_size().width,
                        ceil(this->text_length_ / static_cast<value_type>(number_of_char_in_line)) *
                        character_size().height
                    };
                })
        );
    }

private:
    identifier_t identifier_;
    int text_length_;
    vpk::EdgeInsets<value_type> padding_;
    vpk::Point<value_type> offset_;
};

}

#endif //VPACKCORE_TEXT_HPP
