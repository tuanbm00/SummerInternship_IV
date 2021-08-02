precision mediump float;

varying vec2 v_uv;
varying float v_dist;

uniform sampler2D u_Texture1; //Fire Texture
uniform sampler2D u_Texture2; //Fire mask Texture 
uniform sampler2D u_Texture3; //Displacement Map
uniform float u_Time;

void main()
{
	vec2 disp = texture2D(u_Texture3, vec2(v_uv.x, v_uv.y + u_Time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * 0.8;
	vec2 uv = v_uv + offset;
	
	//Texture
	float lerpValue = clamp((v_dist - 1.0)/50.0, 0.0, 1.0);
	vec4 FogColor = vec4(1.0, 1.0, 1.0, 1.0);

	gl_FragColor = texture2D (u_Texture1, uv) * (1.0, 1.0, 1.0, texture2D(u_Texture2, v_uv).r) * (1.0 - lerpValue);
}