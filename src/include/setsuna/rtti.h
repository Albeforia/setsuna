#pragma once

#include <type_traits>
#include <array>

/** @file
@brief Custom RTTI functionality

The implementation is inspired by @b RTTR, https://www.rttr.org/ .

Also refer to the blog http://www.axelmenzel.de/articles/rtti for more information.
*/

namespace setsuna {

template<typename... Ts>
struct type_list {
	static constexpr auto length = sizeof...(Ts);
};

using type_id_t = std::size_t;

using type_cast_func = void* (*)(void*);

struct base_class_info {
	constexpr base_class_info() :
	    m_type_id{0}, m_cast_func{nullptr} {}

	constexpr base_class_info(type_id_t id, type_cast_func cast_func) :
	    m_type_id{id}, m_cast_func{cast_func} {}

	type_id_t m_type_id;
	type_cast_func m_cast_func;
};

template<std::size_t N>
using base_list = std::array<base_class_info, N>;

/*
Compile-time string hash using FNV-1a algorithm.
ref: https://gist.github.com/Lee-R/3839813
*/
template<std::size_t N>
constexpr std::size_t hash(const char (&str)[N], std::size_t len = N - 2) {
	return ((len ? hash(str, len - 1) : 2166136261u) ^ str[len]) * 16777619u;
}

template<typename Base, typename... Bases>
constexpr std::size_t base_classes_count_impl() {
	if constexpr (sizeof...(Bases) == 0) {
		return base_classes_count<Base>();
	}
	else {
		return base_classes_count<Base>() + base_classes_count_impl<Bases...>();
	}
}

template<typename... Bases>
constexpr std::size_t base_classes_count_impl(type_list<Bases...>) {
	if constexpr (sizeof...(Bases) == 0) {
		return 0;
	}
	else {
		return type_list<Bases...>::length + base_classes_count_impl<Bases...>();
	}
}

/*
Get the total number of base classes of T in its type hierarchy.
*/
template<typename T>
constexpr std::size_t base_classes_count() {
	return base_classes_count_impl(T::base_classes{});
}

template<typename T, std::size_t N, typename Base, typename... Bases>
constexpr void base_classes_fill_direct(base_list<N>& list, std::size_t index) {
	list[index] = base_class_info(Base::type_id, &type_cast_impl<T, Base>);
	if constexpr (sizeof...(Bases) != 0) {
		base_classes_fill_direct<T, N, Bases...>(list, index + 1);
	}
}

template<typename T, std::size_t N, typename Base, typename... Bases>
constexpr void base_classes_fill(base_list<N>& list, std::size_t index_offset) {
	base_classes_fill<T, N>(Base::base_classes{}, list, index_offset);
	if constexpr (sizeof...(Bases) != 0) {
		base_classes_fill<T, N, Bases...>(list, index_offset + Base::base_classes::length);
	}
}

template<typename T, std::size_t N, typename... Bases>
constexpr void base_classes_fill(type_list<Bases...>,
                                 base_list<N>& list,
                                 std::size_t index_offset) {
	if constexpr (sizeof...(Bases) != 0) {
		// first add direct bases
		base_classes_fill_direct<T, N, Bases...>(list, index_offset);
		// then fill recursively
		base_classes_fill<T, N, Bases...>(list, index_offset + type_list<Bases...>::length);
	}
}

/*
Get a list of all base classes in the type hierarchy of T.
Pass list.size() as template parameter N here to avoid duplicate evaluation of base_classes_count<T>()
*/
template<typename T>
constexpr auto base_classes_list() {
	base_list<base_classes_count<T>()> list{};
	base_classes_fill<T, list.size()>(T::base_classes{}, list, 0);
	return list;
}

/*
A workaround to make the replacement of 'dynamic_cast' possible.
Note {ptr} has to be of dynamic type DerivedType, otherwise the behavior is undefined.
 */
template<typename DerivedType, typename BaseType>
static void* type_cast_impl(void* ptr) {
	return static_cast<void*>(static_cast<BaseType*>(static_cast<DerivedType*>(ptr)));
}

/**
@brief Cast a pointer @p ptr to @p TargetType

@tparam TargetType Target type to cast, must be a pointer type
@tparam SourceType Deduced from @p ptr

@return A pointer of @p TargetType if success, otherwise @p nullptr

Generally you only need to specify @p TargetType manually:

@code{.cpp}
auto p2 = type_cast<target_type*>(p1);
@endcode

@see @ref RTTI_ENABLE()
*/
template<typename TargetType, typename SourceType>
TargetType type_cast(SourceType ptr) {
	// TODO pointers like T** will pass here which is not correct
	static_assert(std::is_pointer_v<TargetType>, "TargetType must be a pointer");
	static_assert(std::is_pointer_v<SourceType>, "SourceType must be a pointer");

	using tgt_no_pointer = std::remove_pointer_t<TargetType>;
	using src_no_pointer = std::remove_pointer_t<SourceType>;

	static_assert((std::is_volatile_v<src_no_pointer> && std::is_volatile_v<tgt_no_pointer>) ||
	                (!std::is_volatile_v<src_no_pointer> && std::is_volatile_v<tgt_no_pointer>) ||
	                (!std::is_volatile_v<src_no_pointer> && !std::is_volatile_v<tgt_no_pointer>),
	              "TargetType must have volatile qualifier");

	static_assert((std::is_const_v<src_no_pointer> && std::is_const_v<tgt_no_pointer>) ||
	                (!std::is_const_v<src_no_pointer> && std::is_const_v<tgt_no_pointer>) ||
	                (!std::is_const_v<src_no_pointer> && !std::is_const_v<tgt_no_pointer>),
	              "TargetType must have const qualifier");

	// same static type, no need to cast
	if (tgt_no_pointer::type_id == src_no_pointer::type_id ||
	    ptr == nullptr) return reinterpret_cast<TargetType>(ptr);

	auto ptr_no_const = const_cast<std::remove_const_t<src_no_pointer>*>(ptr);

	// same dynamic type (the most derived type of src is same as TargetType), downcast
	if (tgt_no_pointer::type_id == ptr->dynamic_type()) {
		return static_cast<TargetType>(static_cast<void*>(ptr_no_const));
	}

	// otherwise try to cast src to its dynamic type then upcast to TargetType if possible
	auto base_classes = ptr->dynamic_base_classes();
	auto base_arr = std::get<0>(base_classes);
	auto length = std::get<1>(base_classes);
	for (std::size_t i = 0; i < length; ++i) {
		if (tgt_no_pointer::type_id == base_arr[i].m_type_id) {
			return static_cast<TargetType>(base_arr[i].m_cast_func(ptr_no_const));
		}
	}

	return nullptr;
}

#define STRINGIFY(x) #x
#define SETSUNA_CAT_(x, y) STRINGIFY(x##y)
#define SETSUNA_CAT(x, y) SETSUNA_CAT_(x, y)

/**
@brief Enable custom RTTI mechanism for @p type

If you want to enable custom RTTI for @p some_class, for example,
declare it like this:

@code{.cpp}
class some_class : public base_a, public base_b {
	RTTI_ENABLE(some_class, base_a, base_b)
};
@endcode

Then you could do dynamic casting via @ref setsuna::type_cast() .

@attention Enabling custom RTTI for a class requires all its base classes have custom RTTI enabled too.
*/
#define RTTI_ENABLE(type, ...)                                                               \
public:                                                                                      \
	using base_classes = setsuna::type_list<__VA_ARGS__>;                                    \
	static constexpr setsuna::type_id_t type_id = setsuna::hash(SETSUNA_CAT(setsuna, type)); \
	static constexpr auto base_classes_info = setsuna::base_classes_list<typename type>();   \
	virtual setsuna::type_id_t dynamic_type() const { return type_id; }                      \
	virtual std::tuple<const setsuna::base_class_info*, std::size_t>                         \
	dynamic_base_classes() const {                                                           \
		return std::make_tuple(base_classes_info.data(), base_classes_info.size());          \
	}

}  // namespace setsuna
