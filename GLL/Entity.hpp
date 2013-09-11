#ifndef GLL_ENTITY_HPP
#define GLL_ENTITY_HPP

#include <glm.hpp>

namespace GLL
{
	class Entity
	{
	public:
		Entity() {};
		~Entity() {};
		glm::mat4 modelToWorld;
	};
}

#endif
