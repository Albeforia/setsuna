#pragma once

#include <setsuna/visitor.h>

/** @file
@brief Header for @ref setsuna::update_visitor
*/

namespace setsuna {

/**
@brief Visitor responsible for updating the scene graph
*/
class update_visitor : public visitor {

public:
	/**
	@brief Default constructor
	*/
	update_visitor();

	/**
	@brief Visit an object3d

	Calculate the global transform matrix of the object3d, then
	call @ref setsuna::component::update() of every components on it.
	*/
	void apply(object3d&) override;
};

}  // namespace setsuna
