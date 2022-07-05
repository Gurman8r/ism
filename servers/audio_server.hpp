#ifndef _ISM_AUDIO_SERVER_HPP_
#define _ISM_AUDIO_SERVER_HPP_

// PLACEHOLDER

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class AudioServer : public Object
	{
		OBJECT_COMMON(AudioServer, Object);

		static AudioServer * g_singleton;

	public:
		AudioServer();

		virtual ~AudioServer() override;

		FORCE_INLINE static AudioServer * get_singleton() noexcept { return g_singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(AS) AudioServer;

#define AUDIO_SERVER (ism::AS::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_AUDIO_SERVER_HPP_
