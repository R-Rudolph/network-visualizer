#include "Element.hpp"

glElement::glElement(GLuint elem_array[], int array_size)
{
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, array_size, elem_array, GL_STATIC_DRAW);
}

glElement::~glElement()
{
}

glElement::glElement()
{
}

glElement::glElement(GLuint vbo)
{
  this->vbo=vbo;
}

void glElement::draw()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  int size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

GLuint glElement::getVBO()
{
  return vbo;
}

