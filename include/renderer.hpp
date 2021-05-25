// renderer.hpp

#ifndef _RENDERER_HPP
#define _RENDERER_HPP

#include "resource.hpp"
#include "matrix_math.hpp"

typedef struct Model {
  u32 draw_count;
  u32 vao;
  u32 vbo;
  u32 ebo;
} Model;

typedef struct Material {
  float emission;
  float shininess;
  float specular_amp;
  u32 texture_id;
} Material;

extern mat4 projection;
extern mat4 view;
extern mat4 model;

typedef struct Render_state {
	u32 textures[MAX_TEXTURE];
	u32 texture_count;

	u32 cube_maps[MAX_CUBE_MAP];
	u32 cube_map_count;

	Model models[MAX_MESH];
	u32 model_count;

	Resources resources;
	i32 depth_func;
} Render_state;

i32 renderer_initialize();

void render_mesh(v3 position, v3 rotation, v3 size, u32 mesh_id, Material material);

void render_skybox(u32 skybox_id, float brightness);

void renderer_destroy();

#endif
