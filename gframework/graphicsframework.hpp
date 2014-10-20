#ifndef __GRAPHICSFRAMEWORK_H_INCLUDED__
#define __GRAPHICSFRAMEWORK_H_INCLUDED__

#include "Attribute.hpp"
#include "Object.hpp"
#include "Uniform.hpp"
#include "Texture.hpp"

extern GLint uniform_proj, uniform_pos, uniform_col, uniform_scale;
extern Shader* simpleVS;
extern Shader* simpleFS;
extern SProgram* simpleProgram;
extern unsigned char g_stop_updating;

#endif

