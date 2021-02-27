#include "parserSTL.h"
#include <fstream>
#include <iomanip>

namespace graphics {
	
	ParserSTL::ParserSTL(const std::string& fileName) {
		readSTL(fileName);
	}
	
	void ParserSTL::readFile(const std::string &fileName) {
		std::vector<dataStructSTL>().swap(modelMesh);
		readSTL(fileName);
	}
	
	void ParserSTL::readSTL(const std::string &fileName) {
		std::fstream finout(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
		if (!finout) {
			std::cerr << "FILE IS NOT OPEN!" << std::endl;
			std::vector<dataStructSTL>().swap(modelMesh);
		}
		else
		{
			dataStructSTL elementSTL;
			finout.seekg(84, std::ios_base::beg);
			while (finout)
			{
				finout.read((char*)&elementSTL.normalVec, sizeof(elementSTL.normalVec));
				for (int i = 0; i < 3; ++i)
				{
					finout.read((char*)&elementSTL.vertexPos, sizeof(elementSTL.vertexPos));
					modelMesh.push_back(elementSTL);
				}
				finout.seekg(2, std::ios_base::cur);
			}
			finout.close();
		}
	}

}