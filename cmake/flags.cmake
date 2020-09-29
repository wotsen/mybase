
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
# 系统类型检测
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(_IS_64_BIT_ARCH 1)
  message(STATUS "Target is 64 bits")
else()
  set(_IS_64_BIT_ARCH 0)
  message(STATUS "Target is 32 bits")
endif()

# 平台检测
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
  message(STATUS "i386 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
  message(STATUS "i686 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
  message(STATUS "x86_64 architecture detected")
else()
  message(STATUS "host processor architecture is unknown")
endif()

# host name information
cmake_host_system_information(RESULT _HOST_NAME QUERY HOSTNAME)
cmake_host_system_information(RESULT _FQDN QUERY FQDN)

# processor information
cmake_host_system_information(RESULT _PROCESSOR_NAME QUERY PROCESSOR_NAME)
cmake_host_system_information(RESULT _PROCESSOR_DESCRIPTION QUERY PROCESSOR_DESCRIPTION)

# os information
cmake_host_system_information(RESULT _OS_NAME QUERY OS_NAME)
cmake_host_system_information(RESULT _OS_RELEASE QUERY OS_RELEASE)
cmake_host_system_information(RESULT _OS_VERSION QUERY OS_VERSION)
cmake_host_system_information(RESULT _OS_PLATFORM QUERY OS_PLATFORM)

string(TIMESTAMP _CONFIGURATION_TIME "%Y-%m-%d %H:%M:%S [UTC]" UTC)
