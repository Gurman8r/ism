#ifndef _ISM_AUDIO_SERVER_HPP_
#define _ISM_AUDIO_SERVER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// audio server - PLACEHOLDER
	class AudioServer : public Object
	{
		DEFINE_CLASS(AudioServer, Object);

		static AudioServer * __singleton;

	protected:
		using CreateFunc = AudioServer * (*)();
		static CreateFunc __create_func;
		AudioServer();

	public:
		virtual ~AudioServer() override;
		FORCE_INLINE static AudioServer * get_singleton() noexcept { return __singleton; }
		static AudioServer * create();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using AS = AudioServer;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_AUDIO_SERVER_HPP_
