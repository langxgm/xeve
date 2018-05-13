//------------------------------------------------------------------------
// * @filename: MongoElement.h
// *
// * @brief: 获取文档中元素的值
// *
// * @author: XGM
// * @date: 2017/12/27
//------------------------------------------------------------------------
#pragma once

#include <bsoncxx/document/element.hpp>
#include <bsoncxx/types.hpp>

#include <assert.h>

using namespace bsoncxx::types;

template<class _Element_t, class _Value_t>
struct MongoElement_t
{
	using element_type = _Element_t;
	using value_type = _Value_t;
};

template<class _Element_t>
struct MongoElement
{
	template<class _ElementWrap_t>
	static void GetValue(const _ElementWrap_t& e)
	{
		static_assert(std::is_void<_Element_t>::value, "Undefined element type");
		return;
	}
};

template<>
struct MongoElement<b_bool> : MongoElement_t<b_bool, bool>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_bool().value;
		}
		return 0;
	}
};

template<>
struct MongoElement<b_int32> : MongoElement_t<b_int32, int32_t>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_int32().value;
		}
		return 0;
	}
};

template<>
struct MongoElement<b_int64> : MongoElement_t<b_int64, int64_t>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_int64().value;
		}
		return 0;
	}
};

template<>
struct MongoElement<b_double> : MongoElement_t<b_double, double>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_double().value;
		}
		return 0;
	}
};

template<>
struct MongoElement<b_date> : MongoElement_t<b_date, int64_t>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_date().to_int64();
		}
		return 0;
	}
};

template<>
struct MongoElement<b_binary> : MongoElement_t<b_binary, bsoncxx::stdx::string_view>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return value_type((const char*)(e.get_binary().bytes), e.get_binary().size);
		}
		return "";
	}
};

template<>
struct MongoElement<b_utf8> : MongoElement_t<b_utf8, bsoncxx::stdx::string_view>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_utf8().value;
		}
		return "";
	}
};

template<>
struct MongoElement<b_oid> : MongoElement_t<b_oid, bsoncxx::oid>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_oid().value;
		}
		return {};
	}
};

template<>
struct MongoElement<b_array> : MongoElement_t<b_array, bsoncxx::array::view>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_array().value;
		}
		return {};
	}
};

template<>
struct MongoElement<b_document> : MongoElement_t<b_document, bsoncxx::document::view>
{
	template<class _ElementWrap_t>
	static value_type GetValue(const _ElementWrap_t& e)
	{
		assert(!e || e.type() == element_type::type_id);
		if (e && e.type() == element_type::type_id)
		{
			return e.get_document().value;
		}
		return {};
	}
};
