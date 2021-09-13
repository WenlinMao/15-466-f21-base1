#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

#include "PNGLoader.hpp"

struct Mosquito : Mode {
	Mosquito();
	virtual ~Mosquito();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const&, glm::uvec2 const& window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const& drawable_size) override;

	//help functions
	void load_resource();
	void window_to_screen(glm::uvec2 const& window_size, glm::vec2& pos);

	//----- game state -----
	PNGSprite heart_pic;
	PNGSprite background_pic;

	//input tracking:
	uint32_t scale = 2;
	bool mouse_click = false;	
	glm::vec2 mouse_pos = glm::vec2(0.0f);

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
