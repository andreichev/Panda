//
// Created by Michael Andreichev on 21.09.2023.
//

#pragma once

#include "Voxel.hpp"

#include <cstdint>
#include <Panda.hpp>

struct VoxelTextureData {
    VoxelType type;
    uint8_t sideTileIndex;
    uint32_t sideColor;
    uint8_t topTileIndex;
    uint32_t topColor;
    uint8_t bottomTileIndex;
    uint32_t bottomColor;

    VoxelTextureData(VoxelType type, uint8_t tileIndex)
        : type(type)
        , sideTileIndex(tileIndex)
        , sideColor(0xFFFFFFFF)
        , topTileIndex(tileIndex)
        , topColor(0xFFFFFFFF)
        , bottomTileIndex(tileIndex)
        , bottomColor(0xFFFFFFFF) {}

    VoxelTextureData(VoxelType type,
        uint8_t sideTileIndex,
        uint32_t sideColor,
        uint8_t topTileIndex,
        uint32_t topColor,
        uint8_t bottomTileIndex,
        uint32_t bottomColor)
        : type(type)
        , sideTileIndex(sideTileIndex)
        , sideColor(sideColor)
        , topTileIndex(topTileIndex)
        , topColor(topColor)
        , bottomTileIndex(bottomTileIndex)
        , bottomColor(bottomColor) {}
};

class VoxelTextureMapper final {
public:
    static VoxelTextureData &getTextureData(Voxel *voxel) {
        VoxelTextureData &data = s_data[static_cast<int>(voxel->type)];
        PND_ASSERT(data.type == voxel->type, "CHECK VOXEL ID");
        return data;
    }

private:
    static VoxelTextureData s_data[];
};

VoxelTextureData VoxelTextureMapper::s_data[] = {
    // ВОЗДУХ
    VoxelTextureData(VoxelType::NOTHING, 0),
    // ТРАВА
    VoxelTextureData(VoxelType::GRASS, 2, 0xFFFFFFFF, 3, 0x9EEA6CFF, 1, 0xFFFFFFFF),
    // ЗЕМЛЯ
    VoxelTextureData(VoxelType::GROUND, 1),
    // ДЕРЕВО
    VoxelTextureData(VoxelType::TREE, 5, 0xFFFFFFFF, 6, 0xFFFFFFFF, 6, 0xFFFFFFFF),
    // ДОСКИ
    VoxelTextureData(VoxelType::BOARDS, 7),
    // КАМЕНЬ
    VoxelTextureData(VoxelType::STONE, 8),
    // КАМЕННЫЕ КИРПИЧИ
    VoxelTextureData(VoxelType::STONE_BRICKS, 9),
    // ПЕСЧАНИК
    VoxelTextureData(VoxelType::SAND_STONE, 10),
    // ПЕСОК
    VoxelTextureData(VoxelType::SAND, 11)};