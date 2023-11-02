cmake_minimum_required(VERSION 3.16)

project(Neurons VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appNeurons
    main.cpp
    Matrix.hpp
    Neural_Network.hpp
    neuralnetworkwraper.cpp
    neuralnetworkwraper.h
)

qt_add_resources(appNeurons "images"
    PREFIX "/images"
    FILES
        nothing.png
        0.png
        1.png
        2.png
        3.png
        4.png
        5.png
        6.png
        7.png
        8.png
        9.png
        Network2.svg
        Neural.png
)

qt_add_qml_module(appNeurons
    URI Neurons
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES AppMenu.qml
    QML_FILES AppFrame.qml
    QML_FILES SettingsFrame.qml
    QML_FILES DrawSpace.qml
    QML_FILES ButtonSet.qml
)

set_target_properties(appNeurons PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appNeurons
    PRIVATE Qt6::Quick
)

install(TARGETS appNeurons
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
