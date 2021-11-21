#include "../../alledge/Vector3.h"
#include <vector>
#include <functional>

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

class Marching_cubes {
public:
	typedef std::function<float(Vector3 pos)> Sampler;

	const float RADIUS = 8.0f; // sphere radius
	Vectors vertices;
	Indexes indices;

	Sampler sampler;

	void Set_sampler(Sampler sampler);
	void Clear();
	void MarchCube(Vector3 minCornerPos);

private:
	float SampleValue(Vector3 pos);
};

class SphereSampler {
public:
	float radius = 8.0f;
	
	SphereSampler(float r) {
		radius = r;
	}

	float Sample(Vector3 pos) {
		return pos.Length() - radius;
	}
};
