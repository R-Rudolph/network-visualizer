#include "Attribute.hpp"

Attribute::Attribute()
{
}
/*
virtual void Attribute::enable()
{
  printf("enable called\n");
}

virtual void Attribute::disable()
{
}
*/
Attribute_vbo::Attribute_vbo()
{
}

void Attribute_vbo::disable()
{
  glDisableVertexAttribArray(namelink);
}

Attribute_vbo::Attribute_vbo(GLfloat* data, int dim, int size,const char* name,SProgram program)
{
  vbo=*new VertexBufferObject(data,dim,size);
  attrib_name = name;
  GLuint prog=program.getProgram();
  namelink = glGetAttribLocation(prog, name);
  if (namelink == -1)
  {
    fprintf(stderr, "Could not bind attribute %s.\n", name);
    valid_var=0;
  }
}

Attribute_vbo::Attribute_vbo(VertexBufferObject vbo,const char* name,SProgram program)
{
  this->vbo=vbo;
  attrib_name = name;
  GLuint prog=program.getProgram();
  namelink = glGetAttribLocation(prog, name);
  if (namelink == -1)
  {
    fprintf(stderr, "Could not bind attribute %s.\n", name);
    valid_var=0;
  }
}

Attribute_vbo::~Attribute_vbo()
{
  //printf("destr %s\n",attrib_name);
  //glDeleteBuffers(1, vbo);
}

void Attribute_vbo::enable()
{
  glEnableVertexAttribArray(namelink);
  glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO());
  glVertexAttribPointer(
    namelink,          // attribute
    vbo.getDim(),                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );
}
	
