add_executable(app_simple
    simple_culler.h
    simple_culler.cpp
    main.cpp
)

set_target_properties(app_simple
   PROPERTIES
   FOLDER "applications"
)

target_link_libraries(app_simple
    setsuna
    setsuna_app
    setsuna_loaders
)
