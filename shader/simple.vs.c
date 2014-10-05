attribute vec3 coord3d;
attribute vec2 tex_coord;
uniform vec3 obj_pos;
uniform mat4 proj;
uniform vec3 scale;

//varying vec3 fs_color;
varying vec2 fs_tex_coord;

void main()
{
//  fs_color=color;
  fs_tex_coord=tex_coord;
  gl_Position = proj*vec4(scale*coord3d+obj_pos,1);
}
