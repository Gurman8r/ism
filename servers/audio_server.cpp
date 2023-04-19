#include <servers/audio_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(AudioServer, t) {}

	AudioServer * AudioServer::__singleton{};

	AudioServer::CreateFunc AudioServer::__create_func{ []() { return memnew(AudioServer); } };

	AudioServer::AudioServer()
	{
		__singleton = this;
	}

	AudioServer::~AudioServer()
	{
	}

	AudioServer * AudioServer::create()
	{
		return VALIDATE(__create_func)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}