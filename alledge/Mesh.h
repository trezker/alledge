#ifndef Mesh_h
#define Mesh_h

#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include "md5model.h"
#include "Vector3.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	void Set_up_buffers(struct md5_mesh_t *mesh, md5_joint_t *baseSkel);
	void Prepare_frame(const struct md5_joint_t *skeleton);
	void Render() const;

	Vector3 Get_low_corner();
	Vector3 Get_high_corner();
private:
	md5_mesh_t *mesh;

	float *texCoords;
	vec3_t *normals;
	vec3_t *vertexArray;
	GLuint *vertexIndices;
	
	Vector3 low_corner;
	Vector3 high_corner;
};

#endif
