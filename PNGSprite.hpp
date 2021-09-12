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
#include <vector>
#include "PPU466.hpp"



#define PNG_SIZE 32
#define BACKGROUND_SIZE 64

class PNGSprite {
public:
    PNGSprite();
    PNGSprite(uint8_t c_idx, uint8_t t_idx, glm::uvec2 default_pos) : color_pallete_index(c_idx), tile_table_index(t_idx), pos(default_pos) {}
    ~PNGSprite();

    void Initialize_PNG(PPU466& ppu, uint8_t priority);
    void Initialize_Background(PPU466& ppu);

    // Pos is at the middle of 4x4 tiles
    glm::uvec2 pos;
    glm::uvec2 size;
    std::vector< glm::u8vec4 > pic;

    //Only load 32x32 png, which contains 16 tiles/sprites
    std::array<PPU466::Sprite, (PNG_SIZE*PNG_SIZE) / (8 * 8)> png_sprites;
    int png_sprites_index = 0;

private:
    uint8_t color_pallete_index;
    uint8_t tile_table_index;
    uint8_t background_tile_table_index = 16 * 16 - BACKGROUND_SIZE;
    uint8_t background_color_pallete_index = 7;
    void Register_Background(PPU466& ppu);
    void Fill_color_pallete(PPU466& ppu, bool is_background);
    void Register_PNG(PPU466& ppu, uint8_t priority);
    uint8_t Find_color(const PPU466::Palette& pallete, const glm::u8vec4& color);
    void Fill_Tile(const PPU466::Palette& pallete, PPU466::Tile& tile, uint8_t row, uint8_t col);
    void Fill_Sprite(uint8_t t_index, uint8_t prority, uint8_t row, uint8_t col);
    void Set_Tile_Bit(PPU466::Tile& tile, uint8_t row, uint8_t col, uint8_t color_idx);
};