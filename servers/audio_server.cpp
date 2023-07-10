#include <servers/audio_server.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(AudioServer, t) {}

	SINGLETON_EMBED(AudioServer);

	AudioServer::AudioServer() { SINGLETON_CTOR(); }

	AudioServer::~AudioServer() { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}