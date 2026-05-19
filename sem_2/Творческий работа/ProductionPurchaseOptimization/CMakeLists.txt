cmake_minimum_required(VERSION 3.16)

project(ProductionPurchaseOptimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

option(BUILD_CONSOLE "Build old console version" ON)

set(CORE_SOURCES
    src/DateUtils.cpp
    src/Database.cpp
    src/Planner.cpp
    src/Report.cpp
)

find_package(Qt6 QUIET COMPONENTS Widgets)

if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_PACKAGE Qt5)
endif()

if(QT_PACKAGE)
    add_executable(ProductionPurchaseOptimizationQt
        src/QtMain.cpp
        src/MainWindow.cpp
        src/ItemDialog.cpp
        ${CORE_SOURCES}
    )

    target_link_libraries(ProductionPurchaseOptimizationQt PRIVATE ${QT_PACKAGE}::Widgets)

    if(MSVC)
        target_compile_options(ProductionPurchaseOptimizationQt PRIVATE /utf-8)
    endif()
else()
    message(WARNING "Qt6 Widgets was not found. Qt target will not be built.")
endif()

if(BUILD_CONSOLE)
add_executable(ProductionPurchaseOptimizationConsole
    src/main.cpp
    ${CORE_SOURCES}
    src/ConsoleUI.cpp
)

if(MSVC)
    target_compile_options(ProductionPurchaseOptimizationConsole PRIVATE /utf-8)
endif()
endif()
