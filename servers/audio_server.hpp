#ifndef _ISM_AUDIO_SERVER_HPP_
#define _ISM_AUDIO_SERVER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// audio server - PLACEHOLDER
	class AudioServer : public Object
	{
		OBJECT_CLASS(AudioServer, Object);

		SINGLETON_CLASS(AudioServer);

	public:
		AudioServer();
		virtual ~AudioServer() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using AS = AudioServer;

	SINGLETON_WRAPPER(AudioServer, audio_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_AUDIO_SERVER_HPP_
