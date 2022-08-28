#ifndef _ISM_AUDIO_SERVER_HPP_
#define _ISM_AUDIO_SERVER_HPP_

// PLACEHOLDER

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// audio server - PLACEHOLDER
	class AudioServer : public Object
	{
		OBJECT_CLASS(AudioServer, Object);

		static AudioServer * __singleton;

	public:
		AudioServer();

		virtual ~AudioServer() override;

		FORCE_INLINE static AudioServer * get_singleton() noexcept { return __singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using AS = AudioServer;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_AUDIO_SERVER_HPP_
