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

	PNGSprite mosquito_pic_0 = PNGSprite(1, 32, mosquito_pos_0);
	PNGLoader::load("../resource/mos.png", mosquito_pic_0);
	mosquito_pic_0.Initialize_PNG(ppu, 0);

	PNGSprite blood_pic_0 = PNGSprite(2, 48, mosquito_pos_0);
	PNGLoader::load("../resource/blood32.png", blood_pic_0);
	blood_pic_0.Initialize_PNG(ppu, 0);

	PNGSprite mosquito_pic_1 = mosquito_pic_0;
	mosquito_pic_1.Update_Pos(mosquito_pos_1);
	PNGSprite blood_pic_1 = blood_pic_0;
	blood_pic_1.Update_Pos(mosquito_pos_1);

	mosquitos[0].spawn_pos = mosquito_pos_0;
	mosquitos[0].mosquito_pic = mosquito_pic_0;
	mosquitos[0].blood_pic = blood_pic_0;

	mosquitos[1].spawn_pos = mosquito_pos_1;
	mosquitos[1].mosquito_pic = mosquito_pic_1;
	mosquitos[1].blood_pic = blood_pic_1;

	flyswatter_pic = PNGSprite(3, 64, glm::uvec2(124, 122));
	PNGLoader::load("../resource/flyswatter32.png", flyswatter_pic);
	flyswatter_pic.Initialize_PNG(ppu, 0);

	for (auto& mos : mosquitos)
		spawn_mosquito(mos);
}

void Mosquito::window_to_screen(glm::uvec2 const& window_size, glm::vec2& pos) {
	pos.x -= window_size.x / 2.0f;
	pos.y += window_size.y / 2.0f;

	pos /= static_cast<float>(scale);

	pos += glm::vec2(ppu.ScreenWidth / 2.0f, ppu.ScreenHeight / 2.0f);
	pos.x = glm::clamp(pos.x, 16.0f, static_cast<float>(ppu.ScreenWidth) - 16.0f);
	pos.y = glm::clamp(pos.y, 16.0f, static_cast<float>(ppu.ScreenHeight) - 16.0f);

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

			std::cout << "x: " << mouse_pos.x << "\n";
			std::cout << "y: " << mouse_pos.y << "\n";

			for (auto& mos : mosquitos) {
				if (glm::length(mouse_pos - mos.spawn_pos) < 15.0f) {
					kill_mosquito(mos);
					score++;
				}
			}
		}
	}

	if (evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT) {
		mouse_click = false;
	}
	
	if (evt.type == SDL_MOUSEMOTION) {
		// for moving
		mouse_pos = glm::vec2(static_cast<float>(evt.motion.x), -static_cast<float>(evt.motion.y));
		window_to_screen(window_size, mouse_pos);
	}

	return false;
}

void Mosquito::spawn_mosquito(MosquitoObject& mosquito) {
	float x = static_cast<float>(rand() % 256);
	float y = static_cast<float>(rand() % 240);

	//std::cout << "spawn x: " << x << "\n";
	//std::cout << "spawn y: " << y << "\n";

	mosquito.spawn_pos = glm::vec2(x, y);
	mosquito.mosquito_pic.Update_Pos(mosquito.spawn_pos);

	mosquito.since_death = 0.0f;
	mosquito.since_spawn = 0.0f;
	mosquito.show_mosquito = true;
	mosquito.show_blood = false;
}

void Mosquito::kill_mosquito(MosquitoObject& mosquito) {
	mosquito.blood_pic.Update_Pos(mosquito.mosquito_pic.pos);

	mosquito.since_death = 0.0f;
	mosquito.since_spawn = 0.0f;
	mosquito.show_mosquito = false;
	mosquito.show_blood = true;
}

void Mosquito::update(float elapsed) {
	flyswatter_pic.Update_Pos(static_cast<glm::uvec2>(mouse_pos));

	for (auto& mos : mosquitos) {
		//if (mouse_click && glm::length(mouse_pos - mos.spawn_pos) < 5.0f) {
		//	kill_mosquito(mos);
		//	score++;
		//	continue;
		//}

		if (mos.show_mosquito) {
			mos.since_spawn += elapsed;

			if (mos.since_spawn > mosquito_life_span) {
				// Just for testing, should be deducting life
				// kill_mosquito(mos);

				score--;
				life--;
				spawn_mosquito(mos);
			}
		}
		else if (mos.show_blood) {
			mos.since_death += elapsed;

			if (mos.since_death > mosquito_respawn_time) {
				spawn_mosquito(mos);
			}
		}
	}
}

void Mosquito::draw(glm::uvec2 const& drawable_size) {
	// scale
	scale = std::max(1U, std::min(drawable_size.x / PPU466::ScreenWidth, drawable_size.y / PPU466::ScreenHeight));

	uint32_t mosquito_0_tile_pos = 16;
	uint32_t mosquito_1_tile_pos = 32;
	uint32_t swatter_tile_pos = 48;
	for (uint32_t i = 0; i < 16; i++) {	
		ppu.sprites[i] = heart_pic.png_sprites[i];

		if (mosquitos[0].show_mosquito) {
			ppu.sprites[i + mosquito_0_tile_pos] = mosquitos[0].mosquito_pic.png_sprites[i];
		}
		else if (mosquitos[0].show_blood) {
			ppu.sprites[i + mosquito_0_tile_pos] = mosquitos[0].blood_pic.png_sprites[i];
		}
		else {
			ppu.sprites[i + mosquito_0_tile_pos].y = 240;
		}

		if (mosquitos[1].show_mosquito) {
			ppu.sprites[i + mosquito_1_tile_pos] = mosquitos[1].mosquito_pic.png_sprites[i];
		}
		else if (mosquitos[1].show_blood) {
			ppu.sprites[i + mosquito_1_tile_pos] = mosquitos[1].blood_pic.png_sprites[i];
		}
		else {
			ppu.sprites[i + mosquito_1_tile_pos].y = 240;
		}

		ppu.sprites[i + swatter_tile_pos] = flyswatter_pic.png_sprites[i];
		//std::cout << (int)ppu.sprites[i].x << " " << (int)ppu.sprites[i].y << std::endl;
	}

	

	//--- actually draw ---
	ppu.draw(drawable_size);
}