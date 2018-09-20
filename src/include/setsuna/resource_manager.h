#pragma once

#include <setsuna/loader.h>

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
