precision mediump float;
uniform samplerCube u_samplerCubeMap;

varying vec4 v_pos;
varying float v_dist;
void main(void)
{
	float lerpValue = clamp((v_dist - 1.0)/100.0, 0.0, 1.0);
	vec4 FogColor = vec4(0.6, 0.6, 0.6, 0.6);
	gl_FragColor = FogColor * lerpValue + textureCube(u_samplerCubeMap, v_pos.xyz) * (1.0 - lerpValue);
}