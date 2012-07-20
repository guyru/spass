cmake_minimum_required (VERSION 2.8.3)
project (spass)

# Compile-time Options
option (WITH_ALSA "Use Advanced Linux Sound Architecture (ALSA) as the audio
backed [default=ON]" ON)
option (WITH_PORTAUDIO "Use PortAudio as the audio backed [default=OFF]" OFF)
OPTION (WITH_OSS "Use Open Sound System (OSS) as the audio backed
[default=OFF]" OFF)

set (PACKAGE "spass")
set (VERSION "3.0")
set (PACKAGE_STRING "${PACKAGE}-${VERSION}")

# autogenerate dependency information
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include (FindPkgConfig)

find_package (Boost REQUIRED COMPONENTS program_options)

set (SRC
	audio_random.cpp
	diceware8k.c
	main.cpp
	md5.c
)

set (LIBS ${Boost_LIBRARIES})
	
if (WITH_ALSA)
	find_package (ALSA REQUIRED)
	set (SRC ${SRC} audio_random_alsa.cpp)
	set (LIBS ${LIBS} ${ALSA_LIBRARIES})
	include_directories (${ALSA_INCLUDE_DIRS})
	message(STATUS "Using ALSA as audio backend")
endif ()

if (WITH_PORTAUDIO)
	pkg_check_modules (PORTAUDIO REQUIRED portaudio-2.0)
	set (SRC ${SRC} audio_random_portaudio.cpp)
	set (LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})
	include_directories (${PORTAUDIO_INCLUDE_DIRS})
	message(STATUS "Using PortAudio as audio backend")
endif ()

if (WITH_OSS)
	set (SRC ${SRC} audio_random_oss.cpp)
	message(STATUS "Using OSS as audio backend")
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

# Packaging
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Secure password/passpharse generator")
set (CPACK_PACKAGE_VENDOR "Guy Rutenberg")
set (CPACK_PACKAGE_VERSION ${VERSION})
set (CPACK_PACKAGE_CONTACT "Guy Rutenberg <guyrutenberg@gmail.com>")
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/guyru/spass")
set (CPACK_DEBIAN_PACKAGE_SECTION "Utilities")
exec_program ("dpkg" ARGS "--print-architecture"
	OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
set (CPACK_PACKAGE_FILE_NAME
	"${PACKAGE}_${VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

include (CPack) # this has to come after all the packaging configuration
