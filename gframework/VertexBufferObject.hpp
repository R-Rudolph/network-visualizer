#ifndef __GFRAMEWORK_VBO_HPP_INCLUDED__
#define __GFRAMEWORK_VBO_HPP_INCLUDED__

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <GL/glew.h>
#include <GL/glut.h>

//VertexBufferObject class
class VertexBufferObject
{
    GLuint vbo;
    const char* attrib_name;
    int size;
    int dim;
    void init(GLfloat* data, int dim, int size);
  public:
    int load_file(char* filename, char** output);
    VertexBufferObject();
    VertexBufferObject(GLfloat* data, int dim, int size);
    VertexBufferObject(GLfloat* data, int dim, int size, char* dest);
    VertexBufferObject(char* filename);
    ~VertexBufferObject();
    int getSize();
    virtual int getNumElements();
    int getDim();
    GLuint getVBO();
};

//VertexBufferObject_uint class
class VertexBufferObject_uint : public VertexBufferObject
{
    GLuint vbo;
    const char* attrib_name;
    int size;
    int dim;
    void init(GLuint* data, int dim, int size);
  public:
    VertexBufferObject_uint(){}
    VertexBufferObject_uint(GLuint* data, int dim, int size);
    VertexBufferObject_uint(GLuint* data, int dim, int size, char* dest);
    VertexBufferObject_uint(char* filename);
    ~VertexBufferObject_uint();
    int getSize();
    virtual int getNumElements();
    int getDim();
    GLuint getVBO();
};

#endif

