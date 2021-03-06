/**
 * @file
 * @brief Geoscape vertex shader.
 */


out_qualifier vec2 tex;

out_qualifier vec4 ambientLight;
out_qualifier vec4 diffuseLight;
out_qualifier vec4 specularLight;
out_qualifier vec4 diffuseLight2;

out_qualifier vec3 lightVec;
out_qualifier vec3 lightVec2;
out_qualifier vec3 eyeVec;

uniform vec2 UVSCALE;

void main(void) {
	gl_Position = ftransform();
	tex = gl_MultiTexCoord0.xy * UVSCALE;

	vec4 lightPos = gl_LightSource[0].position;
	ambientLight = gl_LightSource[0].ambient;
	diffuseLight = gl_LightSource[0].diffuse;
	specularLight = gl_LightSource[0].specular;

	vec4 lightPos2 =  gl_LightSource[1].position;
	diffuseLight2 = gl_LightSource[1].diffuse;

	vec3 t, b, n;
	n = normalize(gl_NormalMatrix * gl_Normal);
	t = normalize(cross(n, vec3(1.0, 0.0, 0.0)));
	b = normalize(cross(t, n));

	lightVec.x = dot(lightPos.rgb, t);
	lightVec.y = dot(lightPos.rgb, b);
	lightVec.z = dot(lightPos.rgb, n);

	lightVec2.x = dot(lightPos2.rgb, t);
	lightVec2.y = dot(lightPos2.rgb, b);
	lightVec2.z = dot(lightPos2.rgb, n);

	/* estimate view vector (orthographic projection means we don't really have one) */
	vec4 view = vec4(0.0, 0.0, 100.0, 1.0);
	eyeVec.x = dot(view.xyz, t);
	eyeVec.y = dot(view.xyz, b);
	eyeVec.z = dot(view.xyz, n);
}
