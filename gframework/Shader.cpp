#include "Shader.hpp"

void print_log(GLuint object)
{
  GLint log_length = 0;
  if (glIsShader(object))
  {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  }
  else if (glIsProgram(object))
  {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  }
  else 
  {
    fprintf(stderr, "printlog: Not a shader or a program\n");
    return;
  }
  char* log = (char*)malloc(log_length);
  if (glIsShader(object))
  glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
  glGetProgramInfoLog(object, log_length, NULL, log);
  fprintf(stderr, "%s", log);
  free(log);
}

GLuint Shader::getShader()
{
  return shader;
}

GLuint Shader::getType()
{
  return type;
}

ushort Shader::valid()
{
  return valid_var;
}

Shader::Shader(const char* filename, GLuint type) //type GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
{
  valid_var=0;
  FILE* input = fopen(filename, "rb");
  if(input == NULL) return;
  if(fseek(input, 0, SEEK_END) == -1) return;
  long size = ftell(input);
  if(size == -1) return;
  if(fseek(input, 0, SEEK_SET) == -1) return;
  /*if using c-compiler: dont cast malloc's return value*/
  GLchar *content = (GLchar*) malloc( (size_t) size +1  ); 
  if(content == NULL) return;
  fread(content, 1, (size_t)size, input);
  if(ferror(input))
  {
    free(content);
    return;
  }
  fclose(input);
  content[size] = '\0';
  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar**)&content, NULL);
  free(content);
  glCompileShader(shader);
  GLint compile_ok;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in fragment shader in %s:\n",filename);
    print_log(shader);
    return;
  }
  valid_var=1;
}

Shader::~Shader()
{
  //glDeleteShader(shader);
}

SProgram::SProgram(Shader vshader, Shader fshader)
{
  GLint link_ok = GL_FALSE;
  program = glCreateProgram();
  print_log(vshader.getShader());
  print_log(fshader.getShader());
  glAttachShader(program, vshader.getShader());
  glAttachShader(program, fshader.getShader());
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  print_log(program);
  valid_var=link_ok;
}

