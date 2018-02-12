
project (bcls LANGUAGES C VERSION 0.1.0.0)

set(bcls_BUILT_AS_DYNAMIC_LIB)
if(BUILD_SHARED_LIBS)
    set(bcls_BUILT_AS_DYNAMIC_LIB 1)
    if(WIN32)
        ADD_DEFINITIONS("-Dbcls_BUILT_AS_DYNAMIC_LIB")
    endif(WIN32)
endif(BUILD_SHARED_LIBS)

# --------------------------------------------------------------------
# Generate the License File Headers
set(PROJECT_PREFIX "BCLS")
set(CMP_HEADER_DIR ${bcls_BINARY_DIR})
set(CMP_CONFIGURATION_FILE_NAME "bclsConfiguration.h")
set(CMP_TYPES_FILE_NAME "bclsTypes.h")
set(CMP_VERSION_HEADER_FILE_NAME "bclsVersion.h")
set(CMP_VERSION_SOURCE_FILE_NAME "bclsVersion.cpp")


set(CMP_TOP_HEADER_FILE "bclslib.h")
set(CMP_PROJECT_NAMESPACE "BCLS")
set(CMP_PROJECT_NAME "BCLS")
set(CMP_ENABLE_PLUGINS "0")
set(CMP_LIB_SEARCH_DIRS "")
set(CMP_GENERATE_VERSION_STRING 1)
include(${CMP_SOURCE_DIR}/cmpProject.cmake)

cmpConfigureFileWithMD5Check(CONFIGURED_TEMPLATE_PATH ${bcls_SOURCE_DIR}/bcls/bcls.h.in
                            GENERATED_FILE_PATH ${bcls_BINARY_DIR}/${CMP_TOP_HEADER_FILE})

set(bcls_HDRS
  ${bcls_SOURCE_DIR}/bcls/bccgls.h
  ${bcls_SOURCE_DIR}/bcls/bclib.h
  ${bcls_SOURCE_DIR}/bcls/bcls.h
  ${bcls_SOURCE_DIR}/bcls/bclsqr.h
  ${bcls_SOURCE_DIR}/bcls/bcsolver.h
  ${bcls_SOURCE_DIR}/bcls/bctimer.h
  ${bcls_SOURCE_DIR}/bcls/bcversion.h
  ${bcls_SOURCE_DIR}/bcls/lsqr.h
  ${bcls_SOURCE_DIR}/bcls/BCLSLibDLLExport.h
)

set(bcls_SRCS
  ${bcls_SOURCE_DIR}/bcls/bccgls.c
  ${bcls_SOURCE_DIR}/bcls/bcdebug.c
  ${bcls_SOURCE_DIR}/bcls/bclib.c
  ${bcls_SOURCE_DIR}/bcls/bcls.c
  ${bcls_SOURCE_DIR}/bcls/bclsqr.c
  ${bcls_SOURCE_DIR}/bcls/bcsolver.c
  ${bcls_SOURCE_DIR}/bcls/bctimer.c
  ${bcls_SOURCE_DIR}/bcls/bcversion.c
  ${bcls_SOURCE_DIR}/bcls/lsqr.c
)

if(NOT APPLE AND NOT MKL_FOUND)
  set(bcls_HDRS
      ${bcls_HDRS}
      ${bcls_SOURCE_DIR}/bcls/cblas.h
  )

  set(bcls_SRCS
    ${bcls_SRCS}
    ${bcls_SOURCE_DIR}/bcls/bccblas.c
  )
endif()

set(install_dir "bin")
set(lib_install_dir "lib")
if(WIN32)
  set(install_dir "bin")
  set(lib_install_dir "lib")
elseif(APPLE)
  set(install_dir "bin")
  set(lib_install_dir "lib")
endif()


ADD_LIBRARY(bcls ${LIB_TYPE} ${bcls_SRCS} ${bcls_HDRS})
if(MKL_FOUND)
  TARGET_LINK_LIBRARIES(bcls ${MKL_LIBRARIES})
endif()
if(APPLE)
  TARGET_LINK_LIBRARIES(bcls ${LAPACK_LIB})
endif()

target_include_directories(bcls 
                          PUBLIC 
                            ${MKL_INCLUDE_DIR}
                          PRIVATE
                            ${BCLSProj_SOURCE_DIR}/src
                            ${BCLSProj_BINARY_DIR}
                              )

LibraryProperties( ${PROJECT_NAME} ${EXE_DEBUG_EXTENSION} )
  set_target_properties (bcls PROPERTIES
    FOLDER bcls
)


#if(BUILD_SHARED_LIBS)
  INSTALL(TARGETS bcls
    COMPONENT Applications
    EXPORT bclsTargets
    RUNTIME DESTINATION ${install_dir}
    LIBRARY DESTINATION ${lib_install_dir}
    ARCHIVE DESTINATION ${lib_install_dir}
    BUNDLE DESTINATION "."
  )
#endif()

install(FILES ${bcls_HDRS} 
              ${bcls_BINARY_DIR}/${CMP_TOP_HEADER_FILE}
              ${bcls_BINARY_DIR}/${CMP_CONFIGURATION_FILE_NAME}
              ${bcls_BINARY_DIR}/${CMP_TYPES_FILE_NAME}
  DESTINATION include/bcls
  COMPONENT Applications
  )

if(APPLE AND BUILD_SHARED_LIBS)
  set(QAB_INSTALL_DEST "lib")
  set(osx_app_name "libbcls.dylib")
  get_property(bclsBinaryDir GLOBAL PROPERTY bclsBinaryDir)
  set(OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT "${bcls_BINARY_DIR}/bcls/bcls_CompleteTool.cmake")
  set(OPTIMIZE_BUNDLE_SHELL_SCRIPT            "${bcls_BINARY_DIR}/bcls/bcls_OptimizeTool.sh")
  set(PROJECT_INSTALL_DIR "lib")
  configure_file("${CMP_OSX_TOOLS_SOURCE_DIR}/CompleteTool.cmake.in"
                "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}" @ONLY IMMEDIATE)
  configure_file("${CMP_OSX_TOOLS_SOURCE_DIR}/CompleteTool.sh.in"
                "${OPTIMIZE_BUNDLE_SHELL_SCRIPT}" @ONLY IMMEDIATE)
  #install(SCRIPT "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}" COMPONENT ${QAB_COMPONENT})
endif()


# --------------------------------------------------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/bcls/bclsTargetsConfigVersion.cmake"
  VERSION ${bcls_VERSION}
  COMPATIBILITY AnyNewerVersion
)
#if(BUILD_SHARED_LIBS)
  export(EXPORT bclsTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/bcls/bclsTargets.cmake"
    NAMESPACE bcls::
  )
#endif()

configure_file(bcls/cmake/bclsConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/bcls/bclsConfig.cmake"
  @ONLY
)

set(ConfigPackageLocation lib/cmake/bcls)

#if(BUILD_SHARED_LIBS)
  install(EXPORT bclsTargets
    FILE
      bclsTargets.cmake
    NAMESPACE
      bcls::
    DESTINATION
      ${ConfigPackageLocation}
  )

#endif()

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/bcls/bclsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/bcls/bclsTargetsConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)



