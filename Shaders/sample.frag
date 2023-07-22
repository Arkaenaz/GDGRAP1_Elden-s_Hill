#version 330 core

uniform sampler2D tex0;
uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStr;
uniform vec3 ambientColor;
uniform float specStr;
uniform float specPhong;

uniform float intensity;

uniform vec3 cameraPos;

in vec2 texCoord;
uniform vec3 objColor;

in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;

void main() {
	vec4 pixelColor = texture(tex0, texCoord);
	if (pixelColor.a < 0.5) {
		discard;
	}
	//vec3 normal = normalize(normCoord);
	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	normal = normalize(TBN * normal);
	vec3 lightDir = normalize(-lightPos);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	vec3 result = specColor + diffuse + ambientCol;
	//result *= intensity; im also a dumbass for not commenting this out

	FragColor = vec4(result, 1.0) * texture(tex0, texCoord) + vec4(objColor, 1.0);
}