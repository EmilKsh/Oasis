#pragma once
#include<iostream>
#include<fstream>
#include"IOModel.h"

using model::Model, std::string;

namespace IOModel{

	Model LoadObjModel(string filePath) {

		string fileAsText;
		std::ifstream objFile(filePath);
		Model tempModel;
		std::vector<float> tempBuffer;

		while (getline(objFile, fileAsText)) {


			if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "v ") {
				const int numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');

				rsize_t pos1 = fileAsText.find(" ") + 1;
				for (size_t i = 0; i < numOfParams; i++)
				{
					const size_t& pos2 = fileAsText.substr(pos1).find(" ") + pos1;

					tempModel.vertexPoses.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
					pos1 = pos2 + 1;
				}
			}
			else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "vt ") {
				const int& numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');

				size_t pos1 = fileAsText.find(" ") + 1;
				for (size_t i = 0; i < numOfParams; i++)
				{
					const size_t& pos2 = fileAsText.substr(pos1).find(" ") + pos1;

					tempModel.vertexTexCoords.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
					pos1 = pos2 + 1;
				}
			}
			else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "vn ") {
				const int& numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');

				size_t pos1 = fileAsText.find(" ") + 1;
				for (size_t i = 0; i < numOfParams; i++)
				{
					const size_t& pos2 = fileAsText.substr(pos1).find(" ") + pos1;

					tempModel.vertexNormals.push_back(std::stof(fileAsText.substr(pos1, pos2 - pos1)));
					pos1 = pos2 + 1;
				}
			}
			else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "f ") {
				const int& numOfParams = std::count(fileAsText.begin(), fileAsText.end(), ' ');

				size_t pos1 = fileAsText.find(" ") + 1;
				for (size_t i = 0; i < numOfParams; i++)
				{
					const size_t& pos2 = fileAsText.substr(pos1).find(" ") + pos1;
					const size_t& vtpos = fileAsText.substr(pos1).find("/") + pos1 + 1;
					const size_t& vnpos = fileAsText.substr(vtpos).find("/") + vtpos + 1;

					const int& vertexIndex = std::stoi(fileAsText.substr(pos1, pos2 - pos1)) - 1;
					tempModel.indices.push_back(vertexIndex);
					/*tempBuffer.insert(tempBuffer.begin() + 5 * vertexIndex, tempModel.vertexPoses.at(3 * vertexIndex));
					tempBuffer.insert(tempBuffer.begin() + 5 * vertexIndex + 1, tempModel.vertexPoses.at(3 * vertexIndex + 1));
					tempBuffer.insert(tempBuffer.begin() + 5 * vertexIndex + 2, tempModel.vertexPoses.at(3 * vertexIndex + 2));
					tempBuffer.insert(tempBuffer.begin() + 5 * vertexIndex + 3, tempModel.vertexTexCoords.at(2 * std::stoi(fileAsText.substr(vtpos, vnpos - vtpos - 1)) - 2));
					tempBuffer.insert(tempBuffer.begin() + 5 * vertexIndex + 4, tempModel.vertexTexCoords.at(2 * std::stoi(fileAsText.substr(vtpos, vnpos - vtpos - 1)) - 1));*/

					pos1 = pos2 + 1;
				}
			}
		}

		tempModel.numOfVertices = tempModel.vertexPoses.size() / 3;
		std::cout << "number of vertices: " << tempModel.vertexPoses.size() / 3 << std::endl;
		std::cout << "number of indices: " << tempModel.indices.size() << std::endl;
		std::cout << "number of vertex texture coords: " << tempModel.vertexTexCoords.size() / 2 << std::endl;
		tempModel.CreateDataBuffer();
		//tempModel.vertexBuffer = tempBuffer;
		return tempModel;
	}

}