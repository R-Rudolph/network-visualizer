#include "Uniform.hpp"

GLint uniform_init(const char* name, SProgram prog)
{
  GLint uniform = glGetUniformLocation(prog.getProgram(), name);
  if (uniform == -1)
    fprintf(stderr, "Could not bind uniform %s\n", name);
  return uniform;
}

//functions of uniform class

GLint Uniform::getID()
{
  return this->id;
}

Uniform::Uniform(const char* uniform_name, SProgram shader_program)
{
  GLint uniform = glGetUniformLocation(shader_program.getProgram(), uniform_name);
  if (uniform == -1)
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
}

//functions of Attribute_unif_mat class

Attribute_unif_mat::Attribute_unif_mat()
{
}

void Attribute_unif_mat::disable()
{
}

Attribute_unif_mat::Attribute_unif_mat(GLint uniform,GLfloat* value_ptr)
{
  this->value_ptr=value_ptr;
  this->uniform=uniform;
}

void Attribute_unif_mat::enable()
{
  glUniformMatrix4fv(uniform, 1, GL_FALSE, value_ptr);
}

//functions of 3-vector attribute class

Attribute_unif_vec3::Attribute_unif_vec3()
{
}

void Attribute_unif_vec3::disable()
{
}

Attribute_unif_vec3::Attribute_unif_vec3(GLint uniform,GLfloat* value_ptr)
{
  this->value_ptr=value_ptr;
  this->uniform=uniform;
}

void Attribute_unif_vec3::enable()
{
  glUniform3f(uniform, value_ptr[0],value_ptr[1],value_ptr[2]);
}

