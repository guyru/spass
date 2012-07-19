cmake_minimum_required (VERSION 2.6)
project (spass)
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Secure password/passpharse generator")
set (CPACK_PACKAGE_VENDOR "Guy Rutenberg")
set (CPACK_PACKAGE_VERSION_MAJOR "3")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (PACKAGE "spass")
set (VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set (PACKAGE_STRING "${PACKAGE}-${VERSION}")
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Guy Rutenberg <guyrutenberg@gmail.com>") #required
set (CPACK_DEBIAN_PACKAGE_SECTION "Utilities")

# autogenerate dependency information
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include (FindPkgConfig)
include (CPack)

find_package (Boost REQUIRED COMPONENTS program_options)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

set (SRC
	audio_random.cpp
	diceware8k.c
	main.cpp
	md5.c
)

set (LIBS ${Boost_LIBRARIES})
	
option (WITH_ALSA "Use Advanced Linux Sound Architecture (ALSA) as the audio
backed [default=ON]" ON)
if (WITH_ALSA)
	find_package (ALSA REQUIRED)
	set (SRC ${SRC} audio_random_alsa.cpp)
	set (LIBS ${LIBS} ${ALSA_LIBRARIES})
	include_directories (${ALSA_INCLUDE_DIRS})
endif ()

option (WITH_PORTAUDIO "Use PortAudio as the audio backed [default=OFF]" OFF)
if (WITH_PORTAUDIO)
	pkg_check_modules (PORTAUDIO REQUIRED portaudio-2.0)
	set (SRC ${SRC} audio_random_portaudio.cpp)
	set (LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})
	include_directories (${PORTAUDIO_INCLUDE_DIRS})
endif ()

OPTION (WITH_OSS "Use Open Sound System (OSS) as the audio backed
[default=OFF]" OFF)
if (WITH_OSS)
	set (SRC ${SRC} audio_random_oss.cpp)
endif ()

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${PROJECT_SOURCE_DIR})

add_executable (spass ${SRC})
target_link_libraries (spass ${LIBS})

configure_file (
	${CMAKE_SOURCE_DIR}/config.h.cmake
	${CMAKE_SOURCE_DIR}/config.h
)

install(TARGETS spass RUNTIME DESTINATION bin/)
