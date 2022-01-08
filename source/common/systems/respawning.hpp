#pragma once

#include "../ecs/world.hpp"
#include "../components/respawning.hpp"
#include "../components/mesh-renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <string>
int count = 0;
namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class RespawnSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            for (auto groundEntity : world->getEntities())
            {
                if (groundEntity->name == "ground")
                {

                    for (auto MotoEntity : world->getEntities())
                    {
                        RespawnComponent *Respawn = MotoEntity->getComponent<RespawnComponent>();
                        if (Respawn)
                        {
                            if (MotoEntity->name == "MyMoto")
                            {
                                if (groundEntity->localTransform.position.x + 7 < MotoEntity->parent->localTransform.position.x)
                                {

                                    for (auto MotoEntity1 : world->getEntities())
                                    {

                                        if (MotoEntity1->name == "healthbar1")
                                        {

                                            MeshRendererComponent *healthBar = MotoEntity1->getComponent<MeshRendererComponent>();
                                            if (healthBar)
                                            {
                                                if (count == 0)
                                                {
                                                    MotoEntity1->deleteComponent<MeshRendererComponent>();
                                                    count++;
                                                    break;
                                                }
                                            }
                                        }

                                        else if (MotoEntity1->name == "healthbar2")
                                        {

                                            MeshRendererComponent *healthBar = MotoEntity1->getComponent<MeshRendererComponent>();
                                            if (healthBar)
                                            {
                                                if (count == 1)
                                                {
                                                    MotoEntity1->deleteComponent<MeshRendererComponent>();
                                                    count++;
                                                    break;
                                                }
                                            }
                                        }

                                        else if (MotoEntity1->name == "healthbar3")
                                        {

                                            MeshRendererComponent *healthBar = MotoEntity1->getComponent<MeshRendererComponent>();
                                            if (healthBar)
                                            {
                                                if (count == 2)
                                                {
                                                    MotoEntity1->deleteComponent<MeshRendererComponent>();
                                                    count++;
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                    MotoEntity->parent->localTransform.position.z += 10;
                                    MotoEntity->parent->localTransform.position.x += -7.5;
                                }
                                else if (groundEntity->localTransform.position.x + 7 < -1 * MotoEntity->parent->localTransform.position.x)
                                {
                                    for (auto MotoEntity1 : world->getEntities())
                                    {

                                        if (MotoEntity1->name == "healthbar1")
                                        {

                                            MeshRendererComponent *healthBar = MotoEntity1->getComponent<MeshRendererComponent>();
                                            if (healthBar)
                                            {
                                                if (count == 0)
                                                {
                                                    MotoEntity1->deleteComponent<MeshRendererComponent>();
                                                    count++;
                                                    break;
                                                }
                                            }
                                        }

                                        else if (MotoEntity1->name == "healthbar2")
                                        {

                                            MeshRendererComponent *healthBar = MotoEntity1->getComponent<MeshRendererComponent>();
                                            if (healthBar)
                                            {
                                                if (count == 1)
                                                {
                                                    MotoEntity1->deleteComponent<MeshRendererComponent>();
                                                    count++;
                                                    break;
                                                }
                                            }
                                        }

                                        else if (MotoEntity1->name == "healthbar3")
                                        {

                                            MeshRendererComponent *healthBar = MotoEntity1->getComponent<MeshRendererComponent>();
                                            if (healthBar)
                                            {
                                                if (count == 2)
                                                {
                                                    MotoEntity1->deleteComponent<MeshRendererComponent>();
                                                    count++;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    MotoEntity->parent->localTransform.position.z += 10;
                                    MotoEntity->parent->localTransform.position.x += 7.5;
                                }
                            }
                            if (MotoEntity->name == "EnemyMoto")
                            {
                                if (groundEntity->localTransform.position.x + 7 < MotoEntity->localTransform.position.x)
                                {
                                    MotoEntity->localTransform.position.z += 10;
                                    MotoEntity->localTransform.position.x += -7.5;
                                }
                                else if (groundEntity->localTransform.position.x + 7 < -1 * MotoEntity->localTransform.position.x)
                                {
                                    MotoEntity->localTransform.position.z += 10;
                                    MotoEntity->localTransform.position.x += 7.5;
                                }
                            }
                        }
                    }
                }
            }
        }
    };
}
