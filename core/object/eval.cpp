#include <core/object/eval.hpp>

namespace Ism
{
	Var evaluate(String text)
	{
		if ((text = text.trim()).empty()) { return nullptr; }
		else if (util::is_quote(text)) { return text.trim([](auto c) { return c == '\'' || c == '\"'; }); }
		else if (auto const o{ util::to_i64(text) }) { return *o; }
		else if (auto const o{ util::to_f64(text) }) { return *o; }
		else if (auto const o{ util::to_bool(text) }) { return *o; }
		else { return text; }
	}
}