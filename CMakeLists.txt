cmake_minimum_required(VERSION 3.16)

project(coffee)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 howCOMPONENTS Core Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Sql REQUIRED)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.qrc")

add_executable(coffee ${SOURCES})

target_link_libraries(coffee Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

if (${DEPLOYQT_EXECUTABLE} STREQUAL "Run")
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    if (WIN32)
      find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
      if(WINDEPLOYQT_EXECUTABLE)
        set(OS_VERSION_FILE "win")
        add_custom_command(TARGET coffee POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                    "$<TARGET_FILE:coffee>"
            COMMENT "Running windeployqt...")
      endif()

    elseif (APPLE)
      find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
      if (MACDEPLOYQT_EXECUTABLE)
        set(OS_VERSION_FILE "mac")
        add_custom_command(TARGET coffee POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${_qt_bin_dir}" "${MACDEPLOYQT_EXECUTABLE}"
                    "$<TARGET_FILE:coffee>"
            COMMENT "Running macdeployqt...")
      endif()

    elseif (UNIX AND NOT APPLE)
      find_program(LINUXDEPLOYQT_EXECUTABLE linuxdeployqt HINTS "${_qt_bin_dir}")
      if(LINUXDEPLOYQT_EXECUTABLE)
        set(OS_VERSION_FILE "linux")
        add_custom_command(TARGET coffee POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${_qt_bin_dir}" "${CMAKE_PREFIX_PATH}/bin/linuxdeployqt"
                    "$<TARGET_FILE:coffee>"
            COMMENT "Running linuxdeployqt..."
        )
      endif()
    endif()

    if (CMAKE_BUILD_TYPE MATCHES Release)
      set(BUILD_VERSION_PATH "Release")
    else()
      set(BUILD_VERSION_PATH "Debug")
    endif()

    add_custom_command(TARGET coffee POST_BUILD
      COMMAND 7z a -tzip -mx=9 -mfb=258 -mpass=15 -r coffee_${OS_VERSION_FILE}.zip ${CMAKE_BINARY_DIR}/${BUILD_VERSION_PATH}/*.*
      COMMENT "Running 7z..."
    )
endif()
