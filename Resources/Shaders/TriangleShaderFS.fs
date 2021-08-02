precision mediump float;

varying vec4 v_color;
varying vec2 v_uv;
uniform sampler2D u_texture;
void main()
{
	//Color
	//gl_FragColor = v_color;
	
	//Texture
	gl_FragColor = texture2D(u_texture, v_uv);
}
