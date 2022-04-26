//
// Created by Admin on 12.02.2022.
//

#pragma once

struct Voxel {
    int8_t id;

    Voxel()
        : id(0) {}

    explicit Voxel(int8_t id)
        : id(id) {}
};
