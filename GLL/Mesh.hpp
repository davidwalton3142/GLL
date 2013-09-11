#ifndef GLL_MESH_HPP
#define GLL_MESH_HPP

#include <string>
#include <GL/glew.h>

#include "Entity.hpp"

namespace GLL
{
	class ShaderProgram;

	class MeshException
	{
	public:
		std::string msg;
		MeshException(const std::string& msg) :msg(msg) {};
	};

	class Mesh : public Entity
	{
	public:
		Mesh(const std::string& filename);
		~Mesh();
		void render(ShaderProgram& shader);
	private:
		GLuint vao;
		GLuint v_vbo;
		GLuint n_vbo;
		GLuint t_vbo;
		GLuint e_ebo;
		GLsizei elems;
	};
}
#endif
