// engine.cpp

#include <sys/time.h>	// gettimeofday

#include "engine.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "renderer.hpp"

#define MAX_DT 1.0f

Engine engine;

static void engine_initialize(Engine* engine);
static i32 engine_run(Engine* engine);

void engine_initialize(Engine* engine) {
	engine->is_running = 1;
	engine->animation_playing = 1;
	engine->delta_time = 0;
	engine->total_time = 0;
	engine->time_scale = 1.0f;
	engine->mouse_x = 0;
	engine->mouse_y = 0;
	camera_initialize(V3(13, 3, 9));
}

i32 engine_run(Engine* engine) {
	float angle = 0.0f;
	struct timeval now = {0};
	struct timeval prev = {0};
	u8 follow_guy = 0;
	while (engine->is_running && window_poll_events() >= 0) {
		prev = now;
		gettimeofday(&now, NULL);
		engine->delta_time = ((((now.tv_sec - prev.tv_sec) * 1000000.0f) + now.tv_usec) - (prev.tv_usec)) / 1000000.0f;
		if (engine->delta_time >= MAX_DT) {
			engine->delta_time = MAX_DT;
		}
		if (engine->animation_playing) {
			engine->total_time += engine->delta_time * engine->time_scale;
		}
		if (key_pressed[GLFW_KEY_SPACE]) {
			engine->animation_playing = !engine->animation_playing;
		}
		if (key_pressed[GLFW_KEY_ESCAPE]) {
			engine->is_running = 0;
		}
		if (key_pressed[GLFW_KEY_F11]) {
			window_toggle_fullscreen();
		}
		if (key_pressed[GLFW_KEY_1]) {
			engine->time_scale -= 0.025f;
			fprintf(stdout, "Time scale: %g\n", engine->time_scale);
		}
		if (key_pressed[GLFW_KEY_2]) {
			engine->time_scale += 0.05f;
			fprintf(stdout, "Time scale: %g\n", engine->time_scale);
		}
		if (key_pressed[GLFW_KEY_3]) {
			engine->time_scale = 1;
			fprintf(stdout, "Reset time scale: %g\n", engine->time_scale);
		}
		if (key_pressed[GLFW_KEY_F]) {
			follow_guy = !follow_guy;
		}

		window_get_cursor(&engine->mouse_x, &engine->mouse_y);
		camera_update();

		v3 alien_pos = V3(35 * cos(engine->total_time * 0.85f), 2 * cos(engine->total_time * 0.85f), 35 * sin(engine->total_time * 0.85f));
		v3 alien_size = V3(1.5f, 1.5f, 1.5f);

		v3 earth_pos = V3(20 * cos(engine->total_time), 0, 20 * sin(engine->total_time));
		v3 earth_size = alien_size * 0.5f;

		v3 moon_pos = earth_pos + V3(4 * cos(engine->total_time), 0, 4 * sin(engine->total_time));
		v3 moon_size = earth_size * 0.5f;

		v3 guy_pos = moon_pos + V3(1 * cos(2.5f * engine->total_time), 0, 1 * sin(2.5f * engine->total_time));
		v3 guy_size = moon_size * 0.5f;

		if (follow_guy) {
			camera.pos = guy_pos - camera.forward * 1.5f;
		}

		render_mesh(alien_pos, V3(-25, angle * 1.25f, 0), alien_size, TEXTURE_ALIEN, MESH_SPHERE, 0.0f);
		render_mesh(earth_pos, V3(20, angle, 0), earth_size, TEXTURE_EARTH, MESH_SPHERE, 0.0f);
		render_mesh(moon_pos, V3(0, 0, 0), moon_size, TEXTURE_MOON, MESH_SPHERE, 0.0f);
		render_mesh(guy_pos, V3(angle * 2, angle * 1.2f, -angle), guy_size, TEXTURE_MONSTER, MESH_MONSTER, 0.0f);

		render_mesh(V3(0, 0, 0), V3(0, angle, 0), V3(3, 3, 3), TEXTURE_SUN, MESH_SPHERE, 1.0f);
#if 0
		render_mesh(earth_pos, V3(0, angle, 0), V3(0.4f, 0.4f, 0.4f), TEXTURE_EARTH, MESH_SPHERE, 0.0f);
		render_mesh(V3(-5, 1, -15), V3(0, -angle * 1.15f, 0), V3(1.25f, 1.25f, 1.25f), TEXTURE_ALIEN, MESH_SPHERE, 0.0f);
		render_mesh(V3(0, 0, 0), V3(0, angle, 0), V3(3, 3, 3), TEXTURE_SUN, MESH_SPHERE, 1.0f);
		render_mesh(V3(-16 + angle, 4, 7 - (angle / 2)), V3(angle * 4, angle, -angle), V3(0.1f, 0.1f, 0.1f), TEXTURE_MONSTER, MESH_MONSTER, 0.0f);
#endif
		angle += 2.5f * engine->delta_time;

		window_swap_buffers();
		window_clear_buffers(0, 0, 0);
	}
	return NoError;
}

i32 engine_start() {
	i32 result = NoError;
	engine_initialize(&engine);

	if ((result = window_open("Solar System", 800, 600, 0 /* fullscreen */, 0 /* vsync */)) == NoError) {
		renderer_initialize();
		engine_run(&engine);
		window_close();
		renderer_destroy();
	}
	assert("memory leak" && (memory_total_allocated() == 0));
	return result;
}
