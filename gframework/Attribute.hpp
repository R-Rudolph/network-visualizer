#ifndef __GFRAMEWORK_ATTRIBUTE_HPP_INCLUDED__
#define __GFRAMEWORK_ATTRIBUTE_HPP_INCLUDED__

#include "Shader.hpp"
#include "VertexBufferObject.hpp"
class Attribute
{
  public:
    Attribute();
    virtual void enable(){};
    virtual void disable(){};
};

class Attribute_vbo : public Attribute
{
    VertexBufferObject vbo;
    GLint namelink;
    const char* attrib_name;
    ushort valid_var;
  public:
    Attribute_vbo(GLfloat* data, int dim, int size,const char* name,SProgram program);
    Attribute_vbo(VertexBufferObject vbo, const char* name,SProgram program);
    Attribute_vbo();
    ~Attribute_vbo();
    void enable();
    void disable();
};

#endif
