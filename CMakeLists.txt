cmake_minimum_required(VERSION 3.12)

project(qt-tesla
  VERSION 0.1
  DESCRIPTION "Libraries and apps that utilize the Tesla APIs."
  LANGUAGES CXX
  )

add_subdirectory(src/api)
add_subdirectory(examples/list-vehicles)
