precision mediump float;

varying vec2 v_uv;
varying float v_dist;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform sampler2D u_BlendTexture;


void main()
{
	//Texture
	vec2 uv = v_uv * 100.0;
	vec4 BT = texture2D(u_BlendTexture, v_uv);
	vec4 tex1 = texture2D(u_Texture1, uv);
	vec4 tex2 = texture2D(u_Texture2, uv);
	vec4 tex3 = texture2D(u_Texture3, uv);
	float lerpValue = clamp((v_dist - 1.0)/50.0, 0.0, 1.0);
	vec4 FogColor = vec4(0.8, 0.8, 0.8, 1.0);

	gl_FragColor = FogColor * lerpValue + ((BT.r * tex1 + BT.g * tex2 + BT.b * tex3) / (BT.r * 2.0 + BT.g + BT.b)) * (1.0 - lerpValue);
}