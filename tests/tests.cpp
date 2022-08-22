//
// Created by ktiays on 2022/8/17.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#include <algorithm>

#include "gtest/gtest.h"

#include "src/view.hpp"
#include "src/text.hpp"
#include "src/spacer.hpp"
#include "src/container.hpp"

using Identifier = vpkt::SomeView::identifier_t;
using ValueType = vpkt::SomeView::value_type;
using LayoutResult = vpk::LayoutResult<Identifier, ValueType>;

TEST(VpackCoreTest, HorizontalContainer) {
    const auto result = vpkt::HStack{
        {
            vpkt::View("A", { 20, 20 })
                .padding({ 10, 10, 10, 10 })
                .make_view(),
            vpkt::View("B", { 10, 60 }).make_view(),
            vpkt::View("C", { 100, 100 })
                .offset({ 2, 2 })
                .make_view(),
            vpkt::HStack{
                {
                    vpkt::View("D", { 151, 205 }).make_view(),
                    vpkt::View("E", { 33, 78 }).make_view(),
                    vpkt::Spacer().make_view(),
                }
            }.make_view()

        }
    }.compute({ 0, 0, 100, 100 });

    const LayoutResult answer{
        {
            { "A", {{ -107, 40, 20, 20 }, 0 }},
            { "B", {{ -77, 20, 10, 60 }, 0 }},
            { "C", {{ -65, 2, 100, 100 }, 0 }},
            { "D", {{ 33, -52.5, 151, 205 }, 0 }},
            { "E", {{ 184, 11, 33, 78 }, 0 }},
        }, 0
    };

    ASSERT_EQ(result, answer);
}

TEST(VpackCoreTest, HorizontalText) {
    const auto result = vpkt::HStack{
        {
            vpkt::Text("A", 10).make_view(),
            vpkt::Text("B", 40).make_view(),
        }
    }.alignment(vpk::VerticalAlignment::top)
        .compute({ 0, 0, 200, 200 });

    const LayoutResult answer{
        {
            { "A", {{ 0, 92, 50, 8 }, 0 }},
            { "B", {{ 50, 92, 150, 16 }, 0 }},
        }, 0
    };

    ASSERT_EQ(result, answer);
}

TEST(VpackCoreTest, VerticalContainer) {
    const auto result = vpkt::VStack{
        {
            vpkt::View("A", { 20, 20 }).make_view(),
            vpkt::View("B", { 10, 60 }).make_view(),
            vpkt::View("C", { 100, 100 })
                .offset({ 1, 1 })
                .make_view(),
        }
    }.alignment(vpk::HorizontalAlignment::trailing)
        .compute({ 0, 0, 200, 200 });

    const LayoutResult answer{
        {
            { "A", {{ 130, 10, 20, 20 }, 0 }},
            { "B", {{ 140, 30, 10, 60 }, 0 }},
            { "C", {{ 51, 91, 100, 100 }, 0 }},
        }, 0
    };

    ASSERT_EQ(result, answer);
}

TEST(VpackCoreTest, StackContainer) {
    const auto result = vpkt::ZStack{
        {
            vpkt::View("A", { 20, 20 }).make_view(),
            vpkt::View("B", { 10, 60 }).make_view(),
            vpkt::View("C", { 100, 100 }).make_view(),
            vpkt::View("D", { 151, 205 }).make_view(),
            vpkt::View("E", { 33, 78 }).make_view(),
        }
    }.compute({ 0, 0, 100, 100 });

    const LayoutResult answer{
        {
            { "A", {{ 40, 40, 20, 20 }, 1 }},
            { "B", {{ 45, 20, 10, 60 }, 2 }},
            { "C", {{ 0, 0, 100, 100 }, 3 }},
            { "D", {{ -25.5, -52.5, 151, 205 }, 4 }},
            { "E", {{ 33.5, 11, 33, 78 }, 5 }},
        }, 5
    };

    ASSERT_EQ(result, answer);
}