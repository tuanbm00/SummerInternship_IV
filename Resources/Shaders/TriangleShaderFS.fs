precision mediump float;

varying vec2 v_uv;
void main()
{
	//Color
	//gl_FragColor = v_color;
	
	//Texture
	vec4 color = vec4(1.0, 0.7,0.0, 1.0);
	gl_FragColor = texture2D(color, v_uv);
}
