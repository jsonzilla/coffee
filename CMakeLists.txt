cmake_minimum_required(VERSION 3.16)
project(coffee VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Sql Widgets)

qt_standard_project_setup()

qt_add_executable(coffee WIN32 MACOSX_BUNDLE
    coffeedelegate.cpp coffeedelegate.h
    coffeewindow.cpp coffeewindow.h coffeewindow.ui
    databasemamager.cpp databasemamager.h
    main.cpp
    tablemodelutils.h
)
target_include_directories(coffee PRIVATE
    .
)

target_link_libraries(coffee PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::Widgets
)


# Resources:
set(coffees_resource_files
    "images/arrow-down.png"
    "images/arrow-up.png"
    "images/bean.png"
    "images/clock.png"
    "images/coffee.png"
    "images/coffee2.png"
    "images/data_config.png"
    "images/ground.png"
    "images/ground_v.png"
    "images/spoon.png"
    "images/star.png"
)

qt_add_resources(coffee "coffees"
    PREFIX
        "/"
    FILES
        ${coffees_resource_files}
)

install(TARGETS coffee
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET coffee
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
