#pragma once

#include <setsuna/rtti.h>

/** @file
@brief Header for @ref setsuna::resource
*/

namespace setsuna {

/**
@brief Base class for reference counted resources

Wrap resource with @ref setsuna::ref to get automatic lifetime management.

@attention Reference counting is not synchronized because resource instances
are intended to be accessed by the main thread only.
 */
class resource {

	RTTI_ENABLE(resource)

public:
	/**
	@brief Default constructor
	*/
	resource();

	/**
	@brief Copy-constructor
	*/
	resource(const resource&);

	/**
	@brief Moving is not allowed
	*/
	resource(resource&&) = delete;

	resource& operator=(resource&&) = delete;

	/**
	@brief Destructor
	*/
	virtual ~resource();

	/**
	@brief Copy assignment is not allowed
	*/
	resource& operator=(const resource&) = delete;

	/**
	@brief Increase the reference count
	*/
	void ref() const noexcept;

	/**
	@brief Decrease the reference count
	*/
	void unref() const noexcept;

private:
	mutable uint32_t m_ref_count;
};

}  // namespace setsuna
