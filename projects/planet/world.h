#include <cmath>

typedef std::vector<Vector3> Vectors;
typedef std::vector<int> Indexes;

class World {
private:
	shared_ptr<Scenenode> parent;
	shared_ptr<Static_model> model;
	shared_ptr<Static_model_node> model_node;
public:
	World(shared_ptr<Scenenode> p) {
		parent = p;
		int n = 10;
		float ns  = 2.0f/n;
		int i = 0;

		Vectors vertices;
		Indexes indices;
		for(int x = 0; x<n; ++x) {
			for(int y = 0; y < n; ++y) {
				vertices.push_back(Vector3(x*ns-1,    y*ns-1,    -1));
				vertices.push_back(Vector3(x*ns+ns-1, y*ns-1,    -1));
				vertices.push_back(Vector3(x*ns-1,    y*ns+ns-1, -1));
				vertices.push_back(Vector3(x*ns+ns-1, y*ns+ns-1, -1));
				indices.insert(indices.end(), {i, i+2, i+1, i+1, i+2, i+3});
				i+=4;

				vertices.push_back(Vector3(x*ns-1,    y*ns-1,    1));
				vertices.push_back(Vector3(x*ns+ns-1, y*ns-1,    1));
				vertices.push_back(Vector3(x*ns-1,    y*ns+ns-1, 1));
				vertices.push_back(Vector3(x*ns+ns-1, y*ns+ns-1, 1));
				indices.insert(indices.end(), {i, i+1, i+2, i+1, i+3, i+2});
				i+=4;
			}
			
			for(int z = 0; z < n; ++z) {
				vertices.push_back(Vector3(x*ns-1,    1, z*ns-1));
				vertices.push_back(Vector3(x*ns+ns-1, 1, z*ns-1));
				vertices.push_back(Vector3(x*ns-1,    1, z*ns+ns-1));
				vertices.push_back(Vector3(x*ns+ns-1, 1, z*ns+ns-1));
				indices.insert(indices.end(), {i, i+2, i+1, i+1, i+2, i+3});
				i+=4;

				vertices.push_back(Vector3(x*ns-1,    -1, z*ns-1));
				vertices.push_back(Vector3(x*ns+ns-1, -1, z*ns-1));
				vertices.push_back(Vector3(x*ns-1,    -1, z*ns+ns-1));
				vertices.push_back(Vector3(x*ns+ns-1, -1, z*ns+ns-1));
				indices.insert(indices.end(), {i, i+1, i+2, i+1, i+3, i+2});
				i+=4;
			}
		}
		for(int y = 0; y < n; ++y) {
			for(int z = 0; z < n; ++z) {
				vertices.push_back(Vector3(-1, y*ns-1,    z*ns-1));
				vertices.push_back(Vector3(-1, y*ns+ns-1, z*ns-1));
				vertices.push_back(Vector3(-1, y*ns-1,    z*ns+ns-1));
				vertices.push_back(Vector3(-1, y*ns+ns-1, z*ns+ns-1));
				indices.insert(indices.end(), {i, i+2, i+1, i+1, i+2, i+3});
				i+=4;

				vertices.push_back(Vector3(1, y*ns-1,    z*ns-1));
				vertices.push_back(Vector3(1, y*ns+ns-1, z*ns-1));
				vertices.push_back(Vector3(1, y*ns-1,    z*ns+ns-1));
				vertices.push_back(Vector3(1, y*ns+ns-1, z*ns+ns-1));
				indices.insert(indices.end(), {i, i+1, i+2, i+1, i+3, i+2});
				i+=4;
			}
		}

		for(int i = 0; i<vertices.size(); ++i) {
			float x2 = vertices[i].x*vertices[i].x;
			float y2 = vertices[i].y*vertices[i].y;
			float z2 = vertices[i].z*vertices[i].z;
			float x = vertices[i].x * sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
			float y = vertices[i].y * sqrt(1 - (z2 + x2) / 2 + (z2 * x2) / 3);
			float z = vertices[i].z * sqrt(1 - (x2 + y2) / 2 + (x2 * y2) / 3);
			vertices[i] = Vector3(x, y, z);
		}

		model = new Static_model;
		model->Set_model_data(vertices, indices);
		model->Show_wireframe(true);

		model_node = new Static_model_node;
		model_node->Set_model(model);
		parent->Attach_node(model_node);
	}
};
