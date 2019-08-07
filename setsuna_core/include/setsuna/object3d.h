#pragma once

#include <setsuna/component.h>
#include <setsuna/transform.h>
#include <setsuna/visitor.h>
#include <vector>
#include <algorithm>

/** @file
@brief Header for @ref setsuna::object3d
*/

namespace setsuna {

/**
@brief How a @ref setsuna::object3d is positioned
*/
enum class positioning_type {
	ABSOLUTE, /**< @brief Position using world coordinate */
	RELATIVE  /**< @brief Position relative to the parent, default option */
};

/**
@brief Node(3D) in the scene graph

@see @ref setsuna::component @ref setsuna::visitor
*/
class object3d {

public:
	/**
	@brief Default constructor
	*/
	object3d();

	/**
	@brief Destructor
	*/
	~object3d();

	/**
	@brief Copying is not allowed
	*/
	object3d(const object3d&) = delete;

	object3d& operator=(const object3d&) = delete;

	/**
	@brief Add a new child

	@return The new child added
	*/
	object3d& add_child(positioning_type pt = positioning_type::RELATIVE);

	/**
	@brief Add an existing object3d as a child

	The object3d being added will be detached from its old parent if necessary.
	*/
	void add_child(object3d*);

	/**
	@brief Detach from the parent
	*/
	void detach();

	/**
	@brief Delete all children
	*/
	void clear_children();

	/**
	@brief Get an iterator to the first child
	*/
	auto children_begin() noexcept { return m_children.begin(); }

	/**
	@brief Get an iterator to the off-the-end child
	*/
	auto children_end() noexcept { return m_children.end(); }

	/**
	@brief Accept a visitor
	*/
	void accept(visitor&);

#pragma region component_query

	/**
	@brief Add a new component of type @p component_t to this object3d

	@param params Parameters to construct a component of type @p component_t

	@return The new component added
	*/
	template<class component_t, typename... args>
	auto& add_component(args&&... params) {
		return *static_cast<component_t*>(
		  m_components.emplace_back(new component_t(*this, params...)));
	}

	/**
	@brief Get a component of type @p component_t from this object3d

	@return The first component that matches the type @p component_t,
	nullptr if no such component exists
	*/
	template<class component_t>
	component_t* get_component() const {
		for (auto& component : m_components) {
			auto target = type_cast<component_t*>(component);
			if (target) return target;
		}

		return nullptr;
	}

	/**
	@brief Get all components of type @p component_t from this object3d

	@return A vector of components that match the type @p component_t
	*/
	template<class component_t>
	std::vector<component_t*> get_components() const {
		std::vector<component_t*> required_components;

		for (auto& component : m_components) {
			auto target = type_cast<component_t*>(component);
			if (target) required_components.push_back(target);
		}

		return required_components;
	}

	/**
	@brief Remove the first component that matches the type @p component_t

	@return @p false if no such component exists, otherwise @p true
	*/
	template<class component_t>
	bool remove_component() {
		if (m_components.empty()) return false;

		auto& index = std::find_if(m_components.begin(),
		                           m_components.end(),
		                           [](auto& component) {
			                           return type_cast<component_t*>(component) != nullptr;
		                           });

		bool success = index != m_components.end();

		if (success) {
			delete *index;
			m_components.erase(index);
		}

		return success;
	}

	/**
	@brief Remove all components that match the type @p component_t

	@return The number of successful removals
	*/
	template<class component_t>
	int remove_components() {
		if (m_components.empty()) return 0;

		int removed_count = 0;
		bool success = false;

		do {
			auto& index = std::find_if(m_components.begin(),
			                           m_components.end(),
			                           [](auto& component) {
				                           return type_cast<component_t*>(component) != nullptr;
			                           });

			success = index != m_components.end();

			if (success) {
				delete *index;
				m_components.erase(index);
				++removed_count;
			}
		} while (success);

		return removed_count;
	}

#pragma endregion

	transform local_transform; /**< @brief The local transform */

	positioning_type positioning; /**< @brief The positioning type */

	/**
	@brief Get the global transform matrix

	If the object3d has no parent or the @ref #positioning is @ref positioning_type::ABSOLUTE,
	then it's identical to the local transform matrix.
	*/
	DirectX::XMMATRIX world_matrix() const { return XMLoadFloat4x4(&m_world_matrix); }

	/**
	@brief Set the global transform matrix
	*/
	void set_world_matrix(const DirectX::XMMATRIX& m) {
		DirectX::XMStoreFloat4x4(&m_world_matrix, m);
	}

	/**
	@brief Get the parent
	*/
	object3d* parent() const { return m_parent; }

private:
	object3d* m_parent;

	std::vector<object3d*> m_children;

	std::vector<component*> m_components;

	DirectX::XMFLOAT4X4 m_world_matrix;
};

}  // namespace setsuna
