#version 150

#define LIGHTS 6

precision highp float;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D texture;
uniform int doTexture;
uniform vec3 LightPosition_worldspace[LIGHTS];
uniform vec3 ambiantDirection;
uniform int illum;
uniform int enableSpots;
uniform float ambiance;
uniform int lightCount;

in vec2 UV;

in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[LIGHTS];
in vec3 SpotDirection_cameraspace[LIGHTS];

void main(void)
{
    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );
    vec3 E = normalize(EyeDirection_cameraspace);

    if (dot(E,n) < 0)
    {
        //FragColor = vec4(1.0);
        //discard;
    }

    vec4 C;

    if (doTexture == 1)
    {
        C = texture(texture, UV);
    }
    else
    {
        C = vec4(color, 1.0);
    }

    if (illum == 3)
    {
        FragColor = C;
        return;
    }
    else if (illum == 4)
    {
        if (enableSpots == 1)
        {
            FragColor = vec4(1.0, 1.0, 0.0, 1.0);
        }
        else
        {
            FragColor = vec4(0.0);
        }
        return;
    }

    vec3 LightColor = vec3(1,1,1);
    float LightPower = 200.0f;
    
    vec3 MaterialDiffuseColor = C.rgb;
    vec3 MaterialAmbientColor = 0.1 * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(1.0,1.0,1.0);

    vec3 Co = MaterialAmbientColor;

    if (n.y >= -0.5)
    {
        float cosThetaAmiant = clamp( dot( n,ambiantDirection ), 0,1 );

        Co += MaterialDiffuseColor * vec3(1.0, 1., 1.) * cosThetaAmiant;
    }

    if (n.y == 0.0)
    {
        Co += ambiance * MaterialDiffuseColor;
    }

    if (enableSpots == 1)
    {
        for (int i = 0; i < lightCount; i++)
        {
            vec3 ld = normalize(LightDirection_cameraspace[i]);
            vec3 sd = normalize(SpotDirection_cameraspace[i]);

            if (dot(sd, ld) >= 0.6)
            {
                float distance = length( LightPosition_worldspace[i] - Position_worldspace );

                // Cosine of the angle between the normal and the light direction, 
                // clamped above 0
                //  - light is at the vertical of the triangle -> 1
                //  - light is perpendicular to the triangle -> 0
                //  - light is behind the triangle -> 0
                float cosTheta = clamp( dot( n,ld ), 0,1 );
                
                // Direction in which the triangle reflects the light
                vec3 R = reflect(-ld,n);
                // Cosine of the angle between the Eye vector and the Reflect vector,
                // clamped to 0
                //  - Looking into the reflection -> 1
                //  - Looking elsewhere -> < 1
                float cosAlpha = clamp( dot( E,R ), 0,1 );

                Co += 
                    // Diffuse : "color" of the object
                    MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
                    // Specular : reflective highlight, like a mirror
                    MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
            }
        }
    }

    FragColor = vec4(Co, 1.0);
}
