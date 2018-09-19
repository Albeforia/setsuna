#pragma once

/** @file
@brief Header for @ref setsuna::ref
*/

namespace setsuna {

/**
@brief Pointer wrapper for reference counted objects

@tparam T The type of the pointer being wrapped, required to implement
@p ref() and @p unref() appropriately

If you want to share something among a few threads, @p std::shared_ptr is a
good choice. However ownership sharing inside one thread needs no synchronization,
that's why this lightweight pointer wrapper is implemented. Besides @p std::shared_ptr
does not offer any guarantee that the instance it holds is thread-safe.
*/
template<typename T>
class ref {

public:
	/**
	@brief Default constructor, holding @p nullptr
	*/
	ref() :
	    m_ptr{nullptr} {}

	/**
	@brief Copy-constructor
	*/
	ref(const ref& other) :
	    m_ptr{other.m_ptr} {
		if (m_ptr) m_ptr->ref();
	}

	/**
	@brief Construct from a raw pointer @p ptr
	*/
	ref(T* ptr) :
	    m_ptr{ptr} {
		if (m_ptr) m_ptr->ref();
	}

	/**
	@brief Move-constructor
	*/
	ref(ref&& other) noexcept :
	    m_ptr{other.m_ptr} {
		other.m_ptr = nullptr;
	}

	/**
	@brief Destructor
	*/
	~ref() {
		if (m_ptr) m_ptr->unref();
	}

	/**
	@brief Copy assignment
	*/
	ref& operator=(const ref& other) {
		if (m_ptr == other.m_ptr) return *this;
		if (m_ptr) m_ptr->unref();
		m_ptr = other.m_ptr;
		if (m_ptr) m_ptr->ref();
		return *this;
	}

	/**
	@brief Copy from a raw pointer @p ptr
	*/
	ref& operator=(T* ptr) {
		if (m_ptr == ptr) return *this;
		if (m_ptr) m_ptr->unref();
		m_ptr = ptr;
		if (m_ptr) m_ptr->ref();
		return *this;
	}

	/**
	@brief Move assignment
	*/
	ref& operator=(ref&& other) noexcept {
		if (this != &other) {
			if (m_ptr) m_ptr->unref();
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
		}
		return *this;
	}

	/**
	@brief Comparison
	*/
	bool operator==(const ref& other) const { return (m_ptr == other.m_ptr); }

	bool operator!=(const ref& other) const { return !(*this == other); }

	/**
	@brief Compare with a raw pointer @p ptr
	*/
	bool operator==(const T* ptr) const { return (m_ptr == ptr); }

	bool operator!=(const T* ptr) const { return !(*this == ptr); }

	/**
	@brief Validity, i.e. whether holding @p nullptr or not
	*/
	explicit operator bool() const {
		return m_ptr != nullptr;
	}

	/**
	@brief Pointer member access
	*/
	T* operator->() { return m_ptr; }

	/**
	@brief Pointer member access
	*/
	const T* operator->() const { return m_ptr; }

	/**
	@brief Get the raw pointer
	*/
	const T* get() const { return m_ptr; }

	/**
	@brief Get the raw pointer
	*/
	T* get() { return m_ptr; }

private:
	T* m_ptr;
};

}  // namespace setsuna
