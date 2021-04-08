#include "ObjLoader.h"
#include "Rendering.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
using namespace glm;

#define vertlist vector<Vertex>
#define strlist vector<string>
#define vec3list vector<vec3>
#define vec2list vector<vec2>
#define uilist vector<unsigned int>

//splits given string into vector of sub strings based on positions of delimeter (does not preserve delimiter characters)
static const strlist split(const string& src, const char& delimeter)
{
	string buffer = "";
	strlist result{};

	for (int i = 0; i < src.size(); i++)
	{
		char c = src[i];
		if (c != delimeter)
		{
			buffer += c;
		}
		else if (c == delimeter && buffer != "")
		{ 
			result.push_back(buffer);
			buffer = "";
		}
	}
	if (buffer != "")
	{
		result.push_back(buffer);
	}

	return result;
}

static void processVertexPos(const string& currentLine, vec3list& list)
{
	strlist floats = split(currentLine, ' ');
	list.push_back(vec3(stof(floats[1]), stof(floats[2]), stof(floats[3])));
}

static void processVertexUV(const string& currentLine, vec2list& list)
{
	strlist floats = split(currentLine, ' ');
	list.push_back(vec2(stof(floats[1]), stof(floats[2])));
}

static void processVertexNormal(const string& currentLine, vec3list& list)
{
	strlist floats = split(currentLine, ' ');
	list.push_back(vec3(stof(floats[1]), stof(floats[2]), stof(floats[3])));
}

static void processPolygonIndices(const string& currentLine, uilist& indices, uilist& uvIndices, uilist& normalIndices)
{
	strlist faceTriples = split(currentLine, ' ');// "f " "index/uvindex/faceindex/" "index/uvindex/faceindex/" "index/uvindex/faceindex/"
	int vertIndex = 0;
	for (strlist::iterator i = faceTriples.begin(); i != faceTriples.end(); ++i)//loop through vertex info and extract
	{
		if (vertIndex != 0)//skip over first "vertex". Its just "f "
		{
			strlist vertIndices = split(*i, '/');
			indices.push_back(stoi(vertIndices[0]) - 1);// - 1 because obj indices start from 1 instead of 0
			uvIndices.push_back(stoi(vertIndices[1]) - 1);
			normalIndices.push_back(stoi(vertIndices[2]) - 1);
		}
		vertIndex++;
	}
}

void ObjLoader::getModelDataFromObjFile(const string& fileName, Vertex*& outVertices, unsigned int*& outIndices, int* outVertexCount, int* outIndicesCount)
{
	string path = string("models/") + fileName;

	// If the file is not .obj
	if (path.substr(path.size() - 4, 4) != ".obj")
	{
		cout << "ObjLoader could not load file from path: " << path << ", file is not .obj format." << endl;
		return;
	}
	
	fstream stream = fstream(path);

	if (!stream.is_open())
	{
		cout << "ObjLoader could not open path: " << path << endl;
		return;
	}

	vec3list positions{};
	vec2list unorderedUvs{};
	vec3list unorderedNormals{};
	uilist posIndices{};
	uilist uvIndices{};
	uilist normalIndices{};
	string currentLine;
	vec2* orderedUvs;
	vec3* orderedNormals;
	vertlist resultVertices{};

	//process each line based on token
	while (getline(stream, currentLine))
	{
		if (currentLine.find("v ") == 0)
		{
			processVertexPos(currentLine, positions);
			continue;
		}

		if (currentLine.find("vt ") == 0)
		{
			processVertexUV(currentLine, unorderedUvs);
			continue;
		}

		if (currentLine.find("vn ") == 0)
		{
			processVertexNormal(currentLine, unorderedNormals);
			continue;
		}

		if (currentLine.find("f ") == 0)
		{
			processPolygonIndices(currentLine, posIndices, uvIndices, normalIndices);
			continue;
		}
	}
	stream.close();
	orderedUvs = new vec2[unorderedUvs.size()];

	//ordering uvs based on position indices (which are ordered by default)
	int itter = 0;
	for (uilist::iterator i = uvIndices.begin(); i != uvIndices.end(); ++i)
	{
		orderedUvs[posIndices[itter]] = unorderedUvs[uvIndices[itter]];
		itter++;
	}

	orderedNormals = new vec3[unorderedNormals.size()];

	//ordering normals based on position indices (which are ordered by default)
	itter = 0;
	for (uilist::iterator i = normalIndices.begin(); i != normalIndices.end(); ++i)
	{
		orderedNormals[posIndices[itter]] = unorderedNormals[normalIndices[itter]];
		itter++;
	}

	//building resulting vertices
	itter = 0;
	for (vec3list::iterator i = positions.begin(); i != positions.end(); ++i)
	{
		resultVertices.push_back(Vertex(*i, orderedUvs[itter], orderedNormals[itter], {1,1,1,1}));
		itter++;
	}

	delete[] orderedUvs;
	delete[] orderedNormals;

	*outVertexCount = resultVertices.size();
	*outIndicesCount = posIndices.size();
	outIndices = new unsigned int[posIndices.size()];
	outVertices = new Vertex[resultVertices.size()];

	//fill result indices array
	itter = 0;
	for (uilist::iterator i = posIndices.begin(); i != posIndices.end(); ++i)
	{
		outIndices[itter] = *i;
		itter++;
	}

	//fill result vertex array
	itter = 0;
	for (vertlist::iterator i = resultVertices.begin(); i != resultVertices.end(); ++i)
	{
		outVertices[itter] = *i;
		itter++;
	}

	cout << "ObjLoader Successfully loaded file: " << path << endl;
}
