#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <queue>
#include <vector>

/** @file
@brief Header for @ref setsuna::resource_manager
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

public:
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
};

using loader_ptr = std::shared_ptr<loader>;

/**
@brief Asynchronous resource loading manager

The basic idea is similar to that of the ResLoader of @b KlayGE .

@see @ref setsuna::loader
*/
class resource_manager {

public:
	/**
	@brief Get the resource manager singleton
	*/
	static resource_manager& instance() {
		static resource_manager _instance;
		return _instance;
	}

	/**
	@brief Destructor
	*/
	~resource_manager();

	resource_manager(const resource_manager&) = delete;
	resource_manager& operator=(const resource_manager&) = delete;

	/**
	@brief Load resource using a loader of type @p loader_t

	@param params Parameters to construct a loader of type @p loader_t
	*/
	template<typename loader_t, typename... args>
	auto load(args&&... params) {
		loader_ptr loader = std::make_shared<loader_t>(params...);
		load(loader);
		return loader;
	}

	/**
	@brief Update loading status
	*/
	void update();

	/**
	@brief Stop loading and release all loaded resources
	*/
	void shutdown();

private:
	resource_manager();

	void loading_func();

	void load(loader_ptr&);

private:
	enum class loading_status {
		LS_LOADING,
		LS_COMPLETE,
	};

	/*
	shared_ptr is used because load_pair is shared between m_load_requests and m_loading_res.
	loading_status is atomic since it is modified by sub-thread and read by main thread.
	*/
	using load_pair = std::pair<loader_ptr, std::shared_ptr<std::atomic<loading_status>>>;

	std::unique_ptr<std::thread> m_loading_thread;
	std::atomic<bool> m_stop;

	// filled by main thread and consumed by sub-thread
	std::queue<load_pair> m_load_requests;
	std::mutex m_load_requests_mutex;

	// resources that have not finished loading, only accessed by main thread
	std::vector<load_pair> m_loading_res;

	// resources that have finished loading, only accessed by main thread
	std::vector<std::weak_ptr<loader>> m_loaded_res;
};

}  // namespace setsuna
