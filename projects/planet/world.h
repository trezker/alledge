#include <cmath>
#include <vector>
#include "../../alledge/Vector3.h"
#include "../../alledge/Scenenode.h"
#include "../../alledge/Static_model_node.h"

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

class World {
private:
	shared_ptr<Scenenode> parent;
	shared_ptr<Static_model> model;
	shared_ptr<Static_model_node> model_node;
	Vector3 detail_center;
public:
	World(shared_ptr<Scenenode> p);
	void Set_detail_center(Vector3 dc);
};
