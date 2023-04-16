#include <servers/audio_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AudioServer * AudioServer::__singleton{};

	EMBED_CLASS(AudioServer, t) {}

	AudioServer::AudioServer() { __singleton = this; }

	AudioServer::~AudioServer() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}