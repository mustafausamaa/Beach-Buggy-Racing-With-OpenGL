#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/movement.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
namespace our
{

    // The Collision system is responsible for moving every entity which contains a CollisionComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/Collision.hpp"
    class CollisionSystem
    {
    public:
        bool gameover = false;
        bool win = false;

        // This should be called every frame to update all entities containing a CollisionComponent.
        void update(World *world, float deltaTime)
        {
            // std::cout<<"collision deteteed"<<std::endl;
            //  For each entity in the world

            for (auto collidingEntity : world->getEntities())
            {
                // Get the First Collision component which is our main motorcycle
                CollisionComponent *CollisionMain = collidingEntity->getComponent<CollisionComponent>();

                // If the Collision component exists
                if (CollisionMain)
                {

                    glm::vec4 center1 = collidingEntity->getLocalToWorldMatrix() * CollisionMain->center;
                    float r1 = CollisionMain->radius;

                    for (auto collideWithEntity : world->getEntities())
                    {
                        // get any colliding entity like barriers or other motorcycles
                        CollisionComponent *CollisionSecond = collideWithEntity->getComponent<CollisionComponent>();
                        if (CollisionSecond)
                        {
                            if (collidingEntity != collideWithEntity)
                            {
                                glm::vec4 center2 = collideWithEntity->getLocalToWorldMatrix() * CollisionSecond->center;
                                float r2 = CollisionSecond->radius;
                                // distance is the distance between colliding motorcycle and any other colliding entities
                                float distance = glm::distance(center1, center2);
                                if (distance <= r1 + r2) // then overlapping happened between the 2 entities
                                {
                                    // enter this for loop if colliding entitiy is our main motorcycle

                                    if (collidingEntity->name == "MyMoto" && collideWithEntity->name == "EnemyMoto")
                                    {

                                        our::Entity *CameraControllerEntity = collidingEntity->parent;
                                        // if the camera controlling our motorcycle is in the right side of the collision then collide and go right and collidingwith object go left
                                        if (CameraControllerEntity->localTransform.position.x > collideWithEntity->localTransform.position.x)

                                        {
                                            CameraControllerEntity->localTransform.position.x++;
                                            collideWithEntity->localTransform.position.x--;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (CameraControllerEntity->localTransform.position.x < collideWithEntity->localTransform.position.x)
                                        {
                                            CameraControllerEntity->localTransform.position.x--;
                                            collideWithEntity->localTransform.position.x++;
                                        }
                                    }
                                    // if 2 Enemy Motorcycles collide with each others
                                    // then check which is right and which is left to make the collision left and right in logical view
                                    else if (collidingEntity->name == "EnemyMoto" && collideWithEntity->name == "EnemyMoto")
                                    {

                                        if (collidingEntity->localTransform.position.x >= collideWithEntity->localTransform.position.x)

                                        {
                                            collidingEntity->localTransform.position.x++;
                                            collideWithEntity->localTransform.position.x--;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (collidingEntity->localTransform.position.x < collideWithEntity->localTransform.position.x)
                                        {
                                            collidingEntity->localTransform.position.x--;
                                            collideWithEntity->localTransform.position.x++;
                                        }
                                    }
                                    // if  MyMoto  collide with the Concrete barriers
                                    // then check which is right and which is left to make the collision left and right in logical view and decrease the moto speed
                                    else if (collidingEntity->name == "MyMoto" && collideWithEntity->name == "ConcreteBarrier")
                                    {
                                        our::Entity *CameraControllerEntity = collidingEntity->parent;

                                        if (CameraControllerEntity->localTransform.position.x >= collideWithEntity->localTransform.position.x)

                                        {
                                            CameraControllerEntity->localTransform.position.x++;
                                            MovementComponent *speed = collidingEntity->parent->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (CameraControllerEntity->localTransform.position.x < collideWithEntity->localTransform.position.x)
                                        {
                                            CameraControllerEntity->localTransform.position.x--;
                                            MovementComponent *speed = collidingEntity->parent->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                    }
                                    // if  Enemy  collide with the Concrete barriers
                                    // then check which is right and which is left to make the collision left and right in logical view and decrease the moto speed
                                    else if (collidingEntity->name == "EnemyMoto" && collideWithEntity->name == "ConcreteBarrier")
                                    {

                                        if (collidingEntity->localTransform.position.x >= collideWithEntity->localTransform.position.x)

                                        {
                                            collidingEntity->localTransform.position.x++;
                                            MovementComponent *speed = collidingEntity->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (collidingEntity->localTransform.position.x < collideWithEntity->localTransform.position.x)
                                        {
                                            collidingEntity->localTransform.position.x--;
                                            MovementComponent *speed = collidingEntity->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                    }
                                    // if  MyMoto  collide with the red barriers
                                    // then check which is right and which is left to make the collision left and right in logical view and decrease the moto speed
                                    else if (collidingEntity->name == "MyMoto" && collideWithEntity->name == "RedBarrier")
                                    {
                                        our::Entity *CameraControllerEntity = collidingEntity->parent;

                                        if (CameraControllerEntity->localTransform.position.z >= collideWithEntity->localTransform.position.z)

                                        {
                                            CameraControllerEntity->localTransform.position.z += 2;
                                            MovementComponent *speed = collidingEntity->parent->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (CameraControllerEntity->localTransform.position.z < collideWithEntity->localTransform.position.z)
                                        {
                                            CameraControllerEntity->localTransform.position.z += 2;
                                            MovementComponent *speed = collidingEntity->parent->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                    }
                                    // if  Enemy  collide with the red barriers
                                    // then check which is right and which is left to make the collision left and right in logical view and decrease the moto speed
                                    else if (collidingEntity->name == "EnemyMoto" && collideWithEntity->name == "RedBarrier")
                                    {

                                        if (collidingEntity->localTransform.position.z >= collideWithEntity->localTransform.position.z)

                                        {
                                            collidingEntity->localTransform.position.z += 2;
                                            MovementComponent *speed = collidingEntity->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (collidingEntity->localTransform.position.z < collideWithEntity->localTransform.position.z)
                                        {
                                            collidingEntity->localTransform.position.z += -2;
                                            MovementComponent *speed = collidingEntity->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += deltaTime;
                                        }
                                    }
                                    else if (collidingEntity->name == "MyMoto" && collideWithEntity->name == "Rocket")
                                    {
                                        our::Entity *CameraControllerEntity = collidingEntity->parent;

                                        if (CameraControllerEntity->localTransform.position.z >= collideWithEntity->localTransform.position.z)

                                        {

                                            MovementComponent *speed = collidingEntity->parent->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += -0.5; // speed increase by 1 in -z direction
                                            MeshRendererComponent *rocket = collideWithEntity->getComponent<MeshRendererComponent>();

                                            if (rocket)
                                            {

                                                collideWithEntity->deleteComponent<MeshRendererComponent>();
                                            }
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (CameraControllerEntity->localTransform.position.z < collideWithEntity->localTransform.position.z)
                                        {
                                            MovementComponent *speed = collidingEntity->parent->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += -0.5; // speed increase by 1 in -z direction
                                            collideWithEntity->deleteComponent<MeshRendererComponent>();
                                            win = true;
                                        }
                                    }
                                    // if  Enemy  collide with the red barriers
                                    // then check which is right and which is left to make the collision left and right in logical view and decrease the moto speed
                                    else if (collidingEntity->name == "EnemyMoto" && collideWithEntity->name == "Rocket")
                                    {

                                        if (collidingEntity->localTransform.position.z >= collideWithEntity->localTransform.position.z)

                                        {
                                            MovementComponent *speed = collidingEntity->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += -0.5; // speed increase by 1 in -z direction
                                            collideWithEntity->deleteComponent<MeshRendererComponent>();
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (collidingEntity->localTransform.position.z < collideWithEntity->localTransform.position.z)
                                        {
                                            MovementComponent *speed = collidingEntity->getComponent<MovementComponent>();
                                            speed->linearVelocity.z += -0.5; // speed increase by 1 in -z direction
                                            collideWithEntity->deleteComponent<MeshRendererComponent>();
                                        }
                                    }
                                    else if (collidingEntity->name == "EnemyMoto" && collideWithEntity->name == "endline")
                                    {

                                        if (collidingEntity->localTransform.position.z >= collideWithEntity->localTransform.position.z)

                                        {
                                            gameover = true;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (collidingEntity->localTransform.position.z < collideWithEntity->localTransform.position.z)
                                        {
                                            gameover = true;
                                        }
                                    }
                                    else if (collidingEntity->name == "MyMoto" && collideWithEntity->name == "endline")
                                    {

                                        if (collidingEntity->localTransform.position.z >= collideWithEntity->localTransform.position.z)

                                        {
                                            win = true;
                                        }
                                        // if the camera controlling our motorcycle is in the left side of the collision then collide and go left and collidingwith object go right
                                        else if (collidingEntity->localTransform.position.z < collideWithEntity->localTransform.position.z)
                                        {
                                            win = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };

}
