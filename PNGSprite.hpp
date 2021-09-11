/**
 * @file PNGSprite.hpp
 *
 * @brief store png sprite
 *
 * @author Wenlin Mao
 * Contact: wenlinm@andrew.cmu.edu
 *
 */
#pragma once

#include <glm/glm.hpp>

class PNGSprite {
public:
    glm::uvec2 size;
    std::vector< glm::u8vec4 > pic;

    PNGSprite();
    ~PNGSprite();
};