cmake_minimum_required(VERSION 3.16)

project(Sigma_ROS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Qt6Positioning_DIR "C:/Qt/6.8.2/msvc2022_64/lib/cmake/Qt6Positioning")
set(QT_QML_GENERATE_QMLTYPES TRUE)
find_package(Qt6 REQUIRED COMPONENTS Quick Core Positioning QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(Sigma_ROS
    main.cpp
    appicon.rc
)

qt_add_qml_module(Sigma_ROS
    URI Sigma_ROS
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        QML_FILES qml/menus/SideMenuLeft.qml
        QML_FILES qml/menus/SideMenuRight.qml
        QML_FILES qml/buttons/ZoomButtons.qml
        QML_FILES qml/menus/MyDrawer.qml
        QML_FILES qml/components/MyMap.qml
        QML_FILES qml/content/MyDrawerContent.qml
        QML_FILES qml/windows/SettingsWindow.qml
        QML_FILES qml/menus/TopMenu.qml
        QML_FILES qml/windows/ObjectsList.qml
    RESOURCES
        Icons/Add_Plus_Square.svg
        Icons/Add_Minus_Square.svg
        Icons/Hamburger_MD.svg
        Icons/More_Grid_Big.svg
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
    PRIVATE
            Qt6::Quick
            Qt6::Core
            Qt6::Positioning
            libbase
            libtle
            SaveHelperLib
            Qt6::QuickControls2
            TrajectoryManager
            ObjectListProvider
)



include(GNUInstallDirs)
install(TARGETS Sigma_ROS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(libbase)
add_subdirectory(libtle)
add_subdirectory(SaveHelperLib)
add_subdirectory(TrajectoryManager)
add_subdirectory(ObjectListProvider)
