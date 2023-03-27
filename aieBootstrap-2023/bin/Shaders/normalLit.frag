// Our phong shader with lighting and textures
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 FragColor;

// Camera Data
uniform vec3 CameraPosition;

// Texture Data
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// Model Data
uniform vec3 Ka; // The ambient material color
uniform vec3 Kd; // The diffuse color
uniform vec3 Ks; // The specular color
uniform float specularPower; // The specular power of Ks

//uniform vec4 BaseColor;
uniform vec3 AmbientColor;
uniform vec3 LightColor;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColors[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);
    float specularTerm = pow(max(0, dot(R, view)), specularPower);
    return specularTerm * color;
}

bool CheckTextures()
{
    // check if the textures are valid
    vec3 texDiffuseCol = texture2D(diffuseTexture, vTexCoord).rgb;
    if(length(texDiffuseCol) == 0.0)
    {
        return false;
    }
    
    return true;
}

void main()
{
    // Set the normal and light direction
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    mat3 TBN = mat3(T,B,N);


    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
    
    N = normalize(TBN * (texNormal * 2 - 1));

    // Calculate the negative light direction (Lambert Term)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate the view vector
    vec3 V = normalize(CameraPosition -  vPosition.xyz);
    // and the reflection vector
    vec3 R = reflect(L, N);

    // calculate the diffuse value of light from the global source
    vec3 diffuseTotal = Diffuse(L, LightColor, N);
    
    // Calculate the specular value of light from the global source
    vec3 specularTotal = Specular(L, LightColor, N, V);
    
    for(int i = 0; i < numLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float distance = length(direction);
        direction = direction / distance;
        
        // Set the lighting intensity with the inverse square law
        vec3 color = PointLightColors[i] / (distance * distance);
        
        diffuseTotal += Diffuse(direction, color, N);
        specularTotal += Specular(direction, color, N, V);
    }
    
    // Calculate the properties of each color
    vec3 ambient = AmbientColor * Ka * texDiffuse;
    vec3 diffuse = diffuseTotal * Kd * texDiffuse;
    vec3 specular = specularTotal * Ks * texSpecular;
    
    
    
    N = TBN * (texNormal * 20 - 1);
    lambertTerm = max(0, min(1, dot(N, -L)));

    vec4 cellShade = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (lambertTerm < 1.0f)
    {
        cellShade = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // Sobel filter
    vec3 dx = dFdx(N);
    vec3 dy = dFdy(N);
    vec3 d = normalize(cross(dx, dy));
    float f = abs(dot(N, d));
    
    vec2 texelSize = vec2(1.0f / 800.0f, 1.0f / 600.0f);
    dx = vec3(texelSize.x, 0.0f, texture(normalTexture, vTexCoord + vec2(texelSize.x, 0.0f)).a - texture(normalTexture, vTexCoord).a);
    dy = vec3(0.0f, texelSize.y, texture(normalTexture, vTexCoord + vec2(0.0f, texelSize.y)).a - texture(normalTexture, vTexCoord).a);
    d = normalize(cross(dx, dy));
    f = abs(dot(N, d));
    
    if (f < 0.9f)
    {
        cellShade = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    
    
    
    
    
    
    
    
    
    /*
    no normal map
    vec2 texelSize = vec2(1.0f / 800.0f, 1.0f / 600.0f);
    vec3 dx = vec3(texelSize.x, 0.0f, texture(normalTexture, vTexCoord + vec2(texelSize.x, 0.0f)).a - texture(normalTexture, vTexCoord).a);
    vec3 dy = vec3(0.0f, texelSize.y, texture(normalTexture, vTexCoord + vec2(0.0f, texelSize.y)).a - texture(normalTexture, vTexCoord).a);
    vec3 d = normalize(cross(dx, dy));
    float f = abs(dot(N, d));
    
    if (f < 0.1f)
    {
        cellShade = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }*/
    
    
    
    
    
    
    
    
    
    /*
    ... from the object
    vec2 texCoord = vTexCoord;
    vec4 texel = texture(normalTexture, texCoord);
    vec3 normal = texel.rgb * 2.0f - 1.0f;
    float depth = texel.a;
    
    vec2 texelSize = vec2(1.0f / 800.0f, 1.0f / 600.0f);
    vec3 dx = vec3(texelSize.x, 0.0f, texture(normalTexture, texCoord + vec2(texelSize.x, 0.0f)).a - depth);
    vec3 dy = vec3(0.0f, texelSize.y, texture(normalTexture, texCoord + vec2(0.0f, texelSize.y)).a - depth);
    vec3 d = normalize(cross(dx, dy));
    float f = abs(dot(normal, d));
    
    if (f < 0.9f)
    {
        rCopy = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }*/
    
    
    


    // Google OpenGL edge detection - Sobel Filter
 
    vec4 phong = vec4(ambient + diffuse + specular, 1.0f);
    
    FragColor = phong * cellShade;
}