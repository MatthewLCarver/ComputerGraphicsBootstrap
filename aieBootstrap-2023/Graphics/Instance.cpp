#include "Instance.h"
#include <glm/ext.hpp>
#include "Scene.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "FlyCamera.h"
#include "GraphicsApp.h"


/**
 * \brief Construct a new Instance object
 * \param _transform Set the transform of the instance
 * \param _mesh Set the mesh of the instance
 * \param _shader Set the shader for the instance to use
 */
Instance::Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader):
    m_transform(_transform),
    m_mesh(_mesh),
    m_shader(_shader)
{

}

/**
 * \brief Construct a new Instance object and create the transform
 * \param _position Set the position of the instance
 * \param _eulerAngles Set the rotation of the instance
 * \param _scale Set the scale of the instance
 * \param _mesh Set the mesh of the instance
 * \param _shader Set the shader for the instance to use
 */
Instance::Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader):
    m_mesh(_mesh),
    m_shader(_shader)
{
    m_transform = MakeTransform(_position, _eulerAngles, _scale);
}

/**
 * \brief Draw the instance using the given scene, camera and shader
 * \param _scene The scene to draw the instance in
 */
void Instance::Draw(Scene* _scene)
{    
    // Set the shader pipeline
    m_shader->bind();
    // Bind all relevant uniforms for our shaders
    auto pvm =
        _scene->GetCamera()->GetProjectionMatrix() *
        _scene->GetCamera()->GetViewMatrix() * m_transform;
    
    m_shader->bindUniform("ProjectionViewModel",
                                pvm);
    m_shader->bindUniform("ModelMatrix",
                                m_transform);

    // Bind the camera position
    m_shader->bindUniform("CameraPosition",
                            _scene->GetCamera()->GetPosition());

    int numberOfLights = _scene->GetNumberOfPointLights();
    m_shader->bindUniform("numLights", numberOfLights);
    m_shader->bindUniform("PointLightPositions",
                            numberOfLights, _scene->GetPointLightPositions());
    m_shader->bindUniform("PointLightColors",
                            numberOfLights, _scene->GetPointLightColors());
    
    // Bind the directional light we defined
    m_shader->bindUniform("LightDirection",
                                _scene->GetLight().direction);
    m_shader->bindUniform("LightColor",
                                _scene->GetLight().color);
    m_shader->bindUniform("AmbientColor",
                            _scene->GetAmbientLightColor());

    m_mesh->draw();
}

/**
 * \brief Create a transform matrix from position, rotation and scale
 * \param _position The position of the instance
 * \param _eulerAngles The rotation of the instance
 * \param _scale The scale of the instance
 * \return glm::mat4 of the transform matrix
 */
glm::mat4 Instance::MakeTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::translate(glm::mat4(1), _position) *
        glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1)) *
        glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0)) *
        glm::scale(glm::mat4(1), _scale);
}
