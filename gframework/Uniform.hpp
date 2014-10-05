#ifndef __GFRAMEWORK_UNIFORM_HPP_INCLUDED__
#define __GFRAMEWORK_UNIFORM_HPP_INCLUDED__

#include "Shader.hpp"
#include "Attribute.hpp"

GLint uniform_init(const char* name, SProgram prog);

//uniform class
class Uniform
{
    GLint id;
  public:
    Uniform(const char* uniform_name, SProgram shader_program);
    GLint getID();
};

//4x4 matrix attribute class
class Attribute_unif_mat : public Attribute
{
    GLfloat* value_ptr;
    GLint uniform;
  public:
    Attribute_unif_mat();
    Attribute_unif_mat(GLint uniform,GLfloat* value_ptr);
    void enable();
    void disable();
};

//3-vector attribute class
class Attribute_unif_vec3 : public Attribute
{
    GLfloat* value_ptr;
    GLint uniform;
  public:
    Attribute_unif_vec3();
    Attribute_unif_vec3(GLint uniform,GLfloat* value_ptr);
    void enable();
    void disable();
};

#endif

