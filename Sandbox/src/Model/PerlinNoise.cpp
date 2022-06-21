//
// Created by Admin on 12.02.2022.
//
#include "sndbxpch.hpp"
#include "PerlinNoise.hpp"
#include <random>

float interpolate(float v1, float v2, float blend) {
    return (v2 - v1) * blend + v1;
}

void PerlinNoise::generate2D(int seedValue, int octaves, float bias, float *values, int width, int height) {
    std::srand(seedValue);
    float *seed = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        seed[i] = ((float)std::rand() / (float)RAND_MAX);
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float scale = 1.0f;
            float scaleAccumulation = 0.0f;
            values[x * width + y] = 0;
            for (int o = 0; o < octaves; o++) {
                int dist = width >> o;
                if (dist == 0) {
                    continue;
                }
                int sampleX1 = (x / dist) * dist;
                int sampleY1 = (y / dist) * dist;

                int sampleX2 = (sampleX1 + dist) % width;
                int sampleY2 = (sampleY1 + dist) % width;

                float blendX = (float)(x - sampleX1) / (float)(dist);
                float blendY = (float)(y - sampleY1) / (float)(dist);

                float sampleT = interpolate(seed[sampleY1 * width + sampleX1], seed[sampleY1 * width + sampleX2], blendX);
                float sampleB = interpolate(seed[sampleY2 * width + sampleX1], seed[sampleY2 * width + sampleX2], blendX);
                values[x * width + y] += (blendY * (sampleB - sampleT) + sampleT) * scale;

                scaleAccumulation += scale;
                scale /= bias;
            }
            values[x * width + y] /= scaleAccumulation;
        }
    }

    delete[] seed;
}
