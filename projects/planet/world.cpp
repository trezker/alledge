#include "world.h"
#include "cubesphere.h"

World::World(shared_ptr<Scenenode> p) {
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
		vertices[i] = CubeToSphere(vertices[i]);
	}

	model = new Static_model;
	model->Set_model_data(vertices, indices);
	model->Show_wireframe(true);
	model->Show_faces(false);

	model_node = new Static_model_node;
	model_node->Set_model(model);
	parent->Attach_node(model_node);
}

void World::Set_detail_center(Vector3 dc) {
	detail_center = dc;
}
