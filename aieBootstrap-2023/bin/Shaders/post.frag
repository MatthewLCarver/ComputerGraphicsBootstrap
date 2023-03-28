// A simple flat-color shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colorTarget;
uniform int postProcessTarget;
uniform int windowWidth;
uniform int windowHeight;
uniform float deltaTime;

uniform vec3 lightPosition;
uniform vec3 eyePosition;

//0 linear; 1 exponential; 2 exponential square
uniform int fogSelector;
//0 plane based; 1 range based
uniform int depthFog;

out vec4 FragColor;


float Random (vec2 st) {
    return fract(sin(dot(st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}



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

vec4 EdgeDetection(vec2 texCoord)
{
    float w = 1.0f / windowWidth;
    float h = 1.0f / windowHeight;
    
    vec4 k[9];
    k[0] = texture(colorTarget, texCoord + vec2(-w, -h));
    k[1] = texture(colorTarget, texCoord + vec2( 0, -h));
    k[2] = texture(colorTarget, texCoord + vec2( w, -h));
    
    k[3] = texture(colorTarget, texCoord + vec2(-w,  0));
    k[4] = texture(colorTarget, texCoord);
    k[5] = texture(colorTarget, texCoord + vec2( w,  0));
    
    k[6] = texture(colorTarget, texCoord + vec2(-w,  h));
    k[7] = texture(colorTarget, texCoord + vec2( 0,  h));
    k[8] = texture(colorTarget, texCoord + vec2( w,  h));
    
    vec4 sobelEdgeH = k[2] + (2.0f * k[5]) + k[8] - (k[0] + (2.0f * k[3]) + k[6]);
    vec4 sobelEdgeV = k[0] + (2.0f * k[1]) + k[2] - (k[6] + (2.0f * k[7]) + k[8]);
    
    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));
    
    return vec4(1.0f - sobel.rgb, 1.0f);
}

vec4 Sepia(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    float r = color.r;
    float g = color.g;
    float b = color.b;
    
    color.r = (r * 0.393f) + (g * 0.769f) + (b * 0.189f);
    color.g = (r * 0.349f) + (g * 0.686f) + (b * 0.168f);
    color.b = (r * 0.272f) + (g * 0.534f) + (b * 0.131f);
    
    return color;
}

vec4 ScanLines(vec2 texCoord)
{
    vec2 uv = texCoord / (windowWidth / windowHeight);
    vec3 color = texture(colorTarget, texCoord).rgb;

    float count = windowHeight * 1.3f;
    vec2 sl = vec2(sin(uv.y * count), cos(uv.y * count));
    vec3 scanlines = vec3(sl.x, sl.y, sl.x);

    color += color * scanlines * 0.3f;
    color += color * vec3(Random(uv * deltaTime)) * 0.2f;
    color += color * sin(110.0 * deltaTime) * 0.03f;
    
    return vec4 (color, 1);
}

vec4 GreyScale(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    float grey = (color.r + color.g + color.b) / 3.0f;
    return vec4(grey, grey, grey, 1.0f);
}

vec4 Invert(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    return vec4(1.0f - color.rgb, 1.0f);
}

vec4 Pixelizer(vec2 texCoord)
{
    vec2 uv = texCoord / (windowWidth / windowHeight);
    float px = 8 * (1.0f / windowWidth);
    float py = 8 * (1.0f / windowHeight);
    vec2 c = vec2(px * floor(uv.x / px), py * floor(uv.y / py));
    
    vec4 color = texture(colorTarget, c);
    return vec4(color.rgb, 1.0f);
}

vec4 Posterize(vec2 texCoord)
{
    vec2 uv = texCoord / (windowWidth / windowHeight);
    vec4 inputColor = texture(colorTarget, texCoord);
    
    vec3 color = 0.5 + 0.5*cos(deltaTime + uv.xyx + vec3(0, 2, 4));
    
    float gamma = 0.25f;
    float numColors = 10.0f;

    vec3 c = inputColor.rgb;
    c = pow(c, vec3(gamma, gamma, gamma));
    c = c * numColors;
    c = floor(c);
    c = c / numColors;
    c = pow(c, vec3(1.0/gamma));

    return vec4(c, inputColor.a);
}

vec4 DistanceFog(vec2 texCoord)
{
    vec3 fogColor = vec3(0.5, 0.5,0.5);
    float FogDensity = 0.05;
    vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
    vec3 RimColor = vec3(0.2, 0.2, 0.2);
    vec3 world_pos = texture(colorTarget, texCoord).rgb;
    vec3 world_normal = texture(colorTarget, texCoord).rgb;
    vec4 viewspace = texture(colorTarget, texCoord);
    
    vec3 color = texture(colorTarget, texCoord).rgb;

    //get light an view directions
    vec3 L = normalize( lightPosition - world_pos);
    vec3 V = normalize( eyePosition - world_pos);

    //diffuse lighting
    vec3 diffuse = DiffuseLight * max(0, dot(L,world_normal));

    //rim lighting
    float rim = 1 - max(dot(V, world_normal), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    vec3 finalRim = RimColor * vec3(rim, rim, rim);
    //get all lights and texture
    vec3 lightColor = finalRim + diffuse + color;

    vec3 finalColor = vec3(0, 0, 0);

    //distance
    float dist = 0;
    float fogFactor = 0;

    //compute distance used in fog equations
    if(depthFog == 0)//select plane based vs range based
    {
        //plane based
        dist = abs(viewspace.z);
        //dist = (gl_FragCoord.z / gl_FragCoord.w);
    }
    else
    {
        //range based
        dist = length(1);
    }

    if(fogSelector == 0)//linear fog
    {
        // 20 - fog starts; 80 - fog ends
        fogFactor = (80 - dist)/(80 - 20);
        fogFactor = clamp( fogFactor, 0.0, 1.0 );

        //if you inverse color in glsl mix function you have to
        //put 1.0 - fogFactor
        finalColor = mix(fogColor, lightColor, fogFactor);
    }
    else if( fogSelector == 1)// exponential fog
    {
        fogFactor = 1.0 /exp(dist * FogDensity);
        fogFactor = clamp( fogFactor, 0.0, 1.0 );

        // mix function fogColor⋅(1−fogFactor) + lightColor⋅fogFactor
        finalColor = mix(fogColor, lightColor, fogFactor);
    }
    else if( fogSelector == 2)
    {
        fogFactor = 1.0 /exp( (dist * FogDensity)* (dist * FogDensity));
        fogFactor = clamp( fogFactor, 0.0, 1.0 );

        finalColor = mix(fogColor, lightColor, fogFactor);
    }

    //show fogFactor depth(gray levels)
    //fogFactor = 1 - fogFactor;
    //out_color = vec4( fogFactor, fogFactor, fogFactor,1.0 );
    return vec4(finalColor, 1);
}

vec4 Contrast(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    return vec4(color.rgb * 3.0f, 1.0f);
}

vec4 Rage(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    float red = color.r;
    float green = 0;
    float blue = 0;
    // not using alpha in this tutorial
    float alpha = 1.0;
    // Output to screen    
    
    
    return vec4(red, green, blue, alpha);
}

vec4 Depression(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    float red = 0;
    float green = 0;
    float blue =  color.b;
    // not using alpha in this tutorial
    float alpha = 1.0;
    // Output to screen    
    
    
    return vec4(red, green, blue, alpha);
}

vec4 Greed(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    float red = 0;
    float green = color.g;
    float blue =  0;
    // not using alpha in this tutorial
    float alpha = 1.0;
    // Output to screen    
    
    
    return vec4(red, green, blue, alpha);
}

vec4 BlackAndWhite(vec2 vTexCoord)
{
    vec4 color = texture(colorTarget, vTexCoord);
    float luma = 0.333333 * (color.r + color.g + color.b);
    float l= mix(0.0, 1.0, step(.05f, luma));
    return vec4(l, l, l, 1.0f);
}

vec4 Vignette(vec2 texCoord)
{
    vec4 color = texture(colorTarget, texCoord);
    vec2 uv = texCoord / (windowWidth / windowHeight);
    float vignette = 1.0f - smoothstep(0.0f, 0.8f, length(uv - vec2(0.5f, 0.5f)));
    return vec4(color.rgb * vignette, 1.0f);
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
            FragColor = EdgeDetection(texCoord);
            return;
        case 3:
            // Sepia
            FragColor = Sepia(texCoord);
            return;
        case 4:
            // ScanLines
            FragColor = ScanLines(texCoord);
            return;
        case 5:
            // Grey Scale
            FragColor = GreyScale(texCoord);
            return;
        case 6:
            // Invert
            FragColor = Invert(texCoord);
            return;
        case 7:
            // Pixelizer
            FragColor = Pixelizer(texCoord);
            return;
        case 8:
            // Posterize
            FragColor = Posterize(texCoord);
            return;
        case 9:
            // Distance Fog
            FragColor = DistanceFog(texCoord);
            return;
        case 10:
            // Depth of Field
            FragColor = Default(texCoord);
            return;
        case 11:
            // Contrast
            FragColor = Contrast(texCoord);
            return;
        case 12:
            // Rage
            FragColor = Rage(texCoord);
            return;
        case 13:
            // Depression
            FragColor = Depression(texCoord);
            return;
        case 14:
            // Greed
            FragColor = Greed(texCoord);
            return;
        case 15:
            // Black and White
            FragColor = BlackAndWhite(texCoord);
            return;
        case 16:
            // Vignette
            FragColor = Vignette(texCoord);
            return;
        default:
            // Do nothing
            break;
    }
    
    // Output the post process effect here
    FragColor = Default(texCoord);
}