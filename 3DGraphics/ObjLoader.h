#pragma once
#include <string>
struct Vertex;

/*Static class for loading obj data*/
class ObjLoader
{
public:
	/*Loads and processes the obj file at the provided directory, outputs vertices array, indices array and their lengths.*/
	static void getModelDataFromObjFile(const std::string& fileName, Vertex*& outVertices, unsigned int*& outIndices, int* outVertexCount, int* outIndicesCount);
};