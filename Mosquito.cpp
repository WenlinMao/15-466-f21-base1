#include "Mosquito.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <random>

Mosquito::Mosquito() {
	// clear ppu fields
	for (auto& pallete : ppu.palette_table) {
		pallete = {
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		};
	}

	for (auto& tile : ppu.tile_table) {
		tile = {
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
		};
	}

	load_resource();
}

Mosquito::~Mosquito() {
}

void Mosquito::load_resource() {
	std::string png_path = "../resource/heart.png";
	heart_pic = PNGSprite(0, 0, glm::uvec2(24, 220));
	PNGLoader::load(png_path, heart_pic);
	heart_pic.Initialize_PNG(ppu, 0);

	std::string background_path = "../resource/frame.png";
	background_pic = PNGSprite();
	PNGLoader::load(background_path, background_pic);
	background_pic.Initialize_Background(ppu);

	flyswatter_pic = PNGSprite(1, 16, glm::uvec2(124, 122));
	PNGLoader::load("../resource/flyswatter32.png", flyswatter_pic);
	flyswatter_pic.Initialize_PNG(ppu, 0);

	mosquito_pic = PNGSprite(2, 32, glm::uvec2(124, 122));
	PNGLoader::load("../resource/mos.png", mosquito_pic);
	mosquito_pic.Initialize_PNG(ppu, 0);

	blood_pic = PNGSprite(3, 48, glm::uvec2(124, 122));
	PNGLoader::load("../resource/blood32.png", blood_pic);
	blood_pic.Initialize_PNG(ppu, 0);
}

void Mosquito::window_to_screen(glm::uvec2 const& window_size, glm::vec2& pos) {
	pos.x -= window_size.x / 2.0f;
	pos.y += window_size.y / 2.0f;

	pos /= static_cast<float>(scale);

	pos += glm::vec2(ppu.ScreenWidth / 2.0f, ppu.ScreenHeight / 2.0f);
	pos.x = glm::clamp(pos.x, 0.0f, static_cast<float>(ppu.ScreenWidth));
	pos.y = glm::clamp(pos.y, 0.0f, static_cast<float>(ppu.ScreenHeight));

	return;
}

bool Mosquito::handle_event(SDL_Event const& evt, glm::uvec2 const& window_size) {

	if (evt.type == SDL_MOUSEBUTTONDOWN) {
		// for hitting
		auto keyEvent = evt.button.button;
		if (keyEvent == SDL_BUTTON_LEFT) {
			mouse_click = true;
			mouse_pos = glm::vec2(static_cast<float>(evt.button.x), -static_cast<float>(evt.button.y));
			window_to_screen(window_size, mouse_pos);	

			//std::cout << "x: " << mouse_pos.x << "\n";
			//std::cout << "y: " << mouse_pos.y << "\n";
		}
	}
	
	if (evt.type == SDL_MOUSEMOTION) {
		// for moving
		mouse_pos = glm::vec2(static_cast<float>(evt.motion.x), -static_cast<float>(evt.motion.y));
		window_to_screen(window_size, mouse_pos);


		// std::cout << "x: " << mouse_pos.x << "\n";
		// std::cout << "y: " << mouse_pos.y << "\n";

	}

	return false;
}

void Mosquito::update(float elapsed) {
	blood_pic.Update_Pos(static_cast<glm::uvec2>(mouse_pos));
}

void Mosquito::draw(glm::uvec2 const& drawable_size) {
	// scale
	scale = std::max(1U, std::min(drawable_size.x / PPU466::ScreenWidth, drawable_size.y / PPU466::ScreenHeight));

	uint32_t swatter_tile_pos = 16;
	uint32_t mosquito_tile_pos = 32;
	uint32_t blood_tile_pos = 48;
	for (uint32_t i = 0; i < 16; i++) {
		ppu.sprites[i] = heart_pic.png_sprites[i];
		ppu.sprites[i + swatter_tile_pos] = flyswatter_pic.png_sprites[i];
		ppu.sprites[i + mosquito_tile_pos] = mosquito_pic.png_sprites[i];
		ppu.sprites[i + blood_tile_pos] = blood_pic.png_sprites[i];
		//std::cout << (int)ppu.sprites[i].x << " " << (int)ppu.sprites[i].y << std::endl;
	}

	

	//--- actually draw ---
	ppu.draw(drawable_size);
}
