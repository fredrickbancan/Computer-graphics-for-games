#pragma once
#include <string>
struct Vertex;

class ObjLoader
{
public:
	static void getModelDataFromObjFile(const std::string& fileName, Vertex*& outVertices, unsigned int*& outIndices, int* outVertexCount, int* outIndicesCount);
};