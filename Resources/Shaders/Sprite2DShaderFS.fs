precision mediump float;

varying vec2 v_uv;
uniform sampler2D u_Texture1;


void main()
{
	//Texture
	gl_FragColor = texture2D(u_Texture1, v_uv);
}