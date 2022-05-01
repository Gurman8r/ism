#ifndef _ISM_AUDIO_SERVER_HPP_
#define _ISM_AUDIO_SERVER_HPP_

#include <core/object/detail/class.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	// PLACEHOLDER
	class ISM_API AudioServer : public Object
	{
		OBJECT_COMMON(AudioServer, Object);

		static AudioServer * singleton;

	public:
		explicit AudioServer();

		virtual ~AudioServer();

		NODISCARD static AudioServer * get_singleton() noexcept { return singleton; }
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_AUDIO_SERVER_HPP_
