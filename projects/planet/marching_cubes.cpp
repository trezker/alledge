#include "marching_cubes.h"
#include "marching_cubes_data.h"

void Marching_cubes::Set_sampler(Sampler s) {
	sampler = s;
}

float Marching_cubes::SampleValue(Vector3 pos)
{
	return sampler(pos);
}

void Marching_cubes::Clear()
{
	vertices.clear();
	indices.clear();
	cornermap.clear();
}

void Marching_cubes::MarchCube(Vector3 minCornerPos)
{
	// construct case index from 8 corner samples
	int caseIndex = 0; 
	for(int i = 0; i < 8; i++)
	{
		float sample = SampleValue(minCornerPos + cornerOffsets[i]);
		if (sample >= 0.0f)
			caseIndex |= 1 << i;
	}

	// early out if entirely inside or outside the volume
	if (caseIndex == 0 || caseIndex == 0xFF)
		return;

	int caseVert = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int tri = 0; tri < 3; tri++)
		{
			// get edge index
			int edgeCase = triangleTable[caseIndex][caseVert];
			if (edgeCase == -1)
				return;
			Vector3 vert1 = minCornerPos + edgeVertexOffsets[edgeCase][0]; // beginning of the edge
			Vector3 vert2 = minCornerPos + edgeVertexOffsets[edgeCase][1]; // end of the edge

			// interpolate along the edge
			float s1 = SampleValue(minCornerPos + edgeVertexOffsets[edgeCase][0]);
			float s2 = SampleValue(minCornerPos + edgeVertexOffsets[edgeCase][1]);
			float dif = s1 - s2;
			if (dif == 0.0f)
				dif = 0.5f;
			else
				dif = s1 / dif;
			// Lerp
			Vector3 vertPosInterpolated = vert1 + ((vert2 - vert1) * dif);

//			Vector3 vertPosInterpolated = (vert1 + vert2) / 2.0f; // non interpolated version - in the middle of the edge
			
			if(cornermap.find(vertPosInterpolated) != cornermap.end()) {
				int index = cornermap[vertPosInterpolated];
				indices.push_back(index);
			}
			else
			{
				vertices.push_back(vertPosInterpolated);
				indices.push_back(vertices.size() - 1);
				cornermap[vertPosInterpolated] = vertices.size() - 1;
			}

			caseVert++;
		}
	}
}
