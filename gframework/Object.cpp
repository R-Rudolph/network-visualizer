#include "Object.hpp"

glObject::~glObject()
{
}

glObject::glObject()
{
}

void glObject::addElem(glElement* elem)
{
  elems=elem;
}

void glObject::remElem(glElement elem)
{
}

void glObject::draw()
{
  att_position->enable();
  att_color->enable();
  att_scale->enable();
  elems->draw();
}
