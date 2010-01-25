uniform sampler2D alpha;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform int use_tex1;
uniform int use_tex2;
uniform int use_tex3;
uniform int use_tex4;

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;

void main()
{
	vec3 texAlpha    = texture2D( alpha, gl_TexCoord[0].st ).rgb;

	vec3 tx = vec3(0.5,0.5,0.5);
	
	if(use_tex1)
	{
		vec3 texel1 = texture2D( tex1, gl_TexCoord[0].st ).rgb;
		tx = mix(tx, texel1, texAlpha.r);
	}

	if(use_tex2)
	{
		vec3 texel2 = texture2D( tex2, gl_TexCoord[0].st ).rgb;
		tx = mix(tx, texel2, texAlpha.g);
	}

	if(use_tex3)
	{
		vec3 texel3 = texture2D( tex3, gl_TexCoord[0].st ).rgb;
		tx = mix(tx, texel3, texAlpha.g);
	}
	if(use_tex4)
	{
		vec3 texel4 = texture2D( tex4, gl_TexCoord[0].st ).rgb;
		tx = mix(tx, texel4, texAlpha.g);
	}


	/* Lighting calculations*/
	vec3 n,halfV;
	float NdotL,NdotHV;
	
	/* The ambient term will always be present */
	vec4 color = ambient;
	
	/* a fragment shader can't write a varying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		color += diffuse * NdotL;
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += gl_FrontMaterial.specular * 
				gl_LightSource[0].specular * 
				pow(NdotHV, gl_FrontMaterial.shininess);
	}

	vec3 ct,cf;
	float at,af;
	ct = tx.rgb;
	at = 1;
	cf = color.rgb;
	af = color.a;
	
	gl_FragColor = vec4(ct * cf, at * af);	
}
