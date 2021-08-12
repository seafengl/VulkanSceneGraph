#include <vsg/io/VSG.h>
static auto assimp_vert = []() {std::istringstream str(
R"(#vsga 0.1.5
Root id=1 vsg::ShaderStage
{
  NumUserObjects 0
  stage 1
  entryPointName "main"
  module id=2 vsg::ShaderModule
  {
    NumUserObjects 0
    Source "#version 450
#extension GL_ARB_separate_shader_objects : enable

#pragma import_defines (VSG_INSTANCE_POSITIONS)

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelView;
} pc;

layout(location = 0) in vec3 vsg_Vertex;
layout(location = 0) out vec3 worldPos;

layout(location = 1) in vec3 vsg_Normal;
layout(location = 1) out vec3 normalDir;

layout(location = 2) in vec4 vsg_Color;
layout(location = 2) out vec4 vertexColor;

layout(location = 3) in vec2 vsg_TexCoord0;
layout(location = 3) out vec2 texCoord0;

#ifdef VSG_INSTANCE_POSITIONS
layout(location = 4) in vec3 vsg_position;
#endif

layout(location = 5) out vec3 viewDir;
layout(location = 6) out vec3 lightDir;

out gl_PerVertex{ vec4 gl_Position; };

void main()
{
    vec4 vertex = vec4(vsg_Vertex, 1.0);

#ifdef VSG_INSTANCE_POSITIONS
   vertex.xyz = vertex.xyz + vsg_position;
#endif

    gl_Position = (pc.projection * pc.modelView) * vertex;
    worldPos = vec4(pc.modelView * vertex).xyz;

    normalDir = (pc.modelView * vec4(vsg_Normal, 0.0)).xyz;

    vec4 lpos = /*vsg_LightSource.position*/ vec4(0.0, 0.25, 1.0, 0.0);

    viewDir = -vec3(pc.modelView * vertex);

    if (lpos.w == 0.0)
        lightDir = lpos.xyz;
    else
        lightDir = lpos.xyz + viewDir;

    vertexColor = vsg_Color;

    texCoord0 = vsg_TexCoord0;
}
"
    hints id=0
    SPIRVSize 0
    SPIRV
  }
  NumSpecializationConstants 0
}
)");
vsg::VSG io;
return io.read_cast<vsg::ShaderStage>(str);
};