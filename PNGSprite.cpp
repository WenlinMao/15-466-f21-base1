#include "PNGSprite.hpp"
#include <iostream>
#include <bitset>

PNGSprite::PNGSprite() {}
PNGSprite::~PNGSprite() {}

// bool pallate_equal(const glm::u8vec4& a, const glm::u8vec4& b){
//     const uint8_t epsilion = 10;  // choose something apprpriate.

//     return    abs(a[0] - b[0]) < epsilion   
//             && abs(a[1] - b[1]) < epsilion   
//             && abs(a[2] - b[2]) < epsilion;
// }
    
void PNGSprite::Initialize_PNG(PPU466& ppu, uint8_t priority) {
	Fill_color_pallete(ppu, false);
	Register_PNG(ppu, priority);
}

void PNGSprite::Initialize_Background(PPU466& ppu) {
	Fill_color_pallete(ppu, true);
	Register_Background(ppu);
}

void PNGSprite::Register_Background(PPU466& ppu) {
	PPU466::Palette& pallete = ppu.palette_table[background_color_pallete_index];
	std::array<PPU466::Tile, 16 * 16>& tile_table = ppu.tile_table;

	// Num of tiles for each background col/row
	uint8_t width = BACKGROUND_SIZE / 8;
	uint8_t height = BACKGROUND_SIZE / 8;

	// For each background file
	for (uint8_t i = 0; i < height; i++)
	{
		for (uint8_t j = 0; j < width; j++)
		{
			uint8_t curr_table_index = background_tile_table_index + i * width + j;
			PPU466::Tile& tile = tile_table[curr_table_index];
			for (uint8_t x = 0; x < 8; x++)
			{
				for (uint8_t y = 0; y < 8; y++)
				{
					int pic_index = (i * 8 + x) * BACKGROUND_SIZE + (j * 8 + y);
					uint8_t color_index = Find_color(pallete, pic[pic_index]);
					Set_Tile_Bit(tile, x, y, color_index);
				}
			}
		}
	}

	// Set ppu's background value
	uint16_t background_value = background_color_pallete_index << 8;
	for (uint32_t i=0; i<ppu.BackgroundHeight; i++) {
		for (uint32_t j=0; j < ppu.BackgroundWidth; j++) {
			int idx = i * ppu.BackgroundWidth + j;
			int tile_idx = (i % 8) * 8 + (j % 8) + background_tile_table_index;
			ppu.background[idx] = background_value | tile_idx;
			//std::cout << "background actual value = " << std::bitset<16>((int)ppu.background[idx]) << std::endl;
		}
	}
}

void PNGSprite::Fill_color_pallete(PPU466& ppu, bool is_background) {
	uint8_t pallete_index = 0;
	uint8_t idx = is_background ? background_color_pallete_index : color_pallete_index;
	//std::cout << "color pallete idx = " << (int)idx << std::endl;
	PPU466::Palette& pallete = ppu.palette_table[idx];
	for (unsigned long int i=0; i < pic.size(); i++) {
		bool in_pallete = false;
		for (uint8_t j=0; j <= pallete_index; j++) {
            
			if (pic[i] == pallete[j]) {
				in_pallete = true;
				break;
			}
		}
		if (!in_pallete){ 
			pallete[pallete_index++] = pic[i];
			std::cout << (int)pic[i][0] << " " << (int)pic[i][1] << " " << (int)pic[i][2] << " " << (int)pic[i][3] << std::endl;
		}

		// Max size of pallete is 4
		if (pallete_index >= 4)
			return;
	}
}

void PNGSprite::Register_PNG(PPU466& ppu, uint8_t priority) {
	PPU466::Palette& pallete = ppu.palette_table[color_pallete_index];
	std::array<PPU466::Tile, 16 * 16>& tile_table = ppu.tile_table;

	// Num of tiles for each png col/row
	uint8_t width = PNG_SIZE / 8;
	uint8_t height = PNG_SIZE / 8;

	// For each png file
	for (uint8_t i=0; i<height; i++){ 
		for (uint8_t j=0; j<width; j++) {
			// For each 8x8 tile
			uint8_t curr_table_index = tile_table_index + i * width + j;
			PPU466::Tile& tile = tile_table[curr_table_index];
			Fill_Tile(pallete, tile, i, j);
			Fill_Sprite(curr_table_index, priority, i, j);
		}
	}
}

// Inspired by Oscar's Idea of only store and retrieve 4 colors, otherwise use the first one
uint8_t PNGSprite::Find_color(const PPU466::Palette& pallete, const glm::u8vec4& color) {
	for (unsigned long int i=0; i<pallete.size(); i++) { 
		if (pallete[i] == color)
			return static_cast<uint8_t>(i);
	}
	return 0;
}

void PNGSprite::Fill_Tile(const PPU466::Palette& pallete, PPU466::Tile& tile, uint8_t row, uint8_t col) {
	for (uint8_t x = 0; x < 8; x++) {
		for (uint8_t y = 0; y < 8; y++) {
			int pic_index = (row * 8 + x) * PNG_SIZE + (col * 8 + y);
			//int pic_index = (row * PNG_SIZE/8 + col) * 64 + 8*x + y;
			uint8_t color_index = Find_color(pallete, pic[pic_index]);
			Set_Tile_Bit(tile, x, y, color_index);
		}
	}
}

void PNGSprite::Update_Pos(glm::uvec2 new_pos) {
	pos = new_pos;
	// Num of tiles for each png col/row
	uint8_t width = PNG_SIZE / 8;
	uint8_t height = PNG_SIZE / 8;

	int sprite_idx = 0;
	for (uint8_t i = 0; i < height; i++) {
		for (uint8_t j = 0; j < width; j++) {
			png_sprites[sprite_idx].x = pos.x - PNG_SIZE / 2 + j * 8 + 8 / 2;
			png_sprites[sprite_idx].y = pos.y - PNG_SIZE / 2 + i * 8 + 8 / 2;
			sprite_idx++;
		}
	}
}

void PNGSprite::Fill_Sprite(uint8_t t_index, uint8_t prority, uint8_t row, uint8_t col) {
	PPU466::Sprite sprite;

	sprite.index = t_index;
	
	// Set up attribute
	sprite.attributes = prority << 7;
	sprite.attributes |= color_pallete_index;

	// Update related pos
	sprite.x = pos.x - PNG_SIZE/2 + col * 8 + 8 / 2;
	sprite.y = pos.y - PNG_SIZE/2 + row * 8 + 8 / 2;

	//std::cout << (int)sprite.x << " " << (int)sprite.y << std::endl; 

	assert(png_sprites_index < (PNG_SIZE * PNG_SIZE) / (8 * 8));
	png_sprites[png_sprites_index++] = sprite;	
}

void PNGSprite::Set_Tile_Bit(PPU466::Tile& tile, uint8_t row, uint8_t col, uint8_t color_idx) {
	uint8_t n_bit0 = color_idx & 1;
	uint8_t n_bit1 = (color_idx >> 1) & 1;

	// Initialize
	uint16_t clean = ~(1 << col);
	tile.bit0[row] &= clean;
	tile.bit1[row] &= clean;

	// Set Value
	tile.bit0[row] |= n_bit0 << col;
	tile.bit1[row] |= n_bit1 << col;
}