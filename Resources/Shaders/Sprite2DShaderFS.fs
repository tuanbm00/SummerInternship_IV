precision mediump float;

varying vec2 v_uv;
uniform sampler2D u_Texture1;


void main()
{
	//Texture
	vec4 Color = texture2D(u_Texture1, v_uv);
	if(Color.a < 0.5) discard;
	gl_FragColor = texture2D(u_Texture1, v_uv);
}