#ifndef GLL_MESH_HPP
#define GLL_MESH_HPP

#include <GL/glew.h>

#include "Entity.hpp"

#include <string>

namespace GLL
{
	class ShaderProgram;

	/** MeshException
	 *  Used to report errors during mesh loading.
	 */
	class MeshException
	{
	public:
		std::string msg;
		MeshException(const std::string& msg) :msg(msg) {};
	};

	/** Mesh
	 *  Wraps loading an OpenCTM mesh from file.
	 *  Vertex Attributes:
	 *    - Position: 0
	 *    - Norm: 1
	 *    - Tex coords: 2
	 *  Shaders used to render Mesh objects should specify these
	 *  attribute indices via the `layout(location = x)` syntax.
	 */
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
