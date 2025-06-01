#pragma once
#include<vector>
#include<string>

namespace model {
	class Model
	{
	private:
		int object_id = 0;

	public:
		// graphics variables
		std::vector<float> vertexPoses;
		std::vector<int> indices;
		std::vector<float> vertexTexCoords;
		std::vector<float> vertexBuffer;
		std::vector<float> vertexNormals;

		size_t numOfVertices;

		void CreateDataBuffer();
	};
}