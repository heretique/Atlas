$input a_position, a_texcoord0, a_color0
$output v_color0, v_stipple

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_color0    = a_color0;
	v_stipple   = a_texcoord0.x;
}
