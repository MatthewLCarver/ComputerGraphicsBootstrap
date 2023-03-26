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
    Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader);
    
    void Draw(Scene* _scene);

    glm::mat4 MakeTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale);

protected:
    glm::mat4 m_transform;
    aie::OBJMesh* m_mesh;
    aie::ShaderProgram* m_shader;
};
