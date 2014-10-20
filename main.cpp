//#include "res_texture.c"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* GLM */
// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
#include <pcap.h>
#include "NetworkEntities.hpp"
#include "MovingCube.hpp"
#include "sniffer.hpp"
#include "gframework/graphicsframework.hpp"

int screen_width=1000, screen_height=1000;

//input variables
char pressed[256];
char pressed_last='\0';
char ontrail=0;
char rainbows=0;

//camera variables
glm::vec3 cam_pos = glm::vec3(92.436607, 71.134125, 284.793793);
glm::vec3 cam_dir = glm::vec3(-0.362774, -0.183274, -0.913677);
glm::vec3 cam_up = glm::vec3(-0.083811, 0.965207, -0.160333);
glm::vec3 cam_side = glm::vec3(-0.927788, -0.020712, 0.372532);

//time
int time_passed=0;
int time_abs=0;


void updateCam()
{
  //position
  float factor_pos=0.05*time_passed;
  if(pressed['w'])
    cam_pos+=glm::vec3(factor_pos)*cam_dir;
  if(pressed['s'])
    cam_pos-=glm::vec3(factor_pos)*cam_dir;
  if(pressed['a'])
    cam_pos-=glm::vec3(factor_pos)*glm::cross(cam_dir,cam_up);
  if(pressed['d'])
    cam_pos+=glm::vec3(factor_pos)*glm::cross(cam_dir,cam_up);

  //direction
  float factor_dir=0.002*time_passed;
  if(pressed['8'])
  {
    cam_dir=glm::normalize(  cam_dir+glm::vec3(factor_dir)*cam_up  );
    cam_up=glm::cross(  glm::cross(cam_dir,cam_up) , cam_dir) ;
  }
  if(pressed['2'])
  {
    cam_dir=glm::normalize(  cam_dir-glm::vec3(factor_dir)*cam_up  );
    cam_up=glm::cross(  glm::cross(cam_dir,cam_up) , cam_dir) ;
  }
  if(pressed['4'])
  {
    cam_dir=glm::normalize(  cam_dir+glm::vec3(factor_dir)*cam_side  );
    cam_side=glm::cross(  glm::cross(cam_dir,cam_side) , cam_dir) ;
  }
  if(pressed['6'])
  {
    cam_dir=glm::normalize(  cam_dir-glm::vec3(factor_dir)*cam_side  );
    cam_side=glm::cross(  glm::cross(cam_dir,cam_side) , cam_dir) ;
  }

  //up
  if(pressed['q'])
  {
    cam_up=glm::normalize(  cam_up+glm::vec3(factor_dir)*cam_side  );
    cam_side=glm::cross(  cam_up , cam_dir) ;
  }
  if(pressed['e'])
  {
    cam_up=glm::normalize(  cam_up-glm::vec3(factor_dir)*cam_side  );
    cam_side=glm::cross(  cam_up , cam_dir) ;
  }
}

int init_resources()
{
  simpleVS= new Shader(NETWORK_VISUALIZER_RESSOURCE_PATH "shader/simple.vs.c",GL_VERTEX_SHADER);
  simpleFS= new Shader(NETWORK_VISUALIZER_RESSOURCE_PATH "shader/simple.fs.c",GL_FRAGMENT_SHADER);
  simpleProgram = new SProgram(*simpleVS,*simpleFS);
  uniform_proj=uniform_init("proj",*simpleProgram);
  uniform_pos=uniform_init("obj_pos",*simpleProgram);
  uniform_col=uniform_init("color",*simpleProgram);
  uniform_scale=uniform_init("scale",*simpleProgram);

  print_log(simpleProgram->getProgram());

  initNetworkEntity();
  init_MovingCubes();
  sniffer_start();
  return 1;
}

void onIdle()
{
  //meassure time
  time_passed=glutGet(GLUT_ELAPSED_TIME)-time_abs;
  time_abs=glutGet(GLUT_ELAPSED_TIME);

  //update camera
  updateCam();
  glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_dir, cam_up);
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 100000.0f);
  glm::mat4 mvp = projection * view;
  simpleProgram->use();
  glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(mvp));

  //redisplay
  glutPostRedisplay();
}

int frame_counter=0;

void onDisplay()
{
  //fps counter
  if(time_abs%5000 < (time_abs-time_passed) %5000)
  {
    printf("%f fps,   %i cubes\n",frame_counter/5.0,movingCubes.size());
    frame_counter=0;
  }
  frame_counter++;

  //background color
  float r,g,b;
  r=g=b=0.1;
  if(rainbows!=0)
  {
    r=0.5*sin(2*M_PI*(time_abs/5000.0))+0.5;
    g=0.5*sin(2*M_PI*(time_abs/5000.0+0.333))+0.5;
    b=0.5*sin(2*M_PI*(time_abs/5000.0+0.666))+0.5;
  }
  glClearColor(r, g, b, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  //draw Packages
  drawNetworkPackages(time_passed);
  //draw Network Entities
  drawNetworkEntities();

  glutSwapBuffers();
}

void onReshape(int width, int height)
{
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}

//konami code to toogle rainbow-mode
int trail()
{
  char* trail_chars=(char*) "wwssdadaba";
  if(pressed_last==trail_chars[ontrail])
    ontrail++;
  else
    ontrail=0;
  if(ontrail==strlen(trail_chars))
  {
    ontrail=0;
    rainbows=(rainbows==0);
  }
}

void pressKey(unsigned char key, int x, int y)
{
  int mod = glutGetModifiers();
  //only lowercase keys
  if(key<='Z' && key>='A')
    key=key-'A'+'a';
  //set pressed key
  pressed[key]=1;
  pressed_last=key;
  //quit on Ctrl+C
  if((key==3)&&(mod&2))
    exit(0);
  if(key==' ')
    g_stop_updating^=1;
  //print info on i
  if(key=='i')
  {
    printf("pos: %f, %f, %f\n",cam_pos.x,cam_pos.y,cam_pos.z);
    printf("dir: %f, %f, %f\n",cam_dir.x,cam_dir.y,cam_dir.z);
    printf("up: %f, %f, %f\n",cam_up.x,cam_up.y,cam_up.z);
    printf("side: %f, %f, %f\n",cam_side.x,cam_side.y,cam_side.z);
  }
  trail();
}

void releaseKey(unsigned char key, int x, int y)
{
  if(key<='Z' && key>='A')
    key=key-'A'+'a';
  pressed[key]=0;
}

MovingCube* activeCube;

void onMouse(int button, int state, int x, int y)
{

  if(state != GLUT_DOWN)
    return;
  int window_width = glutGet(GLUT_WINDOW_WIDTH);
  int window_height = glutGet(GLUT_WINDOW_HEIGHT);
  GLbyte color[4];
  GLfloat depth;
  GLuint index;
  glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
  glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
  glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
  glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_dir, cam_up);
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 100000.0f);
  glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
  glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
  glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);

  if(button==0) //left mouse button - halt packages
  {
    //get new cube
    activeCube=getNearestCube(objcoord);
    if(activeCube!=NULL)
      activeCube->toogleHalt();
  }
  else if(button==2) //right button, show contents
  {
    activeCube=getNearestCube(objcoord);
    if(activeCube!=NULL)
      activeCube->printContent();
  }
}

int main(int argc, char* argv[])
{
  srand(1337);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DEPTH|GLUT_DOUBLE);
  glutInitWindowSize(screen_width, screen_height);
  glutCreateWindow("Network Visualizer");
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return 1;
  }
  if (!GLEW_VERSION_2_0)
  {
    fprintf(stderr, "Error: Graphics Card does not seem to support OpenGL 2.0\n");
    return 1;
  }
  if (init_resources())
  {
    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);
    glutIgnoreKeyRepeat(1);
    glutMouseFunc(onMouse);
    glutKeyboardFunc(pressKey);
    glutKeyboardUpFunc(releaseKey);
    glutIdleFunc(onIdle);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
  }
  return 0;
}
