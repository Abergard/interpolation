add_executable(interpolation
  main.cpp

  cameraclass.cpp
  cameraclass.h
  colorshaderclass.cpp
  colorshaderclass.h
  d3dclass.cpp
  d3dclass.h
  defines.h
  fieldclass.cpp
  fieldclass.h
  graphicsclass.cpp
  graphicsclass.h
  inputclass.cpp
  inputclass.h
  interpolation.cpp
  interpolation.h
  modelclass.cpp
  modelclass.h
  pointclass.cpp
  pointclass.h
  polygonclass.cpp
  polygonclass.h
  systemclass.cpp
  systemclass.h
  )

target_link_libraries(interpolation d3dcompiler Boost::headers)

macro(prepare_shaders)
    set(options)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs FILES)
    cmake_parse_arguments(SHADERS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(MAKE_DIRECTORY ${SHADERS_DESTINATION})
    foreach(SHADER_FILE ${SHADERS_FILES})
      configure_file(${SHADER_FILE} ${SHADERS_DESTINATION}/${SHADER_FILE} COPYONLY)
    endforeach()
endmacro()

prepare_shaders(
  DESTINATION ${EXECUTABLE_OUTPUT_PATH}/shaders
  FILES color.ps color.vs)
