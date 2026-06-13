cmake_minimum_required(VERSION 3.16)

project(BossRaid VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# POPRAWIONE: Dopisany moduł Multimedia na końcu obu linijek
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        testy.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BossRaid
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Postac.h
        Karta.h
        Gracz.h
        Boss.h
        assets.qrc
        kartawidget.h kartawidget.cpp kartawidget.ui
    )
else()
    if(ANDROID)
        add_library(BossRaid SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(BossRaid
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# POPRAWIONE: Dodane powiązanie biblioteki dźwiękowej Qt${QT_VERSION_MAJOR}::Multimedia na końcu
target_link_libraries(BossRaid PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Multimedia)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BossRaid)
endif()
set_target_properties(BossRaid PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BossRaid
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BossRaid)
endif()