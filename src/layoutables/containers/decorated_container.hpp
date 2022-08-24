//
// Created by ktiays on 2022/8/24.
// Copyright (c) 2022 ktiays. All rights reserved.
//

#ifndef VPACKCORE_DECORATED_CONTAINER_HPP
#define VPACKCORE_DECORATED_CONTAINER_HPP

#include "stack_container.hpp"

namespace vpk {

template<typename Identifier, typename ValueType>
class DecoratedContainer : public StackContainer<Identifier, ValueType> {
public:
    DecoratedContainer()

};

}

#endif //VPACKCORE_DECORATED_CONTAINER_HPP
