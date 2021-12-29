#include <cmath>
#include "world.h"
#include "cubesphere.h"

class Chunk {
private:
	Vector3 corners[4];
	Vector3 center;
	shared_ptr<Scenenode> parent;
	shared_ptr<Static_model> model;
	shared_ptr<Static_model_node> model_node;
	bool is_attached;
public:
	Chunk(Vector3 oc, shared_ptr<Scenenode> p) {
		int n = 10;
		float ns  = 2.0f/n;
		parent = p;

		Vectors vertices;
		Indexes indices;

		//Interpolate
/*		Vector3 ustep;
		Vector3 vstep;

		for(int u = 0; u < 10; u += 1) {
			for(int v = 0; v < 10; v += 1) {
				Vector3 c = corners[0] + (ustep * u) + (vstep*v);
				c = CubeToSphere(c);
				//Apply perlin value
				vertices.push_back(c);
			}
		}
*/
		//Build faces
		if(abs(oc.x)>1) {
			oc.x /= 2;
			vertices.push_back(Vector3(oc.x, oc.y, oc.z));
			vertices.push_back(Vector3(oc.x, oc.y+ns, oc.z));
			vertices.push_back(Vector3(oc.x, oc.y, oc.z+ns));
			vertices.push_back(Vector3(oc.x, oc.y+ns, oc.z+ns));
			if(oc.x > 0) 
				indices.insert(indices.end(), {0, 1, 2, 1, 3, 2});
			else
				indices.insert(indices.end(), {0, 2, 1, 1, 2, 3});
		}
		else if(abs(oc.y)>1) {
			oc.y /= 2;
			vertices.push_back(Vector3(oc.x, oc.y, oc.z));
			vertices.push_back(Vector3(oc.x+ns, oc.y, oc.z));
			vertices.push_back(Vector3(oc.x, oc.y, oc.z+ns));
			vertices.push_back(Vector3(oc.x+ns, oc.y, oc.z+ns));
			if(oc.y > 0) 
				indices.insert(indices.end(), {0, 2, 1, 1, 2, 3});
			else
				indices.insert(indices.end(), {0, 1, 2, 1, 3, 2});
		}
		else {
			oc.z /= 2;
			vertices.push_back(Vector3(oc.x, oc.y, oc.z));
			vertices.push_back(Vector3(oc.x+ns, oc.y, oc.z));
			vertices.push_back(Vector3(oc.x, oc.y+ns, oc.z));
			vertices.push_back(Vector3(oc.x+ns, oc.y+ns, oc.z));
			if(oc.z > 0) 
				indices.insert(indices.end(), {0, 1, 2, 1, 3, 2});
			else
				indices.insert(indices.end(), {0, 2, 1, 1, 2, 3});
		}

		for(int i = 0; i<4; ++i) {
			corners[i] = vertices[i];
			center += corners[i];
		}
		center /= 4;

		for(int i = 0; i<vertices.size(); ++i) {
			vertices[i] = CubeToSphere(vertices[i]);
		}

		model = new Static_model;
		model->Set_model_data(vertices, indices);
		model->Show_normals(true);

		model_node = new Static_model_node;
		model_node->Set_model(model);
		parent->Attach_node(model_node);
		is_attached = true;
	}

	void Adjust(Vector3 detail_center) {
		if(is_attached && (detail_center - center).Length() > 0.4) {
			parent->Detach_node(model_node);
			is_attached = false;
		}
		if(!is_attached && (detail_center - center).Length() <= 0.4) {
			parent->Attach_node(model_node);
			is_attached = true;
		}
	}
};

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
	int n = 10;
	float ns  = 2.0f/n;

	detail_center = dc.GetNormalized();
	Vector3 oc = SphereToCube(detail_center, 2.0) + Vector3(1, 1, 1);
	if(std::isnan(oc.x) || std::isnan(oc.y) || std::isnan(oc.z))
		return;
	oc *= n/2;
	oc.x = floor(oc.x);
	oc.y = floor(oc.y);
	oc.z = floor(oc.z);
	
	Vector3i chunkvector(oc);
	Chunk* c = chunks[chunkvector];
	if(c == NULL) {
		std::cout<<oc.x<<" "<<oc.y<<" "<<oc.y<<std::endl;
	}

	oc /= (n/2);
	oc -= Vector3(1, 1, 1);

	if(c == NULL) {
		c = new Chunk(oc, parent);
		chunks[chunkvector] = c;
	}

	for (Chunks::iterator it = chunks.begin(); it != chunks.end(); it++) {
		if(it->second != NULL)
			it->second->Adjust(SphereToCube(detail_center));
	}
}
