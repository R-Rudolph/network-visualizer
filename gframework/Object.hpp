#ifndef __GFRAMEWORK_OBJECT_HPP_INCLUDED__
#define __GFRAMEWORK_OBJECT_HPP_INCLUDED__

#include "Element.hpp"
#include "Attribute.hpp"

class glObject
{
    glElement* elems;
  public:
    glObject();
    ~glObject();
    Attribute* att_coord;
    Attribute* att_texcoord;
    Attribute* att_position;
    Attribute* att_color;
    Attribute* att_scale;
    Attribute* att_texture;
    void addAttr(Attribute* attr);
    void remAttr(Attribute attr);
    void addElem(glElement*  elem);
    void remElem(glElement elem);
    void addObject(glObject*  obj);
    void remObject(glObject obj);
    void draw();
};

#endif
