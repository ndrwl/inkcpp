#include "value.h"
#include "output.h"
#include "list_table.h"
#include "string_utils.h"

namespace ink::runtime::internal
{
#ifdef INK_ENABLE_STL
	template<value_type ty = value_type::PRINT_BEGIN>
	void append(std::ostream& os, const value& val, const list_table* lists) {
		if constexpr (ty != value_type::PRINT_END) {
			if (ty == val.type()) {
				os << val.get<ty>();
			} else {
				append<ty+1>(os, val, lists);
			}
		}
	}

	template<>
	void append<value_type::list_flag>(std::ostream& os, const value& val,
			const list_table* lists) {
		if (val.type() == value_type::list_flag) {
			inkAssert(lists, "to stringify lists, we need a list_table");
			os << lists->toString(val.get<value_type::list_flag>());
		} else {
			append<value_type::list_flag + 1>(os, val, lists);
		}
	}
	template<>
	void append<value_type::list>(std::ostream& os, const value& val,
			const list_table* lists) {
		if (val.type() == value_type::list) {
			inkAssert(lists, "to stringify lists, we need a list_table");
			lists->write(os, val.get<value_type::list>());
		} else {
			append<value_type::list +1>(os, val, lists);
		}
	}
	template<>
	void append<value_type::float32>(std::ostream& os, const value& val, const list_table* lists) {
		if(val.type() == value_type::float32) {
			char number[32];
			ink::runtime::internal::toStr(number, 32, val.get<value_type::float32>());
			os << number;
		} else {
			append<value_type::float32+1>(os, val, lists);
		}
	}
	template<>
	void append<value_type::boolean>(std::ostream& os, const value& val, const list_table* lists) {
		if(val.type() == value_type::boolean) {
			os << (val.get<value_type::boolean>() ? "true" : "false");
		} else {
			append<value_type::boolean + 1>(os, val, lists);
		}
	}

	std::ostream& value::write(std::ostream& os, const list_table* lists) const {
		if (type() < value_type::PRINT_BEGIN || type() >= value_type::PRINT_END) {
			throw ink_exception("printing this type is not supported");
		}
		append(os, *this, lists);
		return os;
	}
#endif
#ifdef INK_ENABLE_UNREAL
	FStringBuilderBase& value::write(FStringBuilderBase& stringbuilder, const list_table* lists) const
	{
		if (type() < value_type::PRINT_BEGIN || type() >= value_type::PRINT_END) {
			throw ink_exception("printing this type is not supported");
		}

		switch (type())
		{
		case value_type::boolean:
			stringbuilder.Append(get<value_type::boolean>() ? TEXT("true") : TEXT("false"));
			break;
		case value_type::uint32:
			stringbuilder << get<value_type::uint32>();
			break;
			
		case value_type::int32:
			stringbuilder << get<value_type::int32>();
			break;
			
		case value_type::float32:
			stringbuilder.Appendf(TEXT("%f"), get<value_type::float32>());
			break;
			
		case value_type::list:
			inkAssert(lists, "to stringify lists, we need a list_table");
			lists->write(stringbuilder, get<value_type::list>());
			break;
			
		case value_type::list_flag:
			inkAssert(lists, "to stringify lists, we need a list_table");
			stringbuilder << lists->toString(get<value_type::list_flag>());
			break;
		
		case value_type::string:
			stringbuilder.Append(get<value_type::string>().str);
			break;
			
		case value_type::newline:
			stringbuilder << get<value_type::newline>();
			break;
			
		case value_type::value_pointer:
		default: 
			// unsuported
			break;
		}
		
		return stringbuilder;
	}
#endif

	basic_stream& operator<<(basic_stream& os, const value& val) {
		os.append(val);
		return os;
	}
}
