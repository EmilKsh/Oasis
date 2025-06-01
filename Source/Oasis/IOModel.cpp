#pragma once
#include<iostream>
#include<fstream>
#include"IOModel.h"

using model::Model, std::string;

namespace IOModel{

	model::Model LoadObjModel(std::string filePath) {

		string fileAsText;
		std::ifstream objFile(filePath);
		Model tempModel;

		while (std::getline(objFile, fileAsText)) {


			if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "v ") {
				size_t xpos1 = fileAsText.find(" ") + 1;
				size_t xpos2 = fileAsText.substr(xpos1).find(" ") + xpos1;
				size_t ypos1 = xpos2 + 1;
				size_t ypos2 = fileAsText.substr(ypos1).find(" ") + ypos1;
				size_t zpos1 = ypos2 + 1;

				//std::cout << xpos1 << " " << xpos2 << " " << ypos1 << " " << ypos2 << " " << zpos1 << std::endl;

				//std::cout << fileAsText.substr(xpos1, xpos2-xpos1) << ", " << fileAsText.substr(ypos1, ypos2-ypos1) << ", " << fileAsText.substr(zpos1) << std::endl;
				tempModel.vertexPoses.push_back(std::stof(fileAsText.substr(xpos1, xpos2 - xpos1)));
				tempModel.vertexPoses.push_back(std::stof(fileAsText.substr(ypos1, ypos2 - ypos1)));
				tempModel.vertexPoses.push_back(std::stof(fileAsText.substr(zpos1)));


			}
			else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "vt ") {
				size_t upos1 = fileAsText.find(" ") + 1;
				size_t upos2 = fileAsText.substr(upos1).find(" ") + upos1;
				size_t vpos1 = upos2 + 1;

				tempModel.vertexTexCoords.push_back(std::stof(fileAsText.substr(upos1, upos2 - upos1)));
				tempModel.vertexTexCoords.push_back(std::stof(fileAsText.substr(vpos1)));
			}
			else if (fileAsText.substr(0, fileAsText.find(" ") + 1) == "f ") {
				size_t id1pos1 = fileAsText.find(" ") + 1;
				size_t id1pos2 = fileAsText.find("/");
				size_t id2pos1 = fileAsText.substr(id1pos2).find(" ") + id1pos2 + 1;
				size_t id2pos2 = fileAsText.substr(id2pos1).find("/") + id2pos1;
				size_t id3pos1 = fileAsText.substr(id2pos2).find(" ") + id2pos2 + 1;
				size_t id3pos2 = fileAsText.substr(id3pos1).find("/") + id3pos1;

				// Debug print
				// std::cout << fileAsText << std::endl;
				// std::cout << fileAsText.substr(id1pos1, id1pos2 - id1pos1) << ", " << fileAsText.substr(id2pos1, id2pos2 - id2pos1) << ", " << fileAsText.substr(id3pos1, id3pos2 - id3pos1) << std::endl;

				tempModel.indices.push_back(std::stoi(fileAsText.substr(id1pos1, id1pos2 - id1pos1)));
				tempModel.indices.push_back(std::stoi(fileAsText.substr(id2pos1, id2pos2 - id2pos1)));
				tempModel.indices.push_back(std::stoi(fileAsText.substr(id3pos1, id3pos2 - id3pos1)));
			}
		}

		tempModel.numOfVertices = tempModel.vertexPoses.size() / 3;
		std::cout << "number of vertices: " << tempModel.vertexPoses.size() / 3 << std::endl;
		std::cout << "number of indices: " << tempModel.indices.size() << std::endl;
		std::cout << "number of vertex texture coords: " << tempModel.vertexTexCoords.size() / 2 << std::endl;
		tempModel.CreateDataBuffer();
		return tempModel;
	}

}