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
		DEFINE_CLASS(AudioServer, Object);

		static AudioServer * __singleton;

	public:
		AudioServer();
		virtual ~AudioServer() override;
		FORCE_INLINE static AudioServer * get_singleton() noexcept { return __singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using AS = AudioServer;

	SINGLETON_WRAPPER(AS, get_audio);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_AUDIO_SERVER_HPP_
