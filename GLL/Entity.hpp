#ifndef GLL_ENTITY_HPP
#define GLL_ENTITY_HPP

#include <glm.hpp>

namespace GLL
{
	/** Entity
	 *  Abstract class for any non-camera object in the world.
	 */
	class Entity
	{
	public:
		glm::mat4 modelToWorld;
		//TODO: translation, rotation, scaling functions.
	};
}

#endif
