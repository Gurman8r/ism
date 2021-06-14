#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>

namespace ism
{
	Node::~Node() { clear_children(); }
}