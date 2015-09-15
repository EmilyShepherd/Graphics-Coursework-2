#version 150

#define LIGHTS 6

precision highp float;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_UV;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform float ambiance;
uniform vec3 LightPosition_worldspace[LIGHTS];
uniform vec3 SpotDirection_worldspace[LIGHTS];
uniform int lightCount;

out vec4 normal;
out vec2 UV;
out vec4 light;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace[LIGHTS];
out vec3 SpotDirection_cameraspace[LIGHTS];
out vec3 Normal_cameraspace;

void main(void)
{
    gl_Position = MVP * vec4(in_Position, 1.0);

    Position_worldspace = (M * vec4(in_Position,1)).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( V * M * vec4(in_Position,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    for (int i = 0; i < lightCount; i++)
    {
        // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
        vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace[i],1)).xyz;
        LightDirection_cameraspace[i] = LightPosition_cameraspace + EyeDirection_cameraspace;
        SpotDirection_cameraspace[i] = - ( V * normalize(vec4(SpotDirection_worldspace[i],0))).xyz;
    }

    // Normal of the the vertex, in camera space
    Normal_cameraspace = ( V * M * vec4(in_Normal,0)).xyz;

    UV = in_UV;
}

