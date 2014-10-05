#ifndef __GFRAMEWORK_TEXTURE_HPP_INCLUDED__
#define __GFRAMEWORK_TEXTURE_HPP_INCLUDED__

#include <string>
#include <SOIL/SOIL.h>
#include "Attribute.hpp"
#include "Shader.hpp"

class Texture
{
    GLuint texture_id;
    void init(unsigned int width, unsigned int height, const unsigned char* data);
    int load_file(char* filename, char** output);
  public:
    GLuint getID(){return texture_id;}
    Texture(unsigned int width, unsigned int height, const unsigned char* data);
    Texture(unsigned int width, unsigned int height, const unsigned char* data, std::string filename);
    Texture(std::string filename);
    Texture();
    ~Texture();
};

class Attribute_texture: public Attribute
{
    Texture texture;
    GLuint texnum;
    GLint uniform;
  public:
    Attribute_texture(Texture texture, GLuint texnum, const char* varname, SProgram prog);
    Attribute_texture();
    ~Attribute_texture();
    void enable();
    void disable();
};

#endif
