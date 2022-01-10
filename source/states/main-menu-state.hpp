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
class MenuState : public our::State
{

    our::World world;
    bool startgame = false;
    bool Exit = false;
    void onDestroy() override
    {
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        world.clear();
        our::clearAllAssets();
        // our::State::getApp()->~Application();
    }

    void onImmediateGui() override
    {

        ImGui::Begin("Main Menu");
        for (int i = 0; i < 20; i++)
        {
            ImGui::NewLine();
        }

        ImGui::Indent(ImGui::GetWindowWidth() / 2 - 50);
        ImGui::Selectable("Start Game", &startgame);

        // ImGui::Selectable("Exit Game", &Exit);

        if (Exit)
        {
            startgame = false;
            onDestroy();
        }
        if (startgame)
        {
            world.clear();
            our::State::getApp()->changeState("play");
            startgame = false;
        }

        // ImGui::

        ImGui::End();
    }
};