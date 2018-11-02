#pragma once

#include <setsuna/texture.h>
#include <setsuna/ref.h>
#include <map>
#include <vector>

/** @file
@brief Header for @ref setsuna::texture_manager
*/

namespace setsuna {

/**
@brief Texture manager
*/
class texture_manager {

public:
	/**
	@brief Texture manager option

	Default values: @p sparse=true , @p max_layers=32 , @p max_layers_sparse=1024
	*/
	struct option {
		bool sparse;                /**< @brief Whether to use sparse texture array */
		uint32_t max_layers;        /**< @brief Max number of layers per texture array */
		uint32_t max_layers_sparse; /**< @brief Max number of layers per sparse texture array */
	};

public:
	/**
	@brief Get the texture manager singleton
	*/
	static texture_manager& instance() {
		static texture_manager _instance;
		return _instance;
	}

	/**
	@brief Destructor
	*/
	~texture_manager();

	texture_manager(const texture_manager&) = delete;
	texture_manager& operator=(const texture_manager&) = delete;

	/**
	@brief Create a new texture
	*/
	ref<texture> new_texture(const texture_description&);

	/**
	@brief Get the fallback texture

	The fallback texture is a pure white 1x1 texture.
	*/
	ref<texture> default_texture() const { return *m_default_texture; }

	/**
	@brief Set the option

	The new setting will take effect next time the manager creates a new
	texture array.

	@attention Keep @ref option::max_layers low if not using sparse texture array,
	because the memory for all layers is allocated even if only one texture is required.
	*/
	void set_option(const option& opt) {
		m_option = opt;
	}

	/*
	Only called by texture
	*/
	void assign_unit(texture_unit, texture_container&);

private:
	std::tuple<texture_container*, texture_layer>
	next_free_layer(const texture_description&);

	texture_unit next_texture_unit() const;

private:
	texture_manager();

	std::map<texture_description, std::vector<texture_container*>> m_containers;

	// keep track of which texture_container is using which texture_unit
	std::map<texture_unit, texture_container*> m_units_assignments;

	// keep a reference of default texture in case that it is automatically destroyed
	ref<texture>* m_default_texture;

	GLsizei m_max_texture_units;

	option m_option;
};

// for key comparison
bool operator<(const texture_description&, const texture_description&);

}  // namespace setsuna
