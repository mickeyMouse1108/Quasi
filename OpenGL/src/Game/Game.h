#pragma once

#include "GraphicsDevice.h"
#include "World.h"

namespace Game
{
    class Game
    {
    private:
        World world;

        Maths::Vector3 rot;
        Maths::Vector3 scale = {40, 40, 40};
        Maths::Vector3 trans;
        // Graphics::MeshObject cube;
        // Graphics::TriMesh<Graphics::MeshUtils::Vertex> vtx;
    public:
        std::unique_ptr<Graphics::GraphicsDevice> GraphicsDevice;
        
        Game();
        ~Game();

        void Update();
        void Render();
        bool IsWindowOpen() { return GraphicsDevice->WindowIsOpen(); }
    };
}
