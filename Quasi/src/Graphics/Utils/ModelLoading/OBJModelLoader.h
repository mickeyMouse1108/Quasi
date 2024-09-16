#pragma once
#include <vector>

#include "MTLMaterialLoader.h"
#include "OBJModel.h"

#include "Vector.h"

namespace Quasi::Graphics {
    class OBJModelLoader {
    public:
        // https://en.wikipedia.org/wiki/Wavefront_.obj_file
        struct MaterialLib { String dir;  };
        struct UseMaterial { String name; };
        struct Object      { String name; };
        struct Group       { String name; }; // not impl

        struct Vertex       { Math::fVector3 pos; };
        struct VertexTex    { Math::fVector2 tex; };
        struct VertexNormal { Math::fVector3 nrm; };
        struct VertexParam  { Math::fVector3 prm; }; // not impl

        struct Face { int indices[3][3]; };
        struct Line { Vec<int> indices; }; // not impl

        struct SmoothShade { bool enabled; };

        struct OBJProperty : Variant<
            MaterialLib, UseMaterial, Object, Group,
            Vertex, VertexTex, VertexNormal, VertexParam,
            Face, Line,
            SmoothShade> {};
    private:
        Vec<OBJProperty> properties;
        MTLMaterialLoader mats;
        OBJModel model;

        Vec<Math::fVector3> vertex;
        Vec<Math::fVector2> vertexTexture;
        Vec<Math::fVector3> vertexNormal;
        Vec<Face> faces;

        String folder, filename;
    public:
        OBJModelLoader() = default;

        void LoadFile(Str filepath);
        void Load(Str string);
        void LoadMaterialFile(Str filepath);
        void LoadMaterial(Str string);

        void ParseProperty(Str line);
        void ParseProperties(Str string);
        template <class T> static OBJProperty CreateProperty(Str data);

        void CreateModel();
        void CreateObject(Span<const OBJProperty> objprop);
        void ResolveObjectIndices(OBJObject& obj);

        OBJModel& GetModel() { return model; }
        [[nodiscard]] const OBJModel& GetModel() const { return model; }

        OBJModel&& RetrieveModel();

        [[nodiscard]] String DebugStr() const;
    };
}
