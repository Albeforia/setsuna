#pragma once

#include <queue>
#include <mutex>

/** @file
@brief Header for @ref setsuna::shared_queue
*/

namespace setsuna {

/**
@brief Thread-safe queue
*/
template<typename T>
class shared_queue {

public:
	/**
	@brief Default constructor
	*/
	shared_queue() = default;

	/**
	@brief Copying is not allowed
	*/
	shared_queue(const shared_queue&) = delete;

	shared_queue& operator=(const shared_queue&) = delete;

	/**
    @brief Push the given element to the end of the queue
    */
	void push(const T& e) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(e);
	}

	/**
    @brief Push the given element to the end of the queue
    */
	void push(T&& e) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(std::move(e));
	}

	/**
    @brief Construct an element at the end of the queue

	@return The inserted element
    */
	template<class... Args>
	decltype(auto) emplace(Args&&... args) {
		//return m_queue.emplace(std::forward<Args>(args)...);
		return m_queue.emplace(args...);
	}

	/**
    @brief Try to pop an element from the front of the queue

	@param e An element to receive the popped element
    
	@return @p false if the queue is empty, otherwise @p true
    */
	bool try_pop(T& e) {
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_queue.empty()) return false;

		e = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	/**
    @brief Try to pop an element from the front of the queue
    
	@return @p false if the queue is empty, otherwise @p true
    */
	bool try_pop() {
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_queue.empty()) return false;

		m_queue.pop();
		return true;
	}

	/**
    @brief Check if the queue is empty
    */
	bool empty() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
};

}  // namespace setsuna
