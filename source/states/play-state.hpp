#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/collision.hpp>
#include <systems/respawning.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    our::RespawnSystem respawnSystem;
    bool startgame = false;
    bool Exit = false;
    bool restart = false;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
    }

    void onDraw(double deltaTime) override
    {

        if (startgame && !collisionSystem.win && !collisionSystem.gameover && !respawnSystem.dead)
        { // Here, we just run a bunch of systems to control the world logic
            movementSystem.update(&world, (float)deltaTime);
            collisionSystem.update(&world, (float)deltaTime);
            cameraController.update(&world, (float)deltaTime);
            respawnSystem.update(&world, (float)deltaTime);
            // And finally we use the renderer system to draw the scene
            auto size = getApp()->getFrameBufferSize();
            renderer.render(&world, glm::ivec2(0, 0), size);
        }
    }

    void onDestroy() override
    {
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
    void onImmediateGui() override
    {

        ImGui::Begin("Menu");
        if (collisionSystem.win)
        {
            ImGui::Text("You Won");
            ImGui::Selectable("Restart Game", &restart);
            ImGui::Selectable("Exit Game", &Exit);
        }
        else if (collisionSystem.gameover || respawnSystem.dead)
        {
            ImGui::Text("Game Over You Lost");
            ImGui::Selectable("Restart Game", &restart);
            ImGui::Selectable("Exit Game", &Exit);
        }
        else
        {
            ImGui::Selectable(startgame ? "Pause Game" : "Start Game", &startgame);
            ImGui::Selectable("Restart Game", &restart);

            ImGui::Selectable("Exit Game", &Exit);
        }

        if (Exit)
        {
            respawnSystem.dead = false;
            collisionSystem.win = false;
            collisionSystem.gameover = false;
            startgame = true;
            onDestroy();
        }
        if (restart)
        {
            restart = false;
            respawnSystem.dead = false;
            collisionSystem.win = false;
            collisionSystem.gameover = false;
            startgame = true;
            world.clear();
            onInitialize();
        }

        // ImGui::

        ImGui::End();
    }
};