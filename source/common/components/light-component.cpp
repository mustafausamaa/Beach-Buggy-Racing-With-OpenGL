#include "light-component.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        int inttype = data.value("lightType", 0);

        color = data.value("color", color);

        if (inttype == 0)
        {
            lightType = LightType::DIRECTIONAL;
            direction = data.value("direction", glm::vec3(-1, 0, 0)); // Used for Directional and Spot Lights only
        }
        else if (inttype == 1)
        {
            lightType = LightType::POINT;
            attenuation.constant = data.value("attenuation_constant", 0.0f); // Used for Point and Spot Lights only
            attenuation.linear = data.value("attenuation_linear", 0.0f);
            attenuation.quadratic = data.value("attenuation_quadratic", 0.0f);
        }
        else if (inttype == 2)
        {
            lightType = LightType::SPOTLIGHT;
            direction = data.value("direction", direction);                  // Used for Directional and Spot Lights only
            attenuation.constant = data.value("attenuation_constant", 0.0f); // Used for Point and Spot Lights only
            attenuation.linear = data.value("attenuation_linear", 0.0f);
            attenuation.quadratic = data.value("attenuation_quadratic", 0.0f);
            spot_angle.inner = data.value("inner_angle", 0.0f); // Used for Spot Lights only
            spot_angle.outer = data.value("outer_angle", 0.0f);
        }
    }
}