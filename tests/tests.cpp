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
    }.offset({ 1, 1 }).compute({ 0, 0, 100, 100 });

    const LayoutResult answer{
        {
            { "A", {{ -106, 41, 20, 20 }, 0 }},
            { "B", {{ -76, 21, 10, 60 }, 0 }},
            { "C", {{ -64, 3, 100, 100 }, 0 }},
            { "D", {{ 34, -51.5, 151, 205 }, 0 }},
            { "E", {{ 185, 12, 33, 78 }, 0 }},
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
            vpkt::View("A", { 100, 40 }).make_view(),
            vpkt::View("B", { 320, 70 }).make_view(),
            vpkt::View("C", { 80, 274 })
                .offset({ 2, 2 })
                .make_view(),
            vpkt::View("D", { 100, 40 }).make_view(),
            vpkt::Spacer().make_view(),
        }
    }.alignment(vpk::HorizontalAlignment::trailing)
        .compute({ 0, 0, 350, 500 });

    const LayoutResult answer{
        {
            { "A", {{ 250, 0, 100, 40 }, 0 }},
            { "B", {{ 30, 40, 320, 70 }, 0 }},
            { "C", {{ 272, 112, 80, 274 }, 0 }},
            { "D", {{ 250, 384, 100, 40 }, 0 }},
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

TEST(VpackCoreTest, DecoratedContainer) {
    const auto result = vpkt::HStack(
        {
            vpkt::DStack(
                {
                    vpkt::InfView("Background").make_view(),
                    vpkt::Text("Text", 10)
                        .padding({ 12, 6, 12, 6 })
                        .make_view(),
                }, vpk::DecoratedStyle::background).make_view(),
            vpkt::Spacer().make_view(),
            vpkt::DStack(
                {
                    vpkt::View("View", { 50, 50 })
                        .padding({ 5, 5, 5, 5 })
                        .make_view(),
                    vpkt::View("Overlay", { 70, 70 }).make_view(),
                }, vpk::DecoratedStyle::overlay
            ).make_view(),
        }
    ).compute({ 0, 0, 200, 120 });

    const LayoutResult answer{
        {
            { "Text", {{ 12, 56, 50, 8 }, 2 }},
            { "Background", {{ 0, 29, 74, 20 }, 1 }},
            { "View", {{ 145, 35, 50, 50 }, 3 }},
            { "Overlay", {{ 135, 25, 70, 70 }, 4 }},
        }, 4
    };

    ASSERT_EQ(result, answer);
}