#include "../../alledge/Vector3.h"
#include <vector>

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

class MCubesRef {
public:
	const float RADIUS = 8.0f; // sphere radius
	Vectors vertices;
	Indexes indices;

	// scalar value at given point
	// replace with sampling from voxel data or whetever you'd like to
	float SampleValue(Vector3 pos);
	void Clear();
	void MarchCube(Vector3 minCornerPos);
};