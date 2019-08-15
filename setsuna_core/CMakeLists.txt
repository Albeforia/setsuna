set(SETSUNA_CORE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/setsuna_core/include)
	
set(SETSUNA_CORE_HEADER_FILES
	${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/aabb.h
	${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/color.h
	${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/common.h
	${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/frustum.h
	${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/matrix.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/plane.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/quaternion.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/sphere.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/transform.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/math/vector.h

    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/camera.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/component.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/logger.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/object3d.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/rtti.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/shared_queue.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/update_visitor.h
    ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/visitor.h
)

set(SETSUNA_CORE_SOURCE_FILES
    camera.cpp
    color.cpp
    frustum.cpp
    logger.cpp
    object3d.cpp
    update_visitor.cpp
)

add_library(${PROJECT_NAME}
    ${SETSUNA_CORE_SOURCE_FILES}
    ${SETSUNA_CORE_HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${SETSUNA_CORE_INCLUDE_DIR}
)

if(COMMAND cotire)
    set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT
        ${SETSUNA_CORE_INCLUDE_DIR}/setsuna/rtti_prefix.h
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

    cotire(${PROJECT_NAME})
endif()
