//varying vec3 fs_color;
varying vec2 fs_tex_coord;
uniform sampler2D texture;
uniform vec3 color;

void main()
{

  //gl_FragColor=vec4(fs_color,1.0);
  vec2 flipped_texcoord = vec2(fs_tex_coord.x, 1.0 - fs_tex_coord.y);
  gl_FragColor = texture2D(texture, flipped_texcoord);
  gl_FragColor = vec4(color,1)*gl_FragColor;
}

