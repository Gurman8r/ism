#include <core/os/filesystem.hpp>
#include <core/error/error_macros.hpp>

#include <xfilesystem_abi.h>

namespace ism
{
	Path FileSystem::get_current_path()
	{
		auto _current_path = [&](WideString & text) noexcept -> ulong_t
		{
			text.resize(__std_fs_max_path);
			for (;;)
			{
				auto const req_size{ static_cast<ulong_t>(text.size()) };

				auto const temp_result{ __std_fs_get_current_path(req_size, text.data()) };

				text.resize(temp_result._Size);

				if (text.size() < req_size)
				{
					return (ulong_t)temp_result._Error;
				}
			}
		};

		Path path{};
		
		auto err{ _current_path(path.m_text) };
		
		VERIFY(err == EXIT_SUCCESS);
		
		return path;
	}
	
	void FileSystem::set_current_path(Path const & value)
	{
		auto _current_path = [&](WideString const & text) noexcept
		{
			return (ulong_t)__std_fs_set_current_path(text.data());
		};

		auto err{ _current_path(value.m_text) };

		VERIFY(err == EXIT_SUCCESS);
	}
}