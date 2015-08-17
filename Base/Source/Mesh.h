#ifndef MESH_H
#define MESH_H
#define MAX_TEXTURE 8

#include <string>
#include "Material.h"
#include "Vertex.h"
#include "GL\glew.h"

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	virtual void Render();
	void Render(unsigned offset, unsigned count);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;

	unsigned textureID;
	unsigned textureArray[MAX_TEXTURE];

};

#endif