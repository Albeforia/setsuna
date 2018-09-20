set(SETSUNA_LOADERS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/setsuna_loaders/include)

set(SETSUNA_LOADERS_HEADER_FILES
    ${SETSUNA_LOADERS_INCLUDE_DIR}/setsuna_loaders/texture_loader.h
)

set(SETSUNA_LOADERS_SOURCE_FILES
    texture_loader.cpp
)

add_library(setsuna_loaders
    ${SETSUNA_LOADERS_SOURCE_FILES}
    ${SETSUNA_LOADERS_HEADER_FILES}
)

find_package(Stbi REQUIRED)

target_include_directories(setsuna_loaders
    PUBLIC ${SETSUNA_LOADERS_INCLUDE_DIR}
    PRIVATE ${STBI_INCLUDE_DIRS}
)

target_link_libraries(setsuna_loaders
    setsuna
)
