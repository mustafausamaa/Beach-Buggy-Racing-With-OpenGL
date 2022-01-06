#include "light-component.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 


namespace our {
      LightType getLightType()
    {
        return this->type;
    }

    void setDirectional( glm::vec3 direction)
    {
        this->type=LightType::DIRECTIONAL;
        this->direction=direction;
        this->diffuse = {1,1,1};
        this->specular = {1,1,1};
        this->ambient = {26, 26, 26};
    }
    void setShader(our::ShaderProgram* program) override
    {
        this->program=program;
    }
    void set_camera_transform( glm::mat4 camera_transform) override
    {
        this->camera_transform=camera_transform;
    }
    void seteyeposition(glm::vec3 eye) override
    {
        this->eye=eye;
    }
    void Update() override
    {
        glUseProgram(*program);

        program->set("light.diffuse", this->diffuse);
        program->set("light.specular", this->specular);
        program->set("light.ambient", this->ambient);


        switch(lighType){
            case LightType::DIRECTIONAL:
                program->set("light.direction", glm::normalize(direction));
                break;
            case LightType::POINT:
                program->set("light.position", position);
                program->set("light.attenuation_constant", attenuation.constant);
                program->set("light.attenuation_linear", attenuation.linear);
                program->set("light.attenuation_quadratic", attenuation.quadratic);
                break;
            case LightType::SPOT:
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