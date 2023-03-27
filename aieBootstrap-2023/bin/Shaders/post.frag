// A simple flat-color shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;

out vec4 FragColor;

vec4 Default(vec2 texCoord)
{
    return texture(colorTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colorTarget, 0);
    vec4 color = texture(colorTarget, texCoord);
    color += texture(colorTarget, texCoord + texel * vec2(-1,  1 ));
    color += texture(colorTarget, texCoord + texel * vec2(-1,  0 ));
    color += texture(colorTarget, texCoord + texel * vec2(-1, -1 ));
     
    color += texture(colorTarget, texCoord + texel * vec2( 0,  1 ));
    color += texture(colorTarget, texCoord + texel * vec2( 0, -1 ));
      
    color += texture(colorTarget, texCoord + texel * vec2( 1,  1 ));
    color += texture(colorTarget, texCoord + texel * vec2( 1,  0 ));
    color += texture(colorTarget, texCoord + texel * vec2( 1, -1 ));
    
    return color / 9.0f;
}

vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromTheCenter = distance(texCoord, mid);
    vec2 normalizeCoord = normalize(texCoord - mid);
    float bias = distanceFromTheCenter + sin(distanceFromTheCenter * 15.0f) * 0.05f;
    vec2 newCoord = mid + bias * normalizeCoord;
    
    return texture(colorTarget, newCoord);
}

void main()
{
    vec2 texSize = textureSize(colorTarget, 0);
    vec2 texelSize = 1.0 / texSize;
    
    // THen we will adjust the coordinate
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;
    
    switch(postProcessTarget)
    {
        case 0:
            // Box blur
            FragColor = BoxBlur(texCoord);
            break;
        case 1:
            // distort
            FragColor = Distort(texCoord);
            return;
        case 2:
            // Edge Detection
            FragColor = Default(texCoord);
            return;
        case 3:
            // Sepia
            FragColor = Default(texCoord);
            return;
        case 4:
            // ScanLines
            FragColor = Default(texCoord);
            return;
        case 5:
            // Grey Scale
            FragColor = Default(texCoord);
            return;
        case 6:
            // Invert
            FragColor = Default(texCoord);
            return;
        case 7:
            // Pixelizer
            FragColor = Default(texCoord);
            return;
        case 8:
            // Posterize
            FragColor = Default(texCoord);
            return;
        case 9:
            // Distance Fog
            FragColor = Default(texCoord);
            return;
        case 10:
            // Depth of Field
            FragColor = Default(texCoord);
            return;
        default:
            // Do nothing
            break;
    }
    
    // Output the post process effect here
    FragColor = Default(texCoord);
}