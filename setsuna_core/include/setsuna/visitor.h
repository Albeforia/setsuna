#pragma once

/** @file
@brief Header for @ref setsuna::visitor
*/

namespace setsuna {

/**
@brief How a @ref setsuna::visitor traverses the scene graph
*/
enum class traversal_mode {
	CURRENT, /**< @brief Traverse current node only */
	CHILDREN /**< @brief Traverse all children */
};

class object3d;

/**
@brief Base class for visitors

This class and @ref setsuna::object3d use double dispatch to implement
the visitor design pattern.

@see @ref setsuna::object3d::accept()
*/
class visitor {

public:
	/**
	@brief Constructor
	*/
	explicit visitor(traversal_mode m) :
	    mode{m} {}

	/**
	@brief Destructor
	*/
	virtual ~visitor() {}

	/**
	@brief Visit an object3d
	*/
	virtual void apply(object3d&) = 0;

	traversal_mode mode; /**< @brief The traversal mode */
};

}  // namespace setsuna
