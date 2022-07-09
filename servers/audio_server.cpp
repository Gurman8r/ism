#include <servers/audio_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AS * AS::__singleton{};

	OBJECT_EMBED(AudioServer, t) {}

	AudioServer::AudioServer() { __singleton = this; }

	AudioServer::~AudioServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}