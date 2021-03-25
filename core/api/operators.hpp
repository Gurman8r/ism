#ifndef _ISM_OPERATORS_HPP_
#define _ISM_OPERATORS_HPP_

#include <core/api/runtime.hpp>

namespace ISM::API
{
	struct undefined_t {};
	
	struct self_t {};
	
	static const self_t self{};
	
	inline self_t __self() { return self; }

	enum op_id : int32_t
	{
		op_add, op_sub, op_mul, op_div, op_mod, op_divmod, op_pow, op_lshift,
		op_rshift, op_and, op_xor, op_or, op_neg, op_pos, op_abs, op_invert,
		op_int, op_long, op_float, op_str, op_cmp, op_gt, op_ge, op_lt, op_le,
		op_eq, op_ne, op_iadd, op_isub, op_imul, op_idiv, op_imod, op_ilshift,
		op_irshift, op_iand, op_ixor, op_ior, op_complex, op_bool, op_nonzero,
		op_repr, op_truediv, op_itruediv, op_hash
	};

	enum op_type : int32_t
	{
		op_l, /* base type on left */
		op_r, /* base type on right */
		op_u  /* unary operator */
	};

	template <op_id, op_type, class B, class L, class R> struct op_impl {};

	template <op_id id, op_type ot, class L, class R> struct op_
	{
		template <class C, class ... Extra> void execute(C & cl, Extra && ... extra) const
		{
			using Base = class C::type;
			using L_type = std::conditional_t<std::is_same_v<L, self_t>, Base, L>;
			using R_type = std::conditional_t<std::is_same_v<R, self_t>, Base, R>;
			using op = op_impl<id, ot, Base, L_type, R_type>;
			cl.def(op::name(), &op::execute, is_operator(), FWD(extra)...);
		}
		template <class C, class ... Extra> void execute_cast(C & cl, Extra && ... extra) const
		{
			using Base = class C::type;
			using L_type = std::conditional_t<std::is_same_v<L, self_t>, Base, L>;
			using R_type = std::conditional_t<std::is_same_v<R, self_t>, Base, R>;
			using op = op_impl<id, ot, Base, L_type, R_type>;
			cl.def(op::name(), &op::execute_cast, is_operator(), FWD(extra)...);
		}
	};

#define ISM_BINARY_OPERATOR(id, rid, op, expr)													\
	template <class B, class L, class R> struct op_impl<op_##id, op_l, B, L, R> {				\
		static char const* name() { return "__" #id "__"; }										\
		static auto execute(L const &lhs, R const &rhs) -> decltype(expr) { return (expr); }	\
		static B execute_cast(L const &lhs, R const &rhs) { return B(expr); }					\
	};																							\
	template <class B, class L, class R> struct op_impl<op_##id, op_r, B, L, R> { \
		static char const* name() { return "__" #rid "__"; }									\
		static auto execute(R const &rhs, L const &lhs) -> decltype(expr) { return (expr); }	\
		static B execute_cast(R const &rhs, L const &lhs) { return B(expr); }					\
	};																							\
	inline op_<op_##id, op_l, self_t, self_t> op(self_t const &, self_t const &) {				\
		return op_<op_##id, op_l, self_t, self_t>();											\
	}																							\
	template <class T> op_<op_##id, op_l, self_t, T> op(self_t const &, T const &) {			\
		return op_<op_##id, op_l, self_t, T>();													\
	}																							\
	template <class T> op_<op_##id, op_r, T, self_t> op(T const &, self_t const &) {			\
		return op_<op_##id, op_r, T, self_t>();													\
	}

#define ISM_INPLACE_OPERATOR(id, op, expr)														\
	template <class B, class L, class R> struct op_impl<op_##id, op_l, B, L, R> {				\
		static char const* name() { return "__" #id "__"; }										\
		static auto execute(L &lhs, R const &rhs) -> decltype(expr) { return expr; }			\
		static B execute_cast(L &lhs, R const &rhs) { return B(expr); }							\
	};																							\
	template <class T> op_<op_##id, op_l, self_t, T> op(self_t const &, T const &) {			\
		return op_<op_##id, op_l, self_t, T>();													\
	}

#define ISM_UNARY_OPERATOR(id, op, expr)														\
	template <class B, class L> struct op_impl<op_##id, op_u, B, L, undefined_t> {				\
		static char const* name() { return "__" #id "__"; }										\
		static auto execute(L const &lhs) -> decltype(expr) { return expr; }					\
		static B execute_cast(L const &lhs) { return B(expr); }									\
	};																							\
	inline op_<op_##id, op_u, self_t, undefined_t> op(self_t const &) {							\
		return op_<op_##id, op_u, self_t, undefined_t>();										\
	}

	ISM_BINARY_OPERATOR(sub,		rsub,		operator-,		lhs - rhs)
	ISM_BINARY_OPERATOR(add,		radd,		operator+,		lhs + rhs)
	ISM_BINARY_OPERATOR(mul,		rmul,		operator*,		lhs * rhs)
	ISM_BINARY_OPERATOR(div,		rdiv,		operator/,		lhs / rhs)
	ISM_BINARY_OPERATOR(mod,		rmod,		operator%,		lhs % rhs)
	ISM_BINARY_OPERATOR(lshift,		rlshift,	operator<<,		lhs << rhs)
	ISM_BINARY_OPERATOR(rshift,		rrshift,	operator>>,		lhs >> rhs)
	ISM_BINARY_OPERATOR(and,		rand,		operator&,		lhs & rhs)
	ISM_BINARY_OPERATOR(xor,		rxor,		operator^,		lhs ^ rhs)
	ISM_BINARY_OPERATOR(eq,			eq,			operator==,		lhs == rhs)
	ISM_BINARY_OPERATOR(ne,			ne,			operator!=,		lhs != rhs)
	ISM_BINARY_OPERATOR(or,			ror,		operator|,		lhs | rhs)
	ISM_BINARY_OPERATOR(gt,			lt,			operator>,		lhs > rhs)
	ISM_BINARY_OPERATOR(ge,			le,			operator>=,		lhs >= rhs)
	ISM_BINARY_OPERATOR(lt,			gt,			operator<,		lhs < rhs)
	ISM_BINARY_OPERATOR(le,			ge,			operator<=,		lhs <= rhs)

	ISM_INPLACE_OPERATOR(iadd,		operator+=,		lhs += rhs)
	ISM_INPLACE_OPERATOR(isub,		operator-=,		lhs -= rhs)
	ISM_INPLACE_OPERATOR(imul,		operator*=,		lhs *= rhs)
	ISM_INPLACE_OPERATOR(idiv,		operator/=,		lhs /= rhs)
	ISM_INPLACE_OPERATOR(imod,		operator%=,		lhs %= rhs)
	ISM_INPLACE_OPERATOR(ilshift,	operator<<=,	lhs <<= rhs)
	ISM_INPLACE_OPERATOR(irshift,	operator>>=,	lhs >>= rhs)
	ISM_INPLACE_OPERATOR(iand,		operator&=,		lhs &= rhs)
	ISM_INPLACE_OPERATOR(ixor,		operator^=,		lhs ^= rhs)
	ISM_INPLACE_OPERATOR(ior,		operator|=,		lhs |= rhs)
	ISM_UNARY_OPERATOR(neg,			operator-,		-lhs)
	ISM_UNARY_OPERATOR(pos,			operator+,		+lhs)

	ISM_UNARY_OPERATOR(abs,		abs,		std::abs(lhs))
	ISM_UNARY_OPERATOR(hash,	hash,		Hasher<L>(lhs))
	ISM_UNARY_OPERATOR(invert,	operator~,	(~lhs))
	ISM_UNARY_OPERATOR(bool,	operator!,	!!lhs)
	ISM_UNARY_OPERATOR(int,		integer_,	(int32_t)lhs)
	ISM_UNARY_OPERATOR(float,	decimal_,	(double_t)lhs)
}

#endif // !_ISM_OPERATORS_HPP_
