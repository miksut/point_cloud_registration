
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( BUILD_SUFFIX ${CMAKE_BUILD_TYPE} )
string( TOLOWER ${BUILD_SUFFIX} BUILD_SUFFIX )

set( CMAKE_INCLUDE_CURRENT_DIR On )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

set( FLANN_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/flann" )
set( LZ4_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/lz4" )
set( EIGEN_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/eigen" )

set( FLANN_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install/flann" )
set( LZ4_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install/lz4" )
set( EIGEN_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install/eigen" )


########################################
#         External libraries           #
########################################

# flann

ExternalProject_Add( FLANN_Project
  PREFIX ${FLANN_BUILD_DIR}
  URL "http://www.cs.ubc.ca/research/flann/uploads/FLANN/flann-1.8.4-src.zip"
  BINARY_DIR ${FLANN_BUILD_DIR}
 # INSTALL_DIR ${FLANN_INSTALL_DIR}
 # BUILD_IN_SOURCE 0
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_INSTALL_PREFIX=${FLANN_INSTALL_DIR}
             -DBUILD_PYTHON_BINDINGS=0
             -DBUILD_MATLAB_BINDINGS=0
             -DBUILD_C_BINDINGS=0
)
set (FLANN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install/flann/include")

# lz4

ExternalProject_Add( LZ4_Project
  PREFIX ${LZ4_BUILD_DIR}
  URL "https://github.com/lz4/lz4/archive/dev.zip"
  INSTALL_DIR ${LZ4_INSTALL_DIR}
  BUILD_IN_SOURCE 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lz4_CMakeLists.txt ${LZ4_BUILD_DIR}/src/LZ4_Project/CMakeLists.txt
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
 # CMAKE_CACHE_ARGS "-DLZ4_BUNDLED_MODE:string=off" "-DBUILD_STATIC_LIBS:string=on" "-DBUILD_SHARED_LIBS:string=on"
  #CONFIGURE_COMMAND ""
  INSTALL_COMMAND ""
)


if (MSVC)
set( LZ4_LIBRARY debug ${LZ4_BUILD_DIR}/src/LZ4_Project/contrib/cmake_unofficial/Debug/lz4${CMAKE_STATIC_LIBRARY_SUFFIX} optimized ${LZ4_BUILD_DIR}/src/LZ4_Project/contrib/cmake_unofficial/Release/lz4${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
set( LZ4_LIBRARY debug ${LZ4_BUILD_DIR}/src/LZ4_Project/contrib/cmake_unofficial/Debug/liblz4${CMAKE_STATIC_LIBRARY_SUFFIX} optimized ${LZ4_BUILD_DIR}/src/LZ4_Project/contrib/cmake_unofficial/Release/liblz4${CMAKE_STATIC_LIBRARY_SUFFIX})

endif()
# Eigen

message( "External project - Eigen" )

set( Eigen3_VERSION "3.2.9" )

ExternalProject_Add( Eigen3
  PREFIX ${EIGEN_BUILD_DIR}
  URL "http://bitbucket.org/eigen/eigen/get/${Eigen3_VERSION}.tar.gz"
  #UPDATE_COMMAND ""
  #CONFIGURE_COMMAND ""
  #BUILD_COMMAND ""
  INSTALL_COMMAND ""
 #   ${CMAKE_COMMAND} -E copy_directory
 #     ${CMAKE_BINARY_DIR}/Eigen3-prefix/src/Eigen3/Eigen
 #     ${INSTALL_DEPENDENCIES_DIR}/include/Eigen3/Eigen &&
 #   ${CMAKE_COMMAND} -E copy_directory
 #     ${CMAKE_BINARY_DIR}/Eigen3-prefix/src/Eigen3/unsupported
 #     ${INSTALL_DEPENDENCIES_DIR}/include/Eigen3/unsupported
)

set(EIGEN3_INCLUDE_DIR ${EIGEN_BUILD_DIR}/src/Eigen3)




#find_package( Eigen3 REQUIRED )

##############

set(Registration_Include_Dirs ${EIGEN3_INCLUDE_DIR} ${FLANN_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})
#include_directories( ${EIGEN3_INCLUDE_DIR} ${FLANN_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})

#add_executable( acg_registration ./main.cpp ./icp.cpp ./knn_queries.cpp ./io.cpp )

set(Registration_Libs ${LZ4_LIBRARY})
#target_link_libraries( acg_registration ${LZ4_LIBRARY} )

set(Registration_Dependencies LZ4_Project FLANN_Project Eigen3)
#add_dependencies( acg_registration LZ4_Project FLANN_Project Eigen3)

