#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <SDL.h>

#include "PNGLoader.hpp"

struct Mosquito : Mode {
	Mosquito();
	virtual ~Mosquito();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const&, glm::uvec2 const& window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const& drawable_size) override;

	struct MosquitoObject {
		int spawn_idx = -1;
		glm::vec2 spawn_pos;
		PNGSprite mosquito_pic;
		PNGSprite blood_pic;
		bool show_mosquito = false;
		bool show_blood = false;
		float since_death = 0.0f;
		float since_spawn = 0.0f;
	};

	//help functions
	void load_resource();
	void window_to_screen(glm::uvec2 const& window_size, glm::vec2& pos);
	void spawn_mosquito(MosquitoObject& mosquito);
	void kill_mosquito(MosquitoObject& mosquito);
	void deduct_life();
	void change_game_pace(int dir);

	//----- game state -----
	uint16_t score = 0;
	uint16_t life = 4;

	float mosquito_life_span = 3.0f;
	float mosquito_respawn_time = 1.0f;
	PNGSprite heart_pic;

	PNGSprite background_pic;
	PNGSprite flyswatter_pic;

	std::array<MosquitoObject, 2> mosquitos;
	
	glm::vec2 mosquito_pos_0 = glm::vec2(64, 60);
	glm::vec2 mosquito_pos_1 = glm::vec2(124, 60);

	// sound
	SDL_AudioSpec wavSpec;
	uint32_t wavLength;
	uint8_t *wavBuffer;
	SDL_AudioDeviceID deviceId;

	//input tracking:
	uint32_t scale = 2;
	bool mouse_click = false;	
	glm::vec2 mouse_pos = glm::vec2(0.0f);

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
