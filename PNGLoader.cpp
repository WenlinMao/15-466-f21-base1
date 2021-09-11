/**
 * @file PNGLoader.cpp
 *
 * @brief loader to load png file
 *
 * @author Wenlin Mao
 * Contact: wenlinm@andrew.cmu.edu
 *
 */

#include "PNGLoader.hpp"

PNGLoader::PNGLoader(){}
PNGLoader::~PNGLoader(){}

void PNGLoader::load(const std::string & path, PNGSprite& sprite){
    load_png(data_path(path), &(sprite.size), &(sprite.pic), LowerLeftOrigin); 
}