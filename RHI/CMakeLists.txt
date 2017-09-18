add_subdirectory(Empty)
IF(${WIN32})
# Windows specific code
add_subdirectory(D3D)
ENDIF(${WIN32})