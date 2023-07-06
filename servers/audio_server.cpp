#include <servers/audio_server.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(AudioServer, t) {}

	AudioServer * AudioServer::__singleton{};

	AudioServer::AudioServer() { SINGLETON_CTOR(__singleton, this); }

	AudioServer::~AudioServer() { SINGLETON_DTOR(__singleton, this); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}