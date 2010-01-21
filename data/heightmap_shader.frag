uniform sampler2D alpha;
uniform sampler2D tex1;
uniform sampler2D tex2;

varying vec3 normal, lightDir ;

void main()
{
	vec3 texAlpha    = texture2D( alpha, gl_TexCoord[0].st ).rgb;
	vec3 texel1 = texture2D( tex1, gl_TexCoord[0].st ).rgb;
	vec3 tx = mix(vec3(0.5,0.5,0.5), texel1, texAlpha.r);
	vec3 texel2 = texture2D( tex2, gl_TexCoord[0].st ).rgb;
	tx = mix(tx, texel2, texAlpha.g);

/*	if(tex2!=4)*/
	{
/*		vec3 texel2 = texture2D( tex2, gl_TexCoord[0].st ).rgb;
		tx = mix(tx, texel2, texAlpha.g);
*/	}

/*	vec3 tx = texAlpha;
*/
/*	vec3 tx = vec3(0, 0, 0);
	tx = mix(texGrass, tx, alpha.r);
*/
/*	vec3 dl = gl_LightSource[0].diffuse.rgb;   
	vec3 al = gl_LightSource[0].ambient.rgb;
	vec3 n = normalize(normal);
	vec3 d = tx * (dl * max( dot ( n, lightDir), 0.0 ) + al );   

	vec4 finalColor = vec4( min(d, 1.0), 1.0);

	gl_FragColor = finalColor;
*/
	gl_FragColor = vec4(tx, 1.0);
}
