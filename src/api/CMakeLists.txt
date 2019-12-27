set(GLOB HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)


add_library(TeslaApi SHARED
  Tesla.cxx
  Vehicle.cxx
  ${HEADERS})

target_include_directories(TeslaApi PUBLIC ../../include)
target_compile_features(TeslaApi PUBLIC cxx_std_11)
target_link_libraries(TeslaApi PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::Network
  )
