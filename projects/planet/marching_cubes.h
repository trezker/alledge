#include "../../alledge/Vector3.h"
#include <map>
#include <vector>
#include <functional>
#include "perlin.h"

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

class Marching_cubes {
public:
	typedef std::function<float(Vector3 pos)> Sampler;

	const float RADIUS = 8.0f; // sphere radius
	Vectors vertices;
	Indexes indices;

	typedef std::map<Vector3, int, Vector3_comparator> Cornermap;
	Cornermap cornermap;

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

class PerlinSphere {
private:
	Perlin perlin;
	int radius;
public:
	PerlinSphere(Perlin p, int r) {
		perlin = p;
		//perlin.Seed(seed);
		radius = r;
	}

	float Sample(Vector3 pos) {
		Vector3 pp = pos*8.5;
		float p = perlin.Perlin3(pp.x, pp.y, pp.z);
		if(pos.Length() > radius)
			return -1;

		return p-0.4;
	}
};
