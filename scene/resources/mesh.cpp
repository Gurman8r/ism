#include <scene/resources/mesh.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Mesh, t, "mesh")
{
	t.tp_base = typeof<Resource>();
}

Mesh::~Mesh()
{
}

Mesh::Mesh() : Mesh{ get_class() }
{
}