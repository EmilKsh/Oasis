#include<iostream>
#include<fstream>
#include<string>
#include "Model.h"

void Model::CreateDataBuffer() {
	for (size_t id=0; id < this->vertexPoses.size() / 3; id++) {
		vertexBuffer.push_back(vertexPoses.at(3*id));
		vertexBuffer.push_back(vertexPoses.at(3 * id + 1));
		vertexBuffer.push_back(vertexPoses.at(3 * id + 2));
		vertexBuffer.push_back(vertexTexCoords.at(2 * id));
		vertexBuffer.push_back(vertexTexCoords.at(2 * id + 1));
	}
}

using std::string_view, std::string, std::ifstream, std::getline;


Model parseObj(string filePath = "") {

	string fileAsText;
	ifstream objFile(filePath);
	Model tempModel;

	while (getline(objFile, fileAsText)) {


		if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "v ") {
			const int numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');
			
			rsize_t pos1 = fileAsText.find(" ") + 1;
			for (size_t i = 0; i < numOfParams; i++)
			{
				size_t pos2 = fileAsText.substr(pos1).find(" ") + pos1;
				
				tempModel.vertexPoses.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
				pos1 = pos2 + 1;
			}
		}
		else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "vt ") {
			const int numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');
			
			rsize_t pos1 = fileAsText.find(" ") + 1;
			for (size_t i = 0; i < numOfParams; i++)
			{
				size_t pos2 = fileAsText.substr(pos1).find(" ") + pos1;

				tempModel.vertexTexCoords.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
				pos1 = pos2 + 1;
			}
		}
		else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "vn ") {
			const int numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');

			rsize_t pos1 = fileAsText.find(" ") + 1;
			for (size_t i = 0; i < numOfParams; i++)
			{
				size_t pos2 = fileAsText.substr(pos1).find(" ") + pos1;

				tempModel.vertexNormals.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
				pos1 = pos2 + 1;
			}
		}
		else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "f ") {
			const int numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');

			rsize_t pos1 = fileAsText.find(" ") + 1;
			for (size_t i = 0; i < numOfParams; i++)
			{
				const size_t pos2 = fileAsText.substr(pos1).find(" ") + pos1;
				const size_t vtpos = fileAsText.substr(pos1).find("/") + pos1 + 1;
				const size_t vnpos = fileAsText.substr(vtpos).find("/") + vtpos + 1;

				tempModel.vertexNormals.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
				pos1 = pos2 + 1;
			}
			//size_t id1pos1 = fileAsText.find(" ") + 1;
			//size_t id1pos2 = fileAsText.find("/");
			//size_t id2pos1 = fileAsText.substr(id1pos2).find(" ") + id1pos2 + 1;
			//size_t id2pos2 = fileAsText.substr(id2pos1).find("/") + id2pos1;
			//size_t id3pos1 = fileAsText.substr(id2pos2).find(" ") + id2pos2 + 1;
			//size_t id3pos2 = fileAsText.substr(id3pos1).find("/") + id3pos1;

			//// Debug print
			//// std::cout << fileAsText << std::endl;
			//// std::cout << fileAsText.substr(id1pos1, id1pos2 - id1pos1) << ", " << fileAsText.substr(id2pos1, id2pos2 - id2pos1) << ", " << fileAsText.substr(id3pos1, id3pos2 - id3pos1) << std::endl;

			//tempModel.indices.push_back(std::stoi(fileAsText.substr(id1pos1, id1pos2 - id1pos1)) - 1);
			//tempModel.indices.push_back(std::stoi(fileAsText.substr(id2pos1, id2pos2 - id2pos1)) - 1);
			//tempModel.indices.push_back(std::stoi(fileAsText.substr(id3pos1, id3pos2 - id3pos1)) - 1);
		}
	}

	tempModel.numOfVertices = tempModel.vertexPoses.size() / 3;
	std::cout << "number of vertices: " << tempModel.vertexPoses.size() / 3 << std::endl;
	std::cout << "number of indices: " << tempModel.indices.size() << std::endl;
	std::cout << "number of vertex texture coords: " << tempModel.vertexTexCoords.size() / 2 << std::endl;
	tempModel.CreateDataBuffer();
	return tempModel;
}
