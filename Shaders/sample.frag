#version 330 core

uniform sampler2D tex0;
uniform sampler2D norm_tex;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform float pointAmbientStr;
uniform vec3 pointAmbientColor;
uniform float pointSpecStr;
uniform float pointSpecPhong;

uniform vec3 dirLightPos;
uniform vec3 dirLightColor;
uniform float dirAmbientStr;
uniform vec3 dirAmbientColor;
uniform float dirSpecStr;
uniform float dirSpecPhong;

uniform float pointIntensity;
uniform float dirIntensity;

uniform vec3 cameraPos;

in vec2 texCoord;
uniform vec3 objColor;
uniform vec3 viewColor;

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

	// Point Light
    vec3 lightDir = normalize(pointLightPos - fragPos);

	float diff = max(dot(normal, lightDir), 1.0);
	vec3 diffuse = diff * pointLightColor;

	vec3 ambientCol = pointAmbientColor * pointAmbientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), pointSpecPhong);
	vec3 specColor = spec * pointSpecStr * pointLightColor;

	float distance = pow(pointLightPos.x - fragPos.x, 2) + pow(pointLightPos.y - fragPos.y, 2) + pow(pointLightPos.z - fragPos.z, 2);

	float intensity = 1.f / distance;

	specColor  *= intensity;
    diffuse  *= intensity;
    ambientCol *= intensity;

	vec3 pointResult = specColor + diffuse + ambientCol;
	pointResult *= pointIntensity;

	// Direction Light
	lightDir = normalize(-dirLightPos);

	diff = max(dot(normal, lightDir), 0.0);
	diffuse = diff * dirLightColor;

	ambientCol = dirAmbientColor * dirAmbientStr;

	viewDir = normalize(cameraPos - fragPos);
	reflectDir = reflect(-lightDir, normal);

	spec = pow(max(dot(reflectDir, viewDir), 0.4), dirSpecPhong);
	specColor = spec * dirSpecStr * dirLightColor;

	vec3 dirResult = specColor + diffuse + ambientCol;
	dirResult *= dirIntensity;

	FragColor = vec4(pointResult + dirResult, 1.0) * texture(tex0, texCoord) + vec4(objColor, 1.0) + vec4(viewColor, 1.0);
}