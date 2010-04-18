#include "../alledge/Mesh.h"
#include <cmath>

void vector3_CrossProduct(vec3_t in1, vec3_t in2, vec3_t *out)
{
	(*out)[0] = in1[1] * in2[2] - in1[2] * in2[1];
	(*out)[1] = in1[2] * in2[0] - in1[0] * in2[2];
	(*out)[2] = in1[0] * in2[1] - in1[1] * in2[0];
}

Mesh::Mesh()
:texCoords(NULL)
,normals(NULL)
,vertexArray(NULL)
,vertexIndices(NULL)
{
}

Mesh::~Mesh()
{
	if (vertexArray)
	{
		free (vertexArray);
		vertexArray = NULL;
	}

	if (vertexIndices)
	{
		free (vertexIndices);
		vertexIndices = NULL;
	}
	if (texCoords)
	{
		free (texCoords);
		texCoords = NULL;
	}
	if (normals)
	{
		free (normals);
		normals = NULL;
	}
}

void Mesh::Set_up_buffers(struct md5_mesh_t *imesh, md5_joint_t *skeleton)
{
	mesh = imesh;
	vertexArray = (vec3_t *)malloc (sizeof (vec3_t) * mesh->num_verts);
	vertexIndices = (GLuint *)malloc (sizeof (GLuint) * mesh->num_tris * 3);
	texCoords = (float *)malloc (sizeof (float) * mesh->num_verts * 2);
	normals = (vec3_t *)malloc (sizeof (vec3_t) * mesh->num_verts);

	/* Setup vertex indices */
	int k = 0;
	for (int i = 0; i < mesh->num_tris; ++i)
	{
		for (int j = 0; j < 3; ++j, ++k)
			vertexIndices[k] = mesh->triangles[i].index[j];
	}

	/* Setup texcoords */
	for (int i = 0; i < mesh->num_verts; ++i)
	{
		texCoords[i*2] = mesh->vertices[i].st[0];
		texCoords[i*2+1] = 1-mesh->vertices[i].st[1];
	}


	/* Setup vertices */
	for (int i = 0; i < mesh->num_verts; ++i)
	{
		vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };

		/* Calculate final vertex to draw with weights */
		for (int j = 0; j < mesh->vertices[i].count; ++j)
		{
			const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[i].start + j];
			const struct md5_joint_t *joint = &skeleton[weight->joint];

			/* Calculate transformed vertex for this weight */
			vec3_t wv;
			Quat_rotatePoint (joint->orient, weight->pos, wv);

			/* The sum of all weight->bias should be 1.0 */
			finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
			finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
			finalVertex[2] += (joint->pos[2] + wv[2]) * weight->bias;
		}

		vertexArray[i][0] = finalVertex[0];
		vertexArray[i][1] = finalVertex[1];
		vertexArray[i][2] = finalVertex[2];
	}

	//Zero the normals
	for (int i = 0; i < mesh->num_verts; ++i)
	{
		normals[i][0] = 0;
		normals[i][1] = 0;
		normals[i][2] = 0;
	}

	/* Setup normals */
	for (int i = 0; i < mesh->num_tris; ++i)
	{
		vec3_t *v[3], norm;
		for (int j = 0; j < 3; ++j)
		{
			v[j] = &vertexArray[ mesh->triangles[i].index[j] ];
		}
		vec3_t in1;
		vec3_t in2;
		in1[0] = (*v[1])[0] - (*v[0])[0];
		in1[1] = (*v[1])[1] - (*v[0])[1];
		in1[2] = (*v[1])[2] - (*v[0])[2];
		in2[0] = (*v[2])[0] - (*v[0])[0];
		in2[1] = (*v[2])[1] - (*v[0])[1];
		in2[2] = (*v[2])[2] - (*v[0])[2];
		vector3_CrossProduct(in2, in1, &norm);
		float x = norm[0];
		float y = norm[1];
		float z = norm[2];
		float len = sqrt( x * x + y * y + z * z );
		len = (len != 0.0f ? len : 1.0f);
		float lengthMul = 1.0f / len;
		x *= lengthMul;
		y *= lengthMul;
		z *= lengthMul;
		norm[0] = x;
		norm[1] = y;
		norm[2] = z;

		for (int j = 0; j < 3; ++j)
		{
			int ind = mesh->triangles[i].index[j];
			normals[ind][0] += norm[0];
			normals[ind][1] += norm[1];
			normals[ind][2] += norm[2];
		}
	}
	//Normalize the normals
	for (int i = 0; i < mesh->num_verts; ++i)
	{
		float x = normals[i][0];
		float y = normals[i][1];
		float z = normals[i][2];
		float len = sqrt( x * x + y * y + z * z );
		len = (len != 0.0f ? len : 1.0f);
		float lengthMul = 1.0f / len;
		x *= lengthMul;
		y *= lengthMul;
		z *= lengthMul;
		normals[i][0] = x;
		normals[i][1] = y;
		normals[i][2] = z;
	}
}

void Mesh::Prepare_frame(const struct md5_joint_t *skeleton)
{

	/* Setup vertices */
	for (int i = 0; i < mesh->num_verts; ++i)
	{
		vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };

		/* Calculate final vertex to draw with weights */
		for (int j = 0; j < mesh->vertices[i].count; ++j)
		{
			const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[i].start + j];
			const struct md5_joint_t *joint = &skeleton[weight->joint];

			/* Calculate transformed vertex for this weight */
			vec3_t wv;
			Quat_rotatePoint (joint->orient, weight->pos, wv);

			/* The sum of all weight->bias should be 1.0 */
			finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
			finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
			finalVertex[2] += (joint->pos[2] + wv[2]) * weight->bias;
		}

		vertexArray[i][0] = finalVertex[0];
		vertexArray[i][1] = finalVertex[1];
		vertexArray[i][2] = finalVertex[2];

		if(finalVertex[0] < low_corner.x)
			low_corner.x = finalVertex[0];
		else if(finalVertex[0] > high_corner.x)
			high_corner.x = finalVertex[0];

		if(finalVertex[1] < low_corner.y)
			low_corner.y = finalVertex[1];
		else if(finalVertex[1] > high_corner.y)
			high_corner.y = finalVertex[1];

		if(finalVertex[2] < low_corner.z)
			low_corner.z = finalVertex[2];
		else if(finalVertex[2] > high_corner.z)
			high_corner.z = finalVertex[2];
	}
}

void Mesh::Render() const
{
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer (3, GL_FLOAT, 0, vertexArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glNormalPointer(GL_FLOAT, 0, normals);

	glDrawElements (GL_TRIANGLES, mesh->num_tris * 3,
					GL_UNSIGNED_INT, vertexIndices);
}

Vector3 Mesh::Get_low_corner()
{
	return low_corner;
}

Vector3 Mesh::Get_high_corner()
{
	return high_corner;
}
