#pragma once

#include <setsuna/rtti.h>

/** @file
@brief Header for @ref setsuna::component
*/

namespace setsuna {

class object3d;

/**
@brief Base class for components

Use @ref setsuna::object3d::add_component() to create a component and
add it to an object3d. Usually you would not construct a component directly,
since it won't be added to any object3d actually.

@attention All subclasses should put @p object3d& as its constructors'
first parameter. Skip the first parameter while calling
@ref setsuna::object3d::add_component(), because it's provided automatically.

@see @ref setsuna::object3d
*/
class component {

	RTTI_ENABLE(component)

public:
	/**
	@brief Constructor
	*/
	explicit component(object3d& o3d) :
	    m_object{&o3d} {}

	/**
	@brief Destructor
	*/
	virtual ~component() {}

	/**
	@brief Update every frame

	Do nothing if not overridden.
	*/
	virtual void update() {}

	/**
	@brief Get the object3d this component belongs to
	*/
	object3d& object() const { return *m_object; }

protected:
	object3d* m_object;
};

}  // namespace setsuna
