#pragma once
#include "../ecs/component.hpp"
#include <glm/vec3.hpp>



namespace our {
    our::ShaderProgram* program;
    glm::mat4 camera_transform;
    glm::vec3 eye;
     // An enum that defines the type of the lights (directional,point,spot-light) 
    enum class LightType{
        DIRECTIONAL,
        POINT,
        SPOTLIGHT
    };

    class LightComponent : public Component {
    public:
        LightType lightType;
        glm::vec3 diffuse, specular, ambient;
        glm::vec3 position; // Used for Point and Spot Lights only
        glm::vec3 direction; // Used for Directional and Spot Lights only
        struct {
            float constant, linear, quadratic;
        } attenuation; // Used for Point and Spot Lights only
        struct {
            float inner, outer;
        } spot_angle; // Used for Spot Lights only


          LightType getLightType()
    {
        return this->lightType;
    }

    void setDirectional( glm::vec3 direction)
    {
        this->lightType=LightType::DIRECTIONAL;
        this->direction=direction;
        this->diffuse = {1,1,1};
        this->specular = {1,1,1};
        this->ambient = {26, 26, 26};
    }
    void setShader(our::ShaderProgram* program)
    {
        this->program=program;
    }
    void set_camera_transform( glm::mat4 camera_transform)
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


        switch(lightType){
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

     };
}