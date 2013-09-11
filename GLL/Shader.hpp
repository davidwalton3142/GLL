#ifndef GLL_SHADER_HPP
#define GLL_SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm.hpp>

#include "Globals.hpp"

namespace GLL
{
	class ShaderException 
	{ 
	public: 
		ShaderException(const std::string& msg) :msg(msg) {};
		std::string msg;
	};

	class Shader
	{
	public:
		static GLuint load(GLenum type, std::string key, 
			std::vector<std::string> subs = std::vector<std::string>());

		static std::string path;
		static std::string extension;
	private:
		Shader();
		static std::string getSource(const std::string& key);
		static bool isValidShaderType(GLenum type);
		static void substitute(std::string& input, const std::vector<std::string>& subs);
		static void gsub(std::string& input, 
			const std::string& pattern, const std::string& replacement);
	};

	class ShaderProgram
	{
	public:
		ShaderProgram(std::vector<GLuint> shaders);
		~ShaderProgram();

		void use();
		void unuse();

		void setupUBlock(uBlock block);

		void setUniform(const std::string& name, int value);
		void setUniform(const std::string& name, float value);
		void setUniform(const std::string& name, glm::vec2 value);
		void setUniform(const std::string& name, glm::vec3 value);
		void setUniform(const std::string& name, glm::vec4 value);
		void setUniform(const std::string& name, glm::mat3 value, bool normalize = false);
		void setUniform(const std::string& name, glm::mat4 value, bool normalize = false);
	private:
		GLuint id;
		bool usedUBlocks[nUBlocks];
		std::unordered_map<std::string, GLuint> uniforms;
		void getUniformLocation(const std::string& name);
	};
}

#endif
