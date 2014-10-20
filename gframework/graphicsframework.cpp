#include "graphicsframework.hpp"

#include "Attribute.cpp"
#include "Element.cpp"
#include "Object.cpp"
#include "Shader.cpp"
#include "Texture.cpp"
#include "Uniform.cpp"
#include "VertexBufferObject.cpp"


GLint uniform_proj, uniform_pos, uniform_col, uniform_scale;
Shader* simpleVS;
Shader* simpleFS;
SProgram* simpleProgram;
unsigned char g_stop_updating=0;

