set(SETSUNA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

set(SETSUNA_HEADER_FILES
    ${SETSUNA_INCLUDE_DIR}/setsuna/aabb.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/buffer.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/camera.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/color.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/component.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/cull_visitor.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/directed_graph.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/frustum.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/geometry.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/logger.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/mesh.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/mesh_filter.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/mesh_renderer.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/object3d.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/plane.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/ref.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/render_item.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/render_pass.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/render_system.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/resource.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/resource_manager.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/rtti.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/shader_program.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/sphere.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/texture.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/texture_container.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/texture_manager.h
    #${SETSUNA_INCLUDE_DIR}/setsuna/texture_property.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/transform.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/update_visitor.h
    ${SETSUNA_INCLUDE_DIR}/setsuna/visitor.h
)

set(SETSUNA_SOURCE_FILES
    camera.cpp
    #cull_visitor.cpp
    frustum.cpp
    geometry.cpp
    logger.cpp
    mesh.cpp
    #mesh_renderer.cpp
    object3d.cpp
    #render_pass.cpp
    #render_system.cpp
    resource.cpp
    resource_manager.cpp
    shader_program.cpp
    #texture.cpp
    #texture_container.cpp
    #texture_manager.cpp
    update_visitor.cpp
    ${GLAD_ROOT_DIR}/src/glad.c
)

add_library(${PROJECT_NAME}
    ${SETSUNA_SOURCE_FILES}
    ${SETSUNA_HEADER_FILES}
)

find_package(GLM REQUIRED)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${SETSUNA_INCLUDE_DIR}
    PUBLIC ${GLM_INCLUDE_DIRS}
    PUBLIC ${GLAD_ROOT_DIR}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT
    ${SETSUNA_INCLUDE_DIR}/setsuna/rtti_prefix.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

cotire(${PROJECT_NAME})
