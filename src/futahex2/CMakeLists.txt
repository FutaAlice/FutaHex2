PROJECT (FUTAHEX2)

FIND_PACKAGE(Qt5 REQUIRED Core Gui Widgets)

FILE(GLOB SOURCE_FILES "./*.cpp")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(futahex2 ${SOURCE_FILES})
    
target_link_libraries(futahex2 Qt5::Core Qt5::Gui Qt5::Widgets)

# fhcore
ADD_DEPENDENCIES(futahex2 fhcore)
TARGET_LINK_LIBRARIES(futahex2 fhcore)

# fhgui
ADD_DEPENDENCIES(futahex2 fhgui)
TARGET_LINK_LIBRARIES(futahex2 fhgui)

IF(UNIX)
    TARGET_LINK_LIBRARIES(futahex2 pthread)
ENDIF()
