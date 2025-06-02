#pragma once
#include<iostream>
#include<fstream>
#include<map>
#include<tuple>
#include"IOModel.h"

using model::Model, std::string;

namespace IOModel{

	Model LoadObjModel(string filePath) {

		Model tempModel;
		std::vector<float> tempBuffer;
		std::map<std::tuple<int, int>, int> indexMap;
		string fileAsText;
		std::ifstream file;
		file.open(filePath);

		if (!file.is_open())
		{
			std::cerr << "Error openning file: " << filePath;

			if (file.bad()) {
				std::cerr << "Fatal error: badbit is set." << std::endl;
			}

			return tempModel;
		}

		while (getline(file, fileAsText)) {

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
					const size_t& vtid = std::stoi(fileAsText.substr(vtpos, vnpos - vtpos - 1)) - 1;
					const int& vid = std::stoi(fileAsText.substr(pos1, pos2 - pos1)) - 1;
					const std::tuple<int, int>& key{ vid, vtid };


					if (!indexMap.contains(key))
					{
						indexMap[key] = tempBuffer.size() / 5;
						tempBuffer.push_back(tempModel.vertexPoses.at(3 * vid));
						tempBuffer.push_back(tempModel.vertexPoses.at(3 * vid + 1));
						tempBuffer.push_back(tempModel.vertexPoses.at(3 * vid + 2));
						tempBuffer.push_back(tempModel.vertexTexCoords.at(2 * vtid));
						tempBuffer.push_back(tempModel.vertexTexCoords.at(2 * vtid + 1));
					}

					tempModel.indices.push_back(indexMap[key]);
					pos1 = pos2 + 1;
				}
			}
		}

		tempModel.numOfVertices = tempModel.vertexPoses.size() / 3;
		std::cout << "number of vertices: " << tempModel.vertexPoses.size() / 3 << std::endl;
		std::cout << "number of indices: " << tempModel.indices.size() << std::endl;
		std::cout << "number of vertex texture coords: " << tempModel.vertexTexCoords.size() / 2 << std::endl;
		tempModel.vertexBuffer = tempBuffer;
		return tempModel;
	}
}