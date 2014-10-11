#ifndef __MOVINGCUBES_HPP_INCLUDED__
#define __MOVINGCUBES_HPP_INCLUDED__

#include <mutex>
#include <vector>
#include "sniffer.hpp"
#include "gframework/graphicsframework.hpp"
#include "NetworkEntities.hpp"
#include "Ressources.hpp"

class MovingCube
{
    glObject* graphic;
    NetworkEntity* destination_netent;
    Attribute* att_position;
    Attribute* att_color;
    Attribute* att_scale;
    int factor;
    int move;
    u_char *content;
    bpf_u_int32 contentlength;
    float distance;
  public:
    glm::vec3 destination();
    int end;
    glm::vec3 col_vector;
    glm::vec3 position;
    glm::vec3 sca_vector;
    void printContent();
    MovingCube(int dir,Attribute* coord, Attribute* texcoord, Attribute* texture, GLint unif_position, GLint color, GLint scale, glElement* elements, glm::vec3 color_vec, glm::vec3 scale_vector,const u_char* content, bpf_u_int32 contentlength);
    ~MovingCube();
    int activate(int t_passed);
    int update(int t_passed);
    void draw();
    void toogleHalt();
    glObject* getObject();
};

extern std::vector<MovingCube*> movingCubes;

void addNewNetworkPackage(int direction, glm::vec3 color, glm::vec3 scale_vec, const u_char* content, bpf_u_int32 contentlength);
void drawNetworkPackages(int t_passed);
void* updateNetworkPackages(void* arg);
MovingCube* getNearestCube(glm::vec3 coords);
void init_MovingCubes();

#endif

