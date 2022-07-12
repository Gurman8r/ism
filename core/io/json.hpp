#ifndef _ISM_JSON_HPP_
#define _ISM_JSON_HPP_

#include <core/typedefs.hpp>

#include <nlohmann/json.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(Json) nlohmann::basic_json
	<
		std::map,
		std::vector,
		std::string,
		bool, i64, u64, f64,
		std::allocator,
		nlohmann::adl_serializer,
		std::vector<byte>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_JSON_HPP_
