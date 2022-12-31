#pragma once
#include <string>
#include <unordered_map>

#include "Matrix.h"

namespace Graphics
{
    struct ShaderProgramSource
    {
        std::string vertexShader;
        std::string fragmentShader;
    };

    class Shader
    {
    private:
        unsigned int rendererID;
        std::unordered_map<std::string, int> uniformCache;
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniform1I(const std::string& name, int val);
        void SetUniform1I_Arr(const std::string& name, int* val, unsigned int count);
        void SetUniform1F(const std::string& name, float val);
        void SetUniform4F(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniform4F(const std::string& name, float* vals);
        void SetUniformMatrix4x4(const std::string& name, const Maths::Matrix3D& mat);
    
    private:
        unsigned int GetUniformLocation(const std::string& name);
        static ShaderProgramSource ParseShader(const std::string& filepath);
        static unsigned CompileShader(const std::string& source, unsigned int type);
        static unsigned CreateShader(const std::string& vtx, const std::string& frg);
    };
}
