#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/light-component.hpp"
#include <glad/gl.h>
#include <vector>
#include <algorithm>

namespace our
{

    // The render command stores command that tells the renderer that it should draw
    // the given mesh at the given localToWorld matrix using the given material
    // The renderer will fill this struct using the mesh renderer components
    struct RenderCommand
    {
        glm::mat4 localToWorld;
        glm::vec3 center;
        Mesh *mesh;
        Material *material;
    };

    // A forward renderer is a renderer that draw the object final color directly to the framebuffer
    // In other words, the fragment shader in the material should output the color that we should see on the screen
    // This is different from more complex renderers that could draw intermediate data to a framebuffer before computing the final color
    // In this project, we only need to implement a forward renderer
    class ForwardRenderer
    {
        // These are two vectors in which we will store the opaque and the transparent commands.
        // We define them here (instead of being local to the "render" function) as an optimization to prevent reallocating them every frame
        std::vector<RenderCommand> opaqueCommands;
        std::vector<RenderCommand> transparentCommands;
        std::vector<LightComponent *> lights;

    public:
        // This function should be called every frame to draw the given world
        // Both viewportStart and viewportSize are using to define the area on the screen where we will draw the scene
        // viewportStart is the lower left corner of the viewport (in pixels)
        // viewportSize is the width & height of the viewport (in pixels). It is also used to compute the aspect ratio
        void render(World *world, glm::ivec2 viewportStart, glm::ivec2 viewportSize)
        {
            // First of all, we search for a camera and for all the mesh renderers
            CameraComponent *camera = nullptr;
            opaqueCommands.clear();
            transparentCommands.clear();
            lights.clear(); // clear light vector
            for (auto entity : world->getEntities())
            {

                // If we hadn't found a camera yet, we look for a camera in this entity
                if (!camera)
                    camera = entity->getComponent<CameraComponent>();

                // TODO check if the entity has light component
                if (auto lightRenderer = entity->getComponent<LightComponent>(); lightRenderer)
                {
                    lights.push_back(lightRenderer);
                }
                // If this entity has a mesh renderer component
                if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
                {
                    // We construct a command from it
                    RenderCommand command;
                    command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                    command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                    command.mesh = meshRenderer->mesh;
                    command.material = meshRenderer->material;
                    // if it is transparent, we add it to the transparent commands list
                    if (command.material->transparent)
                    {
                        transparentCommands.push_back(command);
                    }
                    else
                    {
                        // Otherwise, we add it to the opaque command list
                        opaqueCommands.push_back(command);
                    }
                }
            }

            // If there is no camera, we return (we cannot render without a camera)
            if (camera == nullptr)
                return;

            // TODO: Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
            //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
            glm::vec3 cameraForward = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
            std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                      {
                // TODO: Finish this function
                //  HINT: the following return should return true "first" should be drawn before "second".
                return first.center.z * cameraForward.z < second.center.z * cameraForward.z; });

            // TODO: Get the camera ViewProjection matrix and store it in VP
            glm::mat4 VP = camera->getProjectionMatrix(viewportSize) * camera->getViewMatrix();
            // TODO: Set the OpenGL viewport using viewportStart and viewportSize
            glViewport(viewportStart.x, viewportStart.y, viewportSize.x, viewportSize.y);
            // TODO: Set the clear color to black and the clear depth to 1
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            // TODO: Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
            glColorMask(true, true, true, true);
            glDepthMask(true);
            // TODO: Clear the color and depth buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // TODO: Draw all the opaque commands followed by all the transparent commands
            //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
            for (auto command : opaqueCommands)
            {
                command.material->setup();

                command.material->shader->set("transform", VP * command.localToWorld);
                // check if the entity has litMaterial
                if (dynamic_cast<LitMaterial *>(command.material))
                {
                    // Set the matrix and matrix inverse transpose, VP, Eye  in vertix shader
                    // set light count in fragment shaer of light
                    command.material->shader->set("transform", command.localToWorld);
                    command.material->shader->set("M_IT", glm::transpose(glm::inverse(command.localToWorld)));
                    command.material->shader->set("VP", VP);
                    command.material->shader->set("eye", glm::vec3(camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1)));
                    command.material->shader->set("light_count", (int)lights.size());
                    const int MAX_LIGHTS = 8;
                    int lightcounter = 0;
                    for (const auto &light : lights)
                    {
                        // Read Light type and color from Light component
                        std::string prefix = "lights[" + std::to_string(lightcounter) + "].";
                        command.material->shader->set(prefix + "type", (int)light->lightType);
                        command.material->shader->set(prefix + "color", glm::normalize(light->color));

                        switch (light->lightType)
                        {
                        case LightType::DIRECTIONAL:
                            // Load direction of light from light component from json file
                            command.material->shader->set(prefix + "direction", glm::normalize(light->direction)); // Used for Directional and Spot Lights only
                            break;
                        case LightType::POINT:
                            // get postition of the parent
                            glm::vec3 pointlightposition = glm::vec3(light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->getOwner()->localTransform.position, 1));
                            command.material->shader->set(prefix + "position", pointlightposition);                      // Used for Point and Spot Lights only
                            command.material->shader->set(prefix + "attenuation_constant", light->attenuation.constant); // Used for Point and Spot Lights only
                            command.material->shader->set(prefix + "attenuation_linear", light->attenuation.linear);
                            command.material->shader->set(prefix + "attenuation_quadratic", light->attenuation.quadratic);
                            break;
                        case LightType::SPOTLIGHT:
                            // get postition of the parent
                            glm::vec3 spotlightposition = glm::vec3(light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->getOwner()->localTransform.position, 1));
                            command.material->shader->set(prefix + "position", spotlightposition);                       // Used for Point and Spot Lights only
                            command.material->shader->set(prefix + "direction", glm::normalize(light->direction));       // Used for Directional and Spot Lights only
                            command.material->shader->set(prefix + "attenuation_constant", light->attenuation.constant); // Used for Point and Spot Lights only
                            command.material->shader->set(prefix + "attenuation_linear", light->attenuation.linear);
                            command.material->shader->set(prefix + "attenuation_quadratic", light->attenuation.quadratic);
                            command.material->shader->set(prefix + "inner_angle", light->spot_angle.inner);
                            command.material->shader->set(prefix + "outer_angle", light->spot_angle.outer);
                            break;
                        }
                        lightcounter++;
                        if (lightcounter >= MAX_LIGHTS)
                            break;
                    }
                }

                command.mesh->draw();
            }
            for (auto command : transparentCommands)
            {
                command.material->setup();
                command.material->shader->set("transform", VP * command.localToWorld);

                command.mesh->draw();
            }
        };
    };

}