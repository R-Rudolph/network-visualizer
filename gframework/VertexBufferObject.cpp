#include "VertexBufferObject.hpp"

//VertexBufferObject implementations

VertexBufferObject::VertexBufferObject()
{
}

VertexBufferObject::VertexBufferObject(GLfloat* data, int dim, int size)
{
  init(data,dim,size);
}

int VertexBufferObject::load_file(char* filename, char** output)
{
  struct stat* st=(struct stat*) malloc(sizeof(struct stat));
  stat(filename, st);
  int size=st->st_size;
  free(st);
  char* filecontent=(char*) malloc(size);
  int f=open(filename,O_RDONLY);
  read(f, filecontent, size);
  close(f);
  *output=filecontent;
  return size;
}

void VertexBufferObject::init(GLfloat* data, int dim, int size)
{
  this->size=size;
  this->dim = dim;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBufferObject::VertexBufferObject(GLfloat* data, int dim, int size, char* dest)
{
  unsigned char* b=(unsigned char*)&dim;
  int i=size;
  unsigned char* a=(unsigned char*)data;
  FILE *f = fopen(dest, "w");
  fprintf(f, "%c%c%c%c",b[0],b[1],b[2],b[3]);
  int j=0;
  while(j<i)
  {
    fprintf(f,"%c",a[j]);
    j++;
  }
  fclose(f);
  init(data,dim,size);
}

VertexBufferObject::VertexBufferObject(char* filename)
{
  char* filecontent;
  int size=load_file(filename,&filecontent);
  //stuff
  struct {int dim; GLfloat data[];}* filestrct;
  reinterpret_cast<unsigned char*&>(filestrct) = reinterpret_cast<unsigned char*>(filecontent);
  size=size-4;
  init((GLfloat*)(filestrct->data),filestrct->dim,size);
  //free(filecontent);
}

VertexBufferObject::~VertexBufferObject()
{
  //glDeleteBuffers(1, &vbo);
}

int VertexBufferObject::getSize()
{
  return size;
}
int VertexBufferObject::getNumElements()
{
  return size/sizeof(GLfloat);
}
int VertexBufferObject::getDim()
{
  return dim;
}
GLuint VertexBufferObject::getVBO()
{
  return vbo;
}


//VertexBufferObject_uint implementations
VertexBufferObject_uint::VertexBufferObject_uint(GLuint* data, int dim, int size)
{
  init(data,dim,size);
}

int VertexBufferObject_uint::getNumElements()
{
  return size/sizeof(GLuint);
}

VertexBufferObject_uint::~VertexBufferObject_uint()
{
}

void VertexBufferObject_uint::init(GLuint* data, int dim, int size)
{
  this->size=size;
  this->dim = dim;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

VertexBufferObject_uint::VertexBufferObject_uint(GLuint* data, int dim, int size, char* dest)
{
  unsigned char* b=(unsigned char*)&dim;
  int i=size;
  unsigned char* a=(unsigned char*)data;
  FILE *f = fopen(dest, "w");
  fprintf(f, "%c%c%c%c",b[0],b[1],b[2],b[3]);
  int j=0;
  while(j<i)
  {
    fprintf(f,"%c",a[j]);
    j++;
  }
  fclose(f);
  init(data,dim,size);
  
}

VertexBufferObject_uint::VertexBufferObject_uint(char* filename)
{
  char* filecontent;
  int size=load_file(filename,&filecontent);
  //stuff
  struct {int dim; GLuint data[];}* filestrct;
  printf("size: %i\n",filestrct->dim);
  reinterpret_cast<unsigned char*&>(filestrct) = reinterpret_cast<unsigned char*>(filecontent);
  size=size-4;
  init((GLuint*)&(filestrct->data[0]),filestrct->dim,size);
  //free(filecontent);
}

