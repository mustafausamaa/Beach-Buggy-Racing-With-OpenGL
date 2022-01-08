#pragma once
#include "../ecs/component.hpp"
#include <glm/vec3.hpp>
#include "../texture/texture2d.hpp"
#include "../texture/sampler.hpp"
#include "../shader/shader.hpp"

#include <glm/mat4x4.hpp>
#include <json/json.hpp>

namespace our
{
    glm::mat4 camera_transform;
    glm::vec3 eye;
    // An enum that defines the type of the lights (directional,point,spot-light)
    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOTLIGHT
    };

    class LightComponent : public Component
    {

    public:
        ShaderProgram *program;

        LightType lightType;
        glm::vec3 diffuse, specular, ambient;
        glm::vec3 position;  // Used for Point and Spot Lights only
        glm::vec3 direction; // Used for Directional and Spot Lights only
        struct
        {
            float constant, linear, quadratic;
        } attenuation; // Used for Point and Spot Lights only
        struct
        {
            float inner, outer;
        } spot_angle; // Used for Spot Lights only

        LightType getLightType();

        void setDirectional(glm::vec3 direction);
        // void setShader(our::ShaderProgram *program)
        // {
        //     this->program = program;
        // }
        void set_camera_transform(glm::mat4 camera_transform);
        void seteyeposition(glm::vec3 eye);
        void Update();
    };
}