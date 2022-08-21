//
// Created by ktiays on 2022/8/16.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#include <iostream>
#include <vector>

#include "../VpackCore.hpp"
#include "../tests/src/view.hpp"
#include "../tests/src/container.hpp"

int main() {
    const auto result = vpkt::ZStack{
        {
            vpkt::View("view", { 20, 20 }).make_view(),
            vpkt::View("view2", { 10, 60 }).make_view(),
        }
    }.compute({ 0, 0, 100, 100 });

    for (auto& it: result.map) {
        const auto frame = it.second.frame;
        std::cout << it.first << "(" << it.second.z_idx << "): (";
        std::cout << frame.x << ", " << frame.y << ", " << frame.width << ", " << frame.height;
        std::cout << ")\n";
    }

    return 0;
}
