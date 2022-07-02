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
		friend class TYPE;
		friend class Internals;
		friend class EmbedObjectClassHelper<TypeObject>;
		friend struct DefaultDelete<TypeObject>;

		static constexpr StringView __class_static{ "TypeObject" };

		static TypeObject g_type_static;

	protected:
		static void initialize_class();

		virtual void _initialize_classv() override;

		virtual StringView _get_classv() const noexcept override { return get_class_static(); }

		virtual TYPE _get_typev() const noexcept override;

	public:
		NODISCARD static constexpr StringView get_class_static() noexcept { return __class_static; }

		NODISCARD static TYPE get_type_static() noexcept;

		TypeObject() noexcept {}

		template <class T
		> TypeObject(mpl::type_tag<T>, cstring name, int32_t flags = TypeFlags_None) noexcept : TypeObject{}
		{
			tp_name = name;
			tp_size = sizeof(T);
			tp_flags = flags;
			tp_base = ism::baseof<T>();
			tp_del = (delfunc)ism::memdelete<T>;
			tp_install = (installerfunc)[](TYPE t) -> TYPE { return t; };
			tp_hash = (hashfunc)[](OBJ o) -> hash_t { return ism::Hasher<intptr_t>{}((intptr_t)*o); };
			tp_cmp = (cmpfunc)[](OBJ a, OBJ b) -> int32_t { return util::compare((intptr_t)*a, (intptr_t)*b); };

			if constexpr (std::is_default_constructible_v<T>) {
				tp_new = (newfunc)[](TYPE, OBJ) -> OBJ { return memnew(T); };
			}

			if constexpr (std::is_base_of_v<TypeObject, T>) { tp_flags |= TypeFlags_Type_Subclass; }
			else if constexpr (std::is_base_of_v<IntObject, T>) { tp_flags |= TypeFlags_Int_Subclass; }
			else if constexpr (std::is_base_of_v<FloatObject, T>) { tp_flags |= TypeFlags_Float_Subclass; }
			else if constexpr (std::is_base_of_v<StringObject, T>) { tp_flags |= TypeFlags_Str_Subclass; }
			else if constexpr (std::is_base_of_v<TupleObject, T>) { tp_flags |= TypeFlags_Tuple_Subclass; }
			else if constexpr (std::is_base_of_v<ListObject, T>) { tp_flags |= TypeFlags_List_Subclass; }
			else if constexpr (std::is_base_of_v<DictObject, T>) { tp_flags |= TypeFlags_Dict_Subclass; }
		}

	public:
		String				tp_name				{};
		ssize_t				tp_size				{};
		int32_t				tp_flags			{};
		installerfunc		tp_install			{};
		ssize_t				tp_dictoffset		{};
		ssize_t				tp_vectorcalloffset	{};

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
		vectorcallfunc		tp_vectorcall		{};
		
		Ref<TypeObject>		tp_base				{ /* TYPE */ };
		OBJ					tp_bases			{ /* LIST */ };
		OBJ					tp_dict				{ /* DICT */ };
		OBJ					tp_mro				{ /* LIST */ };
		OBJ					tp_subclasses		{ /* DICT */ };

	public:
		NODISCARD bool ready();

		NODISCARD OBJ lookup(OBJ const & name) const;

		NODISCARD bool is_subtype(TYPE const & value) const;

	protected:
		bool check_consistency() const;

		void inherit_slots(TypeObject * base);

		void modified();

		bool mro_internal(OBJ * old_mro);

		Error_ update_slot(STR const & name);

	protected:
		template <class Slot> bool slot_defined(TypeObject * base, Slot TypeObject:: * slot) const
		{
			return (this->*slot) && (!base || (this->*slot) != (base->*slot));
		}

		template <class Slot> void copy_val(TypeObject * base, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base) { (this->*slot) = (base->*slot); }
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base && base->slot_defined(basebase, slot)) { (this->*slot) = (base->*slot); }
		}

	public:
		NODISCARD static OBJ type_getattro(TYPE type, OBJ name);

		static Error_ type_setattro(TYPE type, OBJ name, OBJ value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type delete
	template <> struct DefaultDelete<TypeObject> : DefaultDelete<Object> {};

	// type check
#define OBJECT_CHECK_TYPE(o) (ism::typeof(o).has_feature(ism::TypeFlags_Type_Subclass))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type ref
	class TYPE : public Ref<TypeObject>
	{
		REF_COMMON(TYPE, OBJECT_CHECK_TYPE);

	public:
		NODISCARD bool ready() const { return m_ptr->ready(); }

		NODISCARD bool has_feature(TypeFlags_ flag) const { return FLAG_READ(m_ptr->tp_flags, flag); }

		NODISCARD bool is_subtype(TYPE const & value) const { return m_ptr->is_subtype(value); }

		NODISCARD OBJ lookup(OBJ const & name) const { return m_ptr->lookup(name); }

		template <class Name = cstring, class Value = OBJ
		> void add_object(Name && name, Value && value) noexcept
		{
			if (!m_ptr || !m_ptr->tp_dict || !m_ptr->ready()) { return; }

			STR str_name{ FWD(name) };

			((DICT &)m_ptr->tp_dict)[str_name] = FWD(value); // modify tp_dict directly

			m_ptr->modified();

			if (is_dunder_name(str_name)) { m_ptr->update_slot(str_name); }

			ASSERT(m_ptr->check_consistency());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class = std::enable_if_t<is_ref_v<T>>
	> NODISCARD hash_t hash(T const & o) noexcept
	{
		if (!o) { return 0; }
		TYPE t{ typeof(o) };
		return t->tp_hash ? t->tp_hash(o) : 0;
	}

	template <class T, class = std::enable_if_t<is_ref_v<T>>
	> NODISCARD ssize_t len(T const & o) noexcept
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
		return get_dict_ptr(typeof(o), o);
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
		return get_vectorcall_func(typeof(o), o);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_TYPE_OBJECT_HPP_
