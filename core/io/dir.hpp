#ifndef _ISM_DIR_HPP_
#define _ISM_DIR_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	enum DirAccess_ {
		DirAccess_Resources,
		DirAccess_User,
		DirAccess_Filesystem,
		DirAccess_MAX
	};

	class ISM_API Dir : public Object
	{
		OBJECT_CLASS(Dir, Object);

		static Error_ __last_dir_open_error;
		DirAccess_ m_access_type{};
		bool m_include_navigational{};
		bool m_include_hidden{};

	public:
		template <class C = char, class = std::enable_if_t<mpl::is_char_v<C>>
		> static constexpr C delimiter{ static_cast<C>(
#			if SYSTEM_WINDOWS
				'\\'
#			else
				'/'
#			endif
		) };

	protected:
		using CreateFunc = Ref<Dir>(*)();
		static CreateFunc __create_func[DirAccess_MAX];

		Error_ _copy_dir(Ref<Dir> & from, String const & to, i32 chmod_flags, bool copy_links);
		String _get_root_path() const;
		virtual String _get_root_string() const;
		DirAccess_ get_access_type() const;
		virtual String fix_path(String path) const;

		template <class T
		> static Ref<Dir> _create_builtin() {
			return memnew(T);
		}

	public:
		virtual ~Dir() = default;

		virtual Error_ list_dir_begin() = 0; ///< This starts dir listing
		NODISCARD virtual String get_next() = 0;
		NODISCARD virtual bool current_is_dir() const = 0;
		NODISCARD virtual bool current_is_hidden() const = 0;
		virtual void list_dir_end() = 0; ///<

		NODISCARD virtual i32 get_drive_count() = 0;
		NODISCARD virtual String get_drive(i32 drive) = 0;
		NODISCARD virtual i32 get_current_drive();
		NODISCARD virtual bool drives_are_shortcuts();

		virtual Error_ change_dir(String path) = 0; // can be relative or abs, return false on success
		NODISCARD virtual String get_current_dir(bool include_drive = true) const = 0; // return current dir location
		virtual Error_ make_dir(String path) = 0;
		virtual Error_ make_dir_recursive(String path);
		virtual Error_ erase_contents_recursive(); // super dangerous, use with care!

		NODISCARD virtual bool file_exists(String path) = 0;
		NODISCARD virtual bool dir_exists(String path) = 0;
		NODISCARD virtual bool is_readable(String path) { return true; };
		NODISCARD virtual bool is_writable(String path) { return true; };
		NODISCARD static bool exists(String path);
		NODISCARD virtual u64 get_space_left() = 0;

		Error_ copy_dir(String from, String to, i32 chmod_flags = -1, bool copy_links = false);
		virtual Error_ copy(String from, String to, i32 chmod_flags = -1);
		virtual Error_ rename(String from, String to) = 0;
		virtual Error_ remove(String path) = 0;

		NODISCARD virtual bool is_link(String path) = 0;
		NODISCARD virtual String read_link(String path) = 0;
		virtual Error_ create_link(String src, String dst) = 0;

		static void remove_file_or_error(String path) {
			Ref<Dir> da = create(DirAccess_Filesystem);
			if (da->file_exists(path)) {
				if (da->remove(path) != Error_OK) {
					CRASH("cannot remove file or directory");
				}
			}
			else {
				CRASH("cannot remove non-existant file or directory");
			}
		}

		NODISCARD virtual String get_filesystem_type() const = 0;
		NODISCARD static String get_full_path(String const & path, DirAccess_ access);
		NODISCARD static Ref<Dir> create_for_path(String const & path);

		NODISCARD static Ref<Dir> create(DirAccess_ access);
		NODISCARD static Error_ get_open_error();

		template <class T
		> static void make_default(DirAccess_ access) {
			create_func[access] = _create_builtin<T>;
		}

		NODISCARD static Ref<Dir> open(String const & path, Error_ * r_error = nullptr);

		NODISCARD static i32 _get_drive_count();
		NODISCARD static String get_drive_name(i32 index);

		static Error_ make_dir_abs(String const & dir);
		static Error_ make_dir_recursive_abs(String const & dir);
		NODISCARD static bool dir_exists_abs(String const & dir);

		static Error_ copy_abs(String const & from, String const & to, i32 chmod_flags = -1);
		static Error_ rename_abs(String const & from, String const & to);
		static Error_ remove_abs(String const & path);

		NODISCARD Vector<String> get_contents(bool directories);
		NODISCARD Vector<String> get_files();
		NODISCARD static Vector<String> get_files_at(String const & path);
		NODISCARD Vector<String> get_directories();
		NODISCARD static Vector<String> get_directories_at(String const & path);
		NODISCARD String _get_next();

		NODISCARD bool get_include_hidden() const { return m_include_hidden; }
		void set_include_hidden(bool enable) { m_include_hidden = enable; }

		NODISCARD bool get_include_navigational() const { return m_include_navigational; }
		void set_include_navigational(bool enable) { m_include_navigational = enable; }
	};
}

#endif // !_ISM_DIR_HPP_
