attribute vec3 a_Position;
varying vec4 v_pos;
varying float v_dist;
uniform mat4 u_WVP;

void main()
{
	v_dist = length(u_WVP * vec4(a_Position, 1.0));
	gl_Position = u_WVP * vec4(a_Position, 1.0);
	v_pos = vec4(a_Position, 1.0);
}