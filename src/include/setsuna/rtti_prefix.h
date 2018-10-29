/*
RTTI_ENABLE() defines two static constexpr properties which will be evaluated repeatedly
if the header is included by different translation units. This will slow the compilation 
speed unnecessarily. Precompiled header could be used to avoid that.

For any class that uses RTTI_ENABLE(), put its header file below.
For any cpp file that include header files below, include this prefix header instead.
*/

#include <setsuna/component.h>
#include <setsuna/camera.h>
#include <setsuna/mesh_filter.h>
#include <setsuna/mesh_renderer.h>

#include <setsuna/resource.h>
#include <setsuna/mesh.h>
#include <setsuna/texture.h>
#include <setsuna/material_instance.h>

#include <setsuna/loader.h>
