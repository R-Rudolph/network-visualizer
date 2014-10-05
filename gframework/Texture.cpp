#include "Texture.hpp"

Texture::Texture()
{
}

Texture::~Texture()
{
}

Texture::Texture(unsigned int width, unsigned int height, const unsigned char* data)
{
  init(width,height,data);
}

int Texture::load_file(char* filename, char** output)
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

void Texture::init(unsigned int width, unsigned int height, const unsigned char* data)
{
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, // target
	       0,  // level, 0 = base, no minimap,
	       GL_RGB, // internalformat
	       width,  // width
	       height,  // height
	       0,  // border, always 0 in OpenGL ES
	       GL_RGB,  // format
	       GL_UNSIGNED_BYTE, // type
	       data);
}

Texture::Texture(unsigned int width, unsigned int height, const unsigned char* data, std::string filename)
{
  init(width,height,data);
  FILE *f = fopen((*((char**)(&filename))), "w");
  char* a=(char*)&width;
  char* b=(char*)&height;
  fprintf(f, "%c%c%c%c",a[0],a[1],a[2],a[3]);
  fprintf(f, "%c%c%c%c",b[0],b[1],b[2],b[3]);
  int i=0;
  while(i<(width*height)*3)
  {
    fprintf(f,"%c",data[i]);
    i++;
  }
  fclose(f);
}

Texture::Texture(std::string filename)
{
  int width, height;
  char* temp = (char*) &(filename[0]);
  unsigned char* image = SOIL_load_image(temp, &width, &height, 0, SOIL_LOAD_RGB);
  init(width,height,(const unsigned char*)image);
}

Attribute_texture::~Attribute_texture()
{
}

Attribute_texture::Attribute_texture()
{
}

Attribute_texture::Attribute_texture(Texture texture, GLuint texnum, const char* varname, SProgram prog)
{
  this->texture=texture;
  this->texnum=texnum;
  print_log(prog.getProgram());
  this->uniform = glGetUniformLocation(prog.getProgram(), varname);
  if (this->uniform == -1)
    fprintf(stderr, "Could not bind uniform %s\n", varname);
}

void Attribute_texture::enable()
{
  glActiveTexture(texnum);
  glBindTexture(GL_TEXTURE_2D, this->texture.getID());
  glUniform1i(this->uniform, GL_TEXTURE);
}

void Attribute_texture::disable()
{
}

