#include "BlockRenderer.h"
#include "BlockBase.h"

namespace Game {
    BlockRenderer::Quad BlockRenderer::BLOCK_FACES[6] = { // VERT / COL / TEXID / CORNER
        Quad { { {  0.5f,  0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 0 }, { {  0.5f,  0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 1 },    // right
               { {  0.5f, -0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 2 }, { {  0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 3 }, },
        Quad { { { -0.5f,  0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 0 }, { { -0.5f,  0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 1 },    // left
               { { -0.5f, -0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 2 }, { { -0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 3 }, },
        Quad { { {  0.5f,  0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 0 }, { {  0.5f,  0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 1 },    // up
               { { -0.5f,  0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 2 }, { { -0.5f,  0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 3 }, },
        Quad { { {  0.5f, -0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 0 }, { {  0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 1 },    // down
               { { -0.5f, -0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 2 }, { { -0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 3 }, },
        Quad { { {  0.5f,  0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 0 }, { {  0.5f, -0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 1 },    // front
               { { -0.5f,  0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 2 }, { { -0.5f, -0.5f,  0.5f }, { 1, 1, 1, 1 }, 0, 3 }, },
        Quad { { {  0.5f,  0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 0 }, { {  0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 1 },    // back
               { { -0.5f,  0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 2 }, { { -0.5f, -0.5f, -0.5f }, { 1, 1, 1, 1 }, 0, 3 }, },
    };

    BlockRenderer::BlockRenderer(BlockBase& parent, unsigned enabledFlags) : enabledFlags(enabledFlags), parentBlock(&parent) {}

    BlockRenderer::BlockRenderer(const BlockRenderer& copy) : enabledFlags(copy.enabledFlags), textureID(copy.textureID), parentBlock(copy.parentBlock) {}
    BlockRenderer::BlockRenderer(BlockRenderer&& copy) noexcept : enabledFlags(copy.enabledFlags), textureID(copy.textureID), parentBlock(copy.parentBlock) {
        copy.parentBlock = nullptr;
    }
    BlockRenderer& BlockRenderer::operator=(const BlockRenderer& copy) noexcept {
        if (this == &copy) return *this;
        enabledFlags = copy.enabledFlags;
        textureID = copy.textureID;
        parentBlock = copy.parentBlock;
        return *this;
    }
    BlockRenderer& BlockRenderer::operator=(BlockRenderer&& copy) noexcept {
        enabledFlags = copy.enabledFlags;
        textureID = copy.textureID;
        parentBlock = copy.parentBlock;
        copy.parentBlock = nullptr;
        return *this;
    }

    const Maths::Vec3Int& BlockRenderer::GetPosition() const { return parentBlock->Position; }

    void BlockRenderer::SetTextureOfMesh(Graphics::QuadMesh<Vertex>& mesh, int textureID) {
        Vertex* subMeshStart = mesh.GetVertices();
        // we know its 4 corners per mesh. probably.
        // decrement first so pre-incr doesnt overflow to index 4.
        // could use post-incr but for optimization purposes
        // (its probably gonna get optim'd anyway but whatever)
        --subMeshStart;
        for (int i = 0; i < 4; ++i)
            (++subMeshStart)->TextureAtlID = (intf)textureID;
    }

    void BlockRenderer::SetTextures(Serialization::BlockTextureStructure texture) {
        using namespace Serialization;
        
        const int all = texture.all.id;
        std::ranges::transform(texture.sides, textureID.begin(),
        [=](const std::optional<TextureAtlasID>& x){ return x.has_value() ? x->id : all; });
    }

    Graphics::MeshObject& BlockRenderer::GetMeshObjectForm() {
        std::vector<Quad*> faces;
        
        for (int i = 0; i < 6; ++i)
            if (enabledFlags & 1 << i) {
                faces.emplace_back(new Graphics::QuadMesh(BLOCK_FACES[i]));
                SetTextureOfMesh(*faces.back(), textureID[i]);
            }

        meshObj = Graphics::MeshObject::Make<Graphics::QuadMesh>(faces.data(), faces.size());
        meshObj.Transform(Maths::Matrix3D::TranslateMat(parentBlock->Position));
        return meshObj;
    }
}
