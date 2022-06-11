#ifndef _ISM_JSON_HPP_
#define _ISM_JSON_HPP_

#include <core/typedefs.hpp>

#include <nlohmann/json.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(JSON) nlohmann::basic_json
	<
		std::map,
		std::vector, std::string, bool, int64_t, uint64_t, double_t,
		std::allocator,
		nlohmann::adl_serializer,
		std::vector<uint8_t>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_JSON_HPP_
