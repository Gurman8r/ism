#include <core/io/config_file.hpp>

#include <inih/INIReader.h>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(ConfigFile, t, "config_file")
{
	t.tp_base = typeof<Resource>();

	t.tp_new = (newfunc)[](TYPE type, OBJ args)->OBJ { return memnew(ConfigFile); };
}

namespace ism
{
	ConfigFile::~ConfigFile()
	{
	}
}