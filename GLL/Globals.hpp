#ifndef GLL_GLOBALS_HPP
#define GLL_GLOBALS_HPP

#include <GL/glew.h>

namespace GLL
{
	/** uBlock 
	 *  Uniform block indices used by GLL.
	 */
	enum class uBlock : GLuint { camera = 0 };
	const size_t nUBlocks = 1;
}

#endif
