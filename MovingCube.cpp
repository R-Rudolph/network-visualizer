#include "MovingCube.hpp"

std::mutex templist_mutex;
std::vector<MovingCube*> movingCubes;
std::vector<MovingCube*> movingCubes_temp;
Attribute* cubes_M_att_coord;
Attribute* cubes_M_att_texcoord;
Attribute* cubes_M_att_texture;
glElement* cubes_M_elems;
std::mutex listmutex;
int update_lasttime=0;

void MovingCube::toogleHalt()
{
  move=move==0;
}

glObject* MovingCube::getObject()
{
  return graphic;
}

void MovingCube::printContent()
{
  printIPPackage(content,contentlength);
}

void MovingCube::draw()
{
  graphic->draw();
}

glm::vec3 MovingCube::destination()
{
  return destination_netent->position;
}

MovingCube::MovingCube(int dir,Attribute* coord, Attribute* texcoord, Attribute* texture, GLint unif_position, GLint color, GLint scale, glElement* elements, glm::vec3 color_vec, glm::vec3 scale_vector,const u_char* content, bpf_u_int32 contentlength)
{
  end=0;
  move=1;
  factor=1;
  this->contentlength=contentlength;
  this->content=(u_char*) malloc(contentlength);
  memcpy((void*)this->content,(const void*)content,contentlength);
  if(dir)
    factor=-1;
  att_position= new Attribute_unif_vec3(unif_position,glm::value_ptr(position));
  col_vector=color_vec;
  sca_vector=scale_vector;
  att_color= new Attribute_unif_vec3(color,glm::value_ptr(col_vector));
  att_scale= new Attribute_unif_vec3(scale,glm::value_ptr(sca_vector));

  graphic = new glObject();
  graphic->addElem(elements);

  graphic->att_coord=coord;
  graphic->att_texcoord=texcoord;
  graphic->att_position=att_position;
  graphic->att_color=att_color;
  graphic->att_scale=att_scale;
  const struct ip_header* hdr=(const struct ip_header*) (content+14);
  NetworkEntity* src = addNetworkEntity(hdr->ip_src);
  destination_netent = getNetworkEntity(hdr->ip_dst);
  position=src->position + glm::vec3( (rand()%2000-1000)/100.0 , (rand()%2000-1000)/100.0 , (rand()%2000-1000)/100.0);
  distance=glm::dot(destination()-position,destination()-position);
}

MovingCube::~MovingCube()
{
  delete graphic;
  delete att_position;
  delete att_color;
  delete att_scale;
  free(content);
}

int MovingCube::activate(int t_passed)
{
  draw();
  return 0;
}


int MovingCube::update(int t_passed)
{
  if(move)
  {
    int num=movingCubes.size();
    if(num>3000)
    {
      num=3000;
    }
    glm::vec3 direction=glm::normalize(destination()-position);
    float distancefactor=t_passed*0.00002*num;
    position = position+glm::vec3(distancefactor)*direction;
    distance=glm::dot(destination()-position,destination()-position);
    if(distance<5.0)
      end=1;
  }
  return 0;
}

//end of moving cube implementation

void addNewNetworkPackage(int direction, glm::vec3 color, glm::vec3 scale_vec, const u_char* content, bpf_u_int32 contentlength)
{
  MovingCube* newcube = new MovingCube(direction,cubes_M_att_coord,cubes_M_att_texcoord,cubes_M_att_texture,uniform_pos, uniform_col, uniform_scale ,cubes_M_elems, color, scale_vec, content, contentlength);
  std::vector<MovingCube*>::iterator it;

  templist_mutex.lock();
  it=movingCubes_temp.end();
  movingCubes_temp.insert(it,newcube);
  templist_mutex.unlock();
}

void drawNetworkPackages(int t_passed)
{
  cubes_M_att_texture->enable();
  listmutex.lock();
  for(int i=0;i<movingCubes.size();i++)
  {
    movingCubes[i]->draw();
  }
  listmutex.unlock();
  cubes_M_att_texture->disable();
}

void* updateNetworkPackages(void* arg)
{
  while(1)
  {
    int update_passedtime=glutGet(GLUT_ELAPSED_TIME)-update_lasttime;
    update_lasttime=glutGet(GLUT_ELAPSED_TIME);
    std::vector<MovingCube*>::iterator it;


    //delete packages marked for deletion
    listmutex.lock();
    it=movingCubes.begin();
    for(int i=movingCubes.size()*0.75;i<movingCubes.size();i++)
    {
      if(movingCubes[i]->end)
      {
        delete movingCubes[i];
        movingCubes.erase(it+i);
        i--;
      }
    }
    listmutex.unlock();

    templist_mutex.lock();
    it=movingCubes.begin();
    //add new
    std::vector<MovingCube*>::iterator it_begin=movingCubes_temp.begin();
    std::vector<MovingCube*>::iterator it_end=movingCubes_temp.end();
    it=movingCubes.begin();
    movingCubes.insert(it,it_begin,it_end);
    movingCubes_temp.clear();
    templist_mutex.unlock();

    //update packages
    for(int i=0;i<movingCubes.size();i++)
    {
      movingCubes[i]->update(update_passedtime);
    }
  }
  return NULL;
}

MovingCube* getNearestCube(glm::vec3 coords)
{
  MovingCube* result;
  for(int i=0;i<movingCubes.size();i++)
  {
    result=movingCubes[i];
    glm::vec3 pos_vec(result->position[0],result->position[1],result->position[2]);
    if(glm::dot(coords - pos_vec,coords - pos_vec)<glm::dot(result->sca_vector,result->sca_vector))
      return result;
  }
  return NULL;
}

void init_MovingCubes()
{
  GLuint elems[]={  
    // front
    0,  1,  2,
    2,  3,  0,
    // top

    4,  5,  6,
    6,  7,  4,
    // back
    8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
    };

  GLfloat coords[]={
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // top
    -1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
    // back
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
    // bottom
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,
    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    // right
     1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0,  1.0,
    };
  GLfloat texcoords[]={  
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,

    0.0, 1.0,

    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,

    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,

    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,

    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,

    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    };

  cubes_M_att_coord=new Attribute_vbo(*new VertexBufferObject(coords,3,sizeof(coords)),"coord3d",*simpleProgram);
  cubes_M_att_texcoord=new Attribute_vbo(*new VertexBufferObject(texcoords,2,sizeof(texcoords)),"tex_coord",*simpleProgram);
  cubes_M_att_texture=new Attribute_texture(*new Texture(NETWORK_VISUALIZER_RESSOURCE_PATH "textures/ip.png"), GL_TEXTURE0, "texture", *simpleProgram);
  cubes_M_elems = new glElement(elems,sizeof(elems));
  cubes_M_att_coord->enable();
  cubes_M_att_texcoord->enable();
  cubes_M_att_texture->enable();

  pthread_t thread;
  pthread_create(&thread,NULL,updateNetworkPackages,NULL);
  pthread_detach(thread);
}

