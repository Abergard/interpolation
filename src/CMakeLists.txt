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

set_target_properties(interpolation PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
target_link_libraries(interpolation gdi32 user32 glu32 opengl32 Boost::headers d3dcompiler)
