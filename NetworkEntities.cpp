#include "NetworkEntities.hpp"

std::vector<NetworkEntity*> networkEntityList;
NetworkEntity* networkEntityExternal;
NetworkEntity* networkEntitySink;
unsigned int subnet;
unsigned int ownIP;

Attribute* netent_M_att_coord;
Attribute* netent_M_att_texcoord;
Attribute* netent_M_att_texture;
Attribute* netent_M_att_texture_sink;
Attribute* netent_M_att_texture_external;
glElement* netent_M_elems;

int isInSubnet(const unsigned int addr)
{
  int result=(subnet&ownIP)==(subnet&addr);
  return result;
}

int isMulticast(const unsigned int address)
{
  unsigned int ssdp=(239<<0) + (255<<8) + (255<<16) + (250<<24);
  if(address==ssdp)
    return 1;
  if((address&0x000000FF) == (224))
    return 1;
  if(address == 0xFFFFFFFF)
    return 1;
  return 0;
}

int defineNetwork(unsigned int ipaddr,unsigned int subnetmask)
{
  subnet=subnetmask;
  ownIP=ipaddr;
  return 0;
}

void initNetworkEntity()
{
  networkEntityLoadData();
  networkEntityExternal=new NetworkEntity(0);
  networkEntityExternal->position=glm::vec3(0.0,0.0,200.0);
  networkEntitySink=new NetworkEntity(0);
  networkEntitySink->position=glm::vec3(0.0,0.0,-200.0);
}


NetworkEntity* getNetworkEntity(const unsigned int address)
{
  if( (!isInSubnet(address)) && (!isMulticast(address)) )
    return networkEntityExternal;
  for(int i=0;i<networkEntityList.size();i++)
  {
    if(networkEntityList[i]->ipAddr==address)
      return networkEntityList[i];
  }
  return networkEntitySink;
}

NetworkEntity* addNetworkEntity(const unsigned int address)
{
  if(!isInSubnet(address))
    return networkEntityExternal;
  if(isMulticast(address))
    return networkEntitySink;
  for(int i=0;i<networkEntityList.size();i++)
  {
    if(networkEntityList[i]->ipAddr==address)
      return networkEntityList[i];
  }
  NetworkEntity* newEnt = new NetworkEntity(address);
  std::vector<NetworkEntity*>::iterator it=networkEntityList.end();
  networkEntityList.insert(it,newEnt);

  float num;
  glm::vec3 circleradius(networkEntityList.size()*13.0+20.0);
  glm::vec3 newpos;
  for(int i=0;i<networkEntityList.size();i++)
  {
    num=((float)i)/networkEntityList.size();
    newpos=circleradius*glm::vec3(sin(2*M_PI*num),cos(2*M_PI*num),0.0);
    networkEntityList[i]->position=newpos;
  }
  return newEnt;
}

void drawNetworkEntities()
{
  networkEntityExternal->color_vector.x=0.5*sin(2*M_PI*(glutGet(GLUT_ELAPSED_TIME)/5000.0))+0.5;
  networkEntityExternal->color_vector.y=0.5*sin(2*M_PI*(glutGet(GLUT_ELAPSED_TIME)/5000.0+0.333))+0.5;
  networkEntityExternal->color_vector.z=0.5*sin(2*M_PI*(glutGet(GLUT_ELAPSED_TIME)/5000.0+0.666))+0.5;
  netent_M_att_texture_external->enable();
  networkEntityExternal->draw();
  netent_M_att_texture_external->disable();
  netent_M_att_texture_sink->enable();
  networkEntitySink->draw();
  netent_M_att_texture_sink->disable();
  netent_M_att_texture->enable();
  for(int i=0;i<networkEntityList.size();i++)
    networkEntityList[i]->draw();
  netent_M_att_texture->disable();
}

NetworkEntity::NetworkEntity(const unsigned int addr)
{
  ipAddr=addr;

  scale_vector=glm::vec3(20.0);
  color_vector=glm::vec3(0.5,0.5,0.5);
  position=glm::vec3(0.5,0.5,0.5);


  att_position= new Attribute_unif_vec3(uniform_pos,glm::value_ptr(position));
  att_color= new Attribute_unif_vec3(uniform_col,glm::value_ptr(color_vector));
  att_scale= new Attribute_unif_vec3(uniform_scale,glm::value_ptr(scale_vector));

  graphic = new glObject();
  graphic->addElem(netent_M_elems);

  graphic->att_coord=netent_M_att_coord;
  graphic->att_texcoord=netent_M_att_texcoord;

  graphic->att_position=att_position;
  graphic->att_color=att_color;
  graphic->att_scale=att_scale;
}

NetworkEntity::~NetworkEntity()
{
  delete graphic;
}

void NetworkEntity::draw()
{
  graphic->draw();
}


void networkEntityLoadData()
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
  GLfloat colors[]={  1.0, 0.0, 0.0,
                    0.4, 0.4, 0.2,
                    0.0, 1.0, 0.0,
                    0.333, 0.333, 0.334,
                    0.4, 0.2, 0.4,
                    0.333, 0.333, 0.333,
                    0.2, 0.4, 0.4,
                    0.0, 0.0, 1.0 
    };
  netent_M_att_coord=new Attribute_vbo(*new VertexBufferObject(coords,3,sizeof(coords)),"coord3d",*simpleProgram);
  netent_M_att_texcoord=new Attribute_vbo(*new VertexBufferObject(texcoords,2,sizeof(texcoords)),"tex_coord",*simpleProgram);
  netent_M_att_texture=new Attribute_texture(*new Texture(NETWORK_VISUALIZER_RESSOURCE_PATH "netentitity.png"), GL_TEXTURE0, "texture", *simpleProgram);
  netent_M_att_texture_sink=new Attribute_texture(*new Texture(NETWORK_VISUALIZER_RESSOURCE_PATH "local.png"), GL_TEXTURE0, "texture", *simpleProgram);
  netent_M_att_texture_external=new Attribute_texture(*new Texture(NETWORK_VISUALIZER_RESSOURCE_PATH "external.png"), GL_TEXTURE0, "texture", *simpleProgram);
  netent_M_elems = new glElement(elems,sizeof(elems));
}

