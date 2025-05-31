#pragma once
#include<vector>
#include<string>

using std::vector;


class Model
{
	/*Model() {
		if (!vertices.empty())
		{

		}
	}*/
	// general variables
	int object_id = 0;

public: 
	// graphics variables
	vector<float> vertexPoses;
	vector<int> indices;
	vector<float> vertexTexCoords;
	vector<float> vertexBuffer;
	vector<float> vertexNormals;

	size_t numOfVertices;

	void CreateDataBuffer();
	
};
