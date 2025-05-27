cmake_minimum_required(VERSION 3.16)

project(Sigma_ROS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(Sigma_ROS
    main.cpp
    appicon.rc
)

qt_add_qml_module(Sigma_ROS
    URI untitled1
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES
        QML_FILES SideMenuLeft.qml
        QML_FILES SideMenuRight.qml
        RESOURCES
        RESOURCES  Icons/Hamburger_MD.svg
        RESOURCES Icons/More_Grid_Big.svg
        QML_FILES
        RESOURCES Icons/Add_Minus_Square.svg Icons/Add_Plus_Square.svg
        QML_FILES ZoomButtons.qml
        QML_FILES MyDrawer.qml MyToolBarDrawer.qml
        RESOURCES
        RESOURCES
        QML_FILES MyMap.qml
        QML_FILES MyDrawerContent.qml
        QML_FILES SettingsWindow.qml
        QML_FILES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Sigma_ROS PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled1
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(Sigma_ROS
    PRIVATE Qt6::Quick
)


include(GNUInstallDirs)
install(TARGETS Sigma_ROS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
