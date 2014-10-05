#ifndef __GFRAMEWORK_SHADER_HPP_INCLUDED__
#define __GFRAMEWORK_SHADER_HPP_INCLUDED__


#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>


void print_log(GLuint object);

class Shader
{
    GLuint shader;
    GLuint type;
    ushort valid_var;
  public:
    GLuint getShader();
    GLuint getType();
    ushort valid();
    ~Shader();
    Shader(const char* filename, GLuint type);
    Shader(){}
};

class SProgram
{
    GLuint program;
    ushort valid_var;
  public:
    SProgram(Shader vshader, Shader fshader);
    SProgram(){}
    ~SProgram(){}
    void use(){glUseProgram(program);}
    GLuint getProgram(){return program;}
    ushort valid(){return valid_var;}
};

#endif

