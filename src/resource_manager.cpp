#include <setsuna/rtti_prefix.h>
#include <setsuna/resource_manager.h>

namespace setsuna {

resource_manager::resource_manager() :
    m_stop{false} {
	m_loading_thread = std::make_unique<std::thread>(&resource_manager::loading_func, this);
}

resource_manager::~resource_manager() {}

void resource_manager::shutdown() {
	m_stop = true;
	m_loading_thread->join();

	while (!m_load_requests.empty()) {
		m_load_requests.pop();
	}

	m_loading_res.clear();
	m_loaded_res.clear();
}

void resource_manager::load(loader_ptr& loader) {
	// check if already loaded
	for (const auto& loaded_loader : m_loaded_res) {
		auto ptr = loaded_loader.lock();
		if (ptr && ptr->match(*loader)) {
			loader = ptr;
			return;
		}
	}

	// check if is loading
	for (const auto& pair : m_loading_res) {
		auto& loading_loader = pair.first;
		if (loading_loader->match(*loader)) {
			loader = loading_loader;
			return;
		}
	}

	// new loading requests
	loader->create_resource();
	auto status = std::make_shared<std::atomic<loading_status>>(loading_status::LS_LOADING);
	m_loading_res.emplace_back(loader, status);
	{
		std::lock_guard<std::mutex> lock(m_load_requests_mutex);
		m_load_requests.emplace(loader, status);
	}
}

void resource_manager::update() {
	// remove unreferenced loaders
	for (auto it = m_loaded_res.begin(); it != m_loaded_res.end();) {
		if (it->lock()) {
			++it;
		}
		else {
			it = m_loaded_res.erase(it);
		}
	}

	// find resources that finish loading,
	// remove from m_loading_res, add to m_loaded_res
	for (auto it = m_loading_res.begin(); it != m_loading_res.end();) {
		if (it->second->load() == loading_status::LS_COMPLETE) {
			auto& loader = it->first;
			loader->main_thread_stage();
			m_loaded_res.emplace_back(loader);

			it = m_loading_res.erase(it);
		}
		else {
			++it;
		}
	}
}

void resource_manager::loading_func() {
	while (!m_stop) {

		m_load_requests_mutex.lock();

		while (!m_stop && !m_load_requests.empty()) {
			auto pair = m_load_requests.front();
			m_load_requests.pop();

			// now we can release the lock and let other threads access the queue
			m_load_requests_mutex.unlock();

			if (pair.second->load() == loading_status::LS_LOADING) {
				pair.first->sub_thread_stage();
				pair.second->store(loading_status::LS_COMPLETE);
			}

			// lock for the next loop
			m_load_requests_mutex.lock();
		}

		m_load_requests_mutex.unlock();
	}
}

}  // namespace setsuna
