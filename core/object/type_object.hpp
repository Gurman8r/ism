#ifndef _ISM_TYPE_OBJECT_HPP_
#define _ISM_TYPE_OBJECT_HPP_

#include <core/object/detail/internals.hpp>

// type
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type object
	class ISM_API TypeObject : public Object
	{
	private:
		friend class Internals;

		friend class Handle<TypeObject>;

		static TypeObject __class_type;

	protected:
		static void initialize_class();

		virtual void _initialize_classv() override;

		virtual TYPE _get_typev() const noexcept override;

	public:
		NODISCARD static TYPE get_class() noexcept;

		TypeObject() noexcept;

		template <class T
		> TypeObject(mpl::type_tag<T>, cstring name, int32_t flags = TypeFlags_Default) : TypeObject{}
		{
			tp_name = name;
			tp_size = sizeof(T);
			tp_flags = flags;
			tp_base = baseof<T>();
			tp_bind = (bindfunc)[](TYPE type) { return type; };
			tp_cmp = (cmpfunc)[](OBJ lhs, OBJ rhs) { return util::compare(*lhs, *rhs); };
			tp_del = (delfunc)memdelete<T>;
		}

	public:
		String				tp_name				{};
		ssize_t				tp_size				{};
		int32_t				tp_flags			{};

		ssize_t				tp_dictoffset		{};
		ssize_t				tp_vectorcalloffset	{};

		bindfunc			tp_bind				{};
		vectorcallfunc		tp_vectorcall		{};

		getattrfunc			tp_getattr			{};
		setattrfunc			tp_setattr			{};
		getattrofunc		tp_getattro			{};
		setattrofunc		tp_setattro			{};
		descrgetfunc		tp_descr_get		{};
		descrsetfunc		tp_descr_set		{};

		binaryfunc			tp_call				{};
		cmpfunc				tp_cmp				{};
		delfunc				tp_del				{};
		hashfunc			tp_hash				{};
		lenfunc				tp_len				{};
		newfunc				tp_new				{};
		reprfunc			tp_repr				{};
		reprfunc			tp_str				{};
		
		Ref<TypeObject>		tp_base				{ /* TYPE */ };
		OBJ					tp_bases			{ /* LIST */ };
		OBJ					tp_dict				{ /* DICT */ };
		OBJ					tp_mro				{ /* LIST */ };
		OBJ					tp_subclasses		{ /* LIST */ };

	public:
		NODISCARD bool ready();

		NODISCARD OBJ lookup(OBJ name) const;

		NODISCARD bool is_subtype(TYPE value) const;

		bool check_consistency() const;

		void modified();

		Error update_slot(STR name);

	protected:
		bool add_subclass(TypeObject * type);

		bool mro_internal(OBJ * old_mro);

		void inherit_special(TypeObject * base);

		void inherit_slots(TypeObject * base);

	protected:
		template <class Slot> bool slot_defined(TypeObject * base, Slot TypeObject:: * slot) const
		{
			return (this->*slot) && (!base || (this->*slot) != (base->*slot));
		}

		template <class Slot> void copy_val(TypeObject * base, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base)
			{
				(this->*slot) = (base->*slot);
			}
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base && base->slot_defined(basebase, slot))
			{
				(this->*slot) = (base->*slot);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type delete
	template <> struct DefaultDelete<TypeObject> : DefaultDelete<Object> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type check
#define OBJ_CHECK_TYPE(o) (ism::typeof(o).has_feature(ism::TypeFlags_Type_Subclass))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type handle
	HANDLE_CLASS(TypeObject)
	{
		HANDLE_COMMON(TypeObject, OBJ_CHECK_TYPE);

	public:
		NODISCARD bool ready() const { return m_ptr->ready(); }

		NODISCARD bool has_feature(int32_t flag) const { return flag_read(m_ptr->tp_flags, flag); }

		NODISCARD bool is_abstract() const { return has_feature(TypeFlags_IsAbstract); }

		NODISCARD bool is_final() const { return has_feature(TypeFlags_IsFinal); }

		NODISCARD bool is_local() const { return has_feature(TypeFlags_IsLocal); }

		NODISCARD bool is_subtype(TYPE const & value) const { return m_ptr->is_subtype(value); }

		NODISCARD OBJ lookup(OBJ const & name) const { return m_ptr->lookup(name); }

		NODISCARD auto name() const { return attr("__name__"); }

		template <class Name = cstring, class Value = OBJ
		> void add_object(Name && name, Value && value) noexcept
		{
			if (!m_ptr || !m_ptr->tp_dict || !m_ptr->ready()) { return; }

			STR str_name{ FWD(name) };

			DICT(m_ptr->tp_dict)[str_name] = FWD(value);

			m_ptr->modified();

			if (is_dunder_name(str_name))
			{
				m_ptr->update_slot(str_name);
			}

			VERIFY(m_ptr->check_consistency());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD hash_t hash(Handle<T> const & o) noexcept
	{
		if (!o) { return 0; }
		TYPE t{ typeof(o) };
		return t->tp_hash ? t->tp_hash(o) : 0;
	}

	template <class T> NODISCARD ssize_t len(Handle<T> const & o) noexcept
	{
		if (!o) { return -1; }
		TYPE t{ typeof(o) };
		return t->tp_len ? t->tp_len(o) : -1;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_dict_ptr(TYPE const & t, OBJ const & o) -> OBJ *
	{
		if (t && o && 0 < t->tp_dictoffset)
		{
			return reinterpret_cast<OBJ *>(reinterpret_cast<char *>(*o) + t->tp_dictoffset);
		}
		else
		{
			return (OBJ *)nullptr;
		}
	}

	NODISCARD inline auto get_dict_ptr(OBJ const & o) noexcept -> OBJ *
	{
		return ism::get_dict_ptr(typeof(o), o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline auto get_vectorcall_func(TYPE const & t, OBJ const & o) -> vectorcallfunc
	{
		if (t && o && 0 < t->tp_vectorcalloffset)
		{
			return *reinterpret_cast<vectorcallfunc *>(reinterpret_cast<char *>(*o) + t->tp_vectorcalloffset);
		}
		else
		{
			return (vectorcallfunc)nullptr;
		}
	}

	NODISCARD inline auto get_vectorcall_func(OBJ const & o) noexcept -> vectorcallfunc
	{
		return ism::get_vectorcall_func(typeof(o), o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ISM_API_FUNC(OBJ) type_getattro(TYPE type, OBJ name);

	ISM_API_FUNC(Error) type_setattro(TYPE type, OBJ name, OBJ value);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_OBJECT_HPP_
