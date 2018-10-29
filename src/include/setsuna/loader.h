#pragma once

#include <setsuna/resource.h>
#include <setsuna/ref.h>
#include <functional>
#include <vector>

/** @file
@brief Header for @ref setsuna::loader
*/

namespace setsuna {

/**
@brief Base class for resource loaders

Loading a resource involves two threads. The main thread is where rendering happens,
while the sub-thread is where loading i.e. I/O happens. OpenGL API can only be called
in the main thread, so generally the sub-thread is responsible to prepare the data
for the main thread to create OpenGL objects. Of course, loader should be carefully
designed so that there won't be any racing condition between the two threads.
*/
class loader {

	RTTI_ENABLE(loader)

	friend class resource_manager;

public:
	/**
	@brief 
	*/
	using callback_t = std::function<void(ref<resource>)>;

	/**
	@brief Default constructor
	*/
	loader() = default;

	/**
	@brief Destructor
	*/
	virtual ~loader() {}

	/**
	@brief Create the resource

	This is called in the main thread before loading happens.

	A fallback value of the specific resource type can be set here so that
	a resource could be used before it is fully loaded. Pre-loading check
	could also be performed here.
	*/
	virtual void create_resource() = 0;

	/**
	@brief Load the resource

	This is called in the sub-thread.
	You can load resource files here without interfering the main thread.

	@attention Do not call any OpenGL functions from here.
	*/
	virtual void sub_thread_stage() = 0;

	/**
	@brief Prepare the resource for rendering

	This is called in the main thread after @ref sub_thread_stage() completes.
	You can create OpenGL objects here using the loaded resources.
	*/
	virtual void main_thread_stage() = 0;

	/**
	@brief Indicate if two loaders load the same resource
	*/
	virtual bool match(const loader&) const = 0;

	/**
	@brief Get the loading result
	*/
	virtual ref<resource> get() const = 0;

private:
	std::vector<callback_t> m_callbacks;
};

}  // namespace setsuna
