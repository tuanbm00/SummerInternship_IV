#include "stdafx.h"
#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(const Vertex & v)
{
	pos.x = v.pos.x;
	pos.y = v.pos.y;
	pos.z = v.pos.z;
	uv.x = v.uv.x;
	uv.y = v.uv.y;
}
