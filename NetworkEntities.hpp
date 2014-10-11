#ifndef __NETWORKENTITIES_HPP_INCLUDED__
#define __NETWORKENTITIES_HPP_INCLUDED__

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
#include "gframework/graphicsframework.hpp"
#include "Ressources.hpp"

class NetworkEntity
{
    glObject* graphic;
    glm::vec3 scale_vector;
    Attribute* att_position;
    Attribute* att_color;
    Attribute* att_scale;
  public:
    glm::vec3 color_vector;
    void draw();
    NetworkEntity(unsigned int addr);
    ~NetworkEntity();
    unsigned int ipAddr;
    glm::vec3 position;
};

int isInSubnet(const unsigned int addr);
int isMulticast(const unsigned int address);
int defineNetwork(unsigned int ipaddr,unsigned int subnetmask);
void networkEntityLoadData();
void initNetworkEntity();
NetworkEntity* getNetworkEntity(const unsigned int address);
NetworkEntity* addNetworkEntity(const unsigned int address);
void drawNetworkEntities();
void networkEntityLoadData();

#endif

