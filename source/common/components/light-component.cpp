#include "light-component.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{
    LightType LightComponent::getLightType()
    {
        return lightType;
    }

    void LightComponent::setDirectional(glm::vec3 direction)
    {
        lightType = LightType::DIRECTIONAL;
        direction = direction;
        diffuse = {1, 1, 1};
        specular = {1, 1, 1};
        ambient = {26, 26, 26};
    }
    void LightComponent::set_camera_transform(glm::mat4 camera_transform)
    {
        our::camera_transform = camera_transform;
    }
    void LightComponent::seteyeposition(glm::vec3 eye)
    {
        our::eye = eye;
    }
    void LightComponent::Update()
    {
        program->use();

        program->set("light.diffuse", this->diffuse);
        program->set("light.specular", this->specular);
        program->set("light.ambient", this->ambient);

        switch (lightType)
        {
        case LightType::DIRECTIONAL:
            program->set("light.direction", glm::normalize(direction));
            break;
        case LightType::POINT:
            program->set("light.position", position);
            program->set("light.attenuation_constant", attenuation.constant);
            program->set("light.attenuation_linear", attenuation.linear);
            program->set("light.attenuation_quadratic", attenuation.quadratic);
            break;
        case LightType::SPOTLIGHT:
            program->set("light.position", position);
            program->set("light.direction", glm::normalize(direction));
            program->set("light.attenuation_constant", attenuation.constant);
            program->set("light.attenuation_linear", attenuation.linear);
            program->set("light.attenuation_quadratic", attenuation.quadratic);
            program->set("light.inner_angle", spot_angle.inner);
            program->set("light.outer_angle", spot_angle.outer);
            break;
        }
    }
}