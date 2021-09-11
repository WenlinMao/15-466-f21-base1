/**
 * @file PNGLoader.hpp
 *
 * @brief loader to load png file
 *
 * @author Wenlin Mao
 * Contact: wenlinm@andrew.cmu.edu
 *
 */
#pragma once

#include "load_save_png.hpp"
#include "PNGSprite.hpp"
#include "data_path.hpp"

class PNGLoader {
    PNGLoader();
    ~PNGLoader();

    void load(const std::string & path, PNGSprite& sprite);
};