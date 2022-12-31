#include "Shader.h"

#include <fstream>

#include "Debugging.h"

namespace Graphics
{
    Shader::Shader(const std::string& filepath) : rendererID(0)
    {
        ShaderProgramSource shadersrc = ParseShader(filepath);
        rendererID = CreateShader(shadersrc.vertexShader, shadersrc.fragmentShader);
    }

    Shader::~Shader()
    {
        GLCALL(glDeleteProgram(rendererID));
    }

    void Shader::Bind() const
    {
        GLCALL(glUseProgram(rendererID));
    }

    void Shader::Unbind() const
    {
        GLCALL(glUseProgram(0));
    }

    void Shader::SetUniform1I(const std::string& name, int val)
    {
        GLCALL(glUniform1i(GetUniformLocation(name), val));
    }

    void Shader::SetUniform1I_Arr(const std::string& name, int* val, unsigned count)
    {
        GLCALL(glUniform1iv(GetUniformLocation(name), count, val));
    }

    void Shader::SetUniform1F(const std::string& name, float val)
    {
        GLCALL(glUniform1f(GetUniformLocation(name), val));
    }

    void Shader::SetUniform4F(const std::string& name, float v0, float v1, float v2, float v3)
    {
        GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::SetUniform4F(const std::string& name, float* vals)
    {
        GLCALL(glUniform4f(GetUniformLocation(name), vals[0], vals[1], vals[2], vals[3]));
    }

    void Shader::SetUniformMatrix4x4(const std::string& name, const Maths::Matrix3D& mat)
    {
        GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, mat.GetInCol()));
    }

    unsigned Shader::GetUniformLocation(const std::string& name)
    {
        if (uniformCache.find(name) != uniformCache.end())
            return uniformCache[name];
    
        GLCALL(int location = glGetUniformLocation(rendererID, name.c_str()));
        ASSERT(location != -1);
        uniformCache[name] = location;
        return location;
    }


    ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
        std::ifstream stream(filepath);

        enum class ShaderType {
            NONE = -1, VERTEX, FRAGMENT 
        };

        ShaderType type = ShaderType::NONE;
        std::string line;
        std::stringstream ss[2];
        while (getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            }
            else {
                ss[(int)type] << line << "\n";
            }
        }

        return ShaderProgramSource{ ss[0].str(), ss[1].str() };
    }

    unsigned int Shader::CompileShader(const std::string& source, unsigned int type) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
        if (result == GL_FALSE) {
            int len;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            char* msg = (char*)_malloca(len * sizeof(char));
            glGetShaderInfoLog(id, len, &len, msg);
            DEBUGFUN(std::cout << "ERR: ERR WHEN COMPILING " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader." << msg << '\n');

            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShader(const std::string& vtx, const std::string& frg) {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(vtx, GL_VERTEX_SHADER);
        unsigned int fs = CompileShader(frg, GL_FRAGMENT_SHADER);

        GLCALL(glAttachShader(program, vs));
        GLCALL(glAttachShader(program, fs));
        GLCALL(glLinkProgram(program));
        GLCALL(glValidateProgram(program));
        
        GLCALL(glDeleteShader(vs));
        GLCALL(glDeleteShader(fs));

        return program;
    }
}