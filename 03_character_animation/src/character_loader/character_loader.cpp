#include "character_loader.hpp"
#include "../environment.hpp"

using namespace cgp;

character_structure load_character_lola() {
	filename_loader_structure loader_param;
	loader_param.set_skeleton(project::path+"assets/lola/skeleton/");
	loader_param.add_animation("Dance1", project::path+"assets/lola/animation/dance_1/");
	loader_param.add_animation("Dance2", project::path+"assets/lola/animation/dance_2/");
	loader_param.add_animation("Dance3", project::path+"assets/lola/animation/dance_3/");
	loader_param.add_animation("Dance4", project::path+"assets/lola/animation/dance_4/");
	loader_param.add_animation("Idle", project::path+"assets/lola/animation/idle/");
	loader_param.add_animation("Walk", project::path+"assets/lola/animation/walk/");
	loader_param.add_animation("Walk2", project::path+"assets/lola/animation/walk_style/");

	character_structure character;
	character.load_and_initialize(loader_param, affine_rts().set_scaling(0.01f));

	return character;
}

character_structure load_character_maria_sword() {
	filename_loader_structure loader_param;
	loader_param.set_skeleton(project::path+"assets/maria-sword/");
	loader_param.add_animation("Idle", project::path+"assets/maria-sword/animation/idle/");
	loader_param.add_animation("Idle2", project::path+"assets/maria-sword/animation/idle2/");
	loader_param.add_animation("Walk", project::path+"assets/maria-sword/animation/walking/");
	loader_param.add_animation("Slash", project::path+"assets/maria-sword/animation/slash/");

	character_structure character;
	character.load_and_initialize(loader_param, affine_rts().set_scaling(0.01f));

	return character;
}