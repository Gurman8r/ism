#include <servers/audio_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AS * AS::__singleton{};

	EMBED_CLASS(AudioServer, t) {}

	AudioServer::AudioServer() { __singleton = this; }

	AudioServer::~AudioServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}