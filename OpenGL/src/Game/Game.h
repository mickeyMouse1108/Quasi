#pragma once
#include "BlockMesh.h"
#include "Graphics/Graphicals/GraphicsDevice.h"
#include "MeshObject.h"

namespace Game
{
    class Game
    {
    private:
        BlockMesh block;
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
