#ifndef Example_MultiLine_h
#define Example_MultiLine_h

#include "DrawableShape.h"

namespace cgCourse
{
	class MultiLine : public DrawableShape
	{
	public:
		MultiLine(const std::vector< glm::vec3 >& _vertices,
                  const std::vector< glm::vec3 >& _normals,
                  const std::vector< glm::vec3 >& _tangents);
        
	};
}

#endif
