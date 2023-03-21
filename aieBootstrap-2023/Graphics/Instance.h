#pragma once
#include <glm/glm.hpp>

class FlyCamera;
class Scene;
struct Light;

namespace aie
{
    class OBJMesh;
    class ShaderProgram;
}

class Instance
{
public:
    Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader);
    
    void Draw(Scene* _scene);
    

protected:
    glm::mat4 m_transform;
    aie::OBJMesh* m_mesh;
    aie::ShaderProgram* m_shader;
};
