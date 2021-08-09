attribute vec3 a_Position;
attribute vec2 a_uv;

varying vec2 v_uv;

uniform mat4 u_WVP;

void main()
{
	gl_Position = u_WVP * vec4(a_Position, 1.0);
	v_uv = a_uv;
}