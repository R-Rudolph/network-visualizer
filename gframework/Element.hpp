#ifndef __GFRAMEWORK_ELEMENT_HPP_INCLUDED__
#define __GFRAMEWORK_ELEMENT_HPP_INCLUDED__

class glElement
{
    GLuint vbo;
  public:
    glElement(GLuint elem_array[], int array_size);
    glElement(GLuint vbo);
    glElement();
    ~glElement();
    void draw();
    GLuint getVBO();
};

#endif
