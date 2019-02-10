cmake_minimum_required (VERSION 2.8.3)
project (spass)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_support)

# Compile-time Options
option (WITH_ALSA "Use Advanced Linux Sound Architecture (ALSA) as the audio
backed [default=ON]" ON)
option (WITH_PORTAUDIO "Use PortAudio as the audio backed [default=OFF]" OFF)
OPTION (WITH_OSS "Use Open Sound System (OSS) as the audio backed
[default=OFF]" OFF)

set (PACKAGE "spass")
set (VERSION "3.1")
set (PACKAGE_STRING "${PACKAGE}-${VERSION}")

# autogenerate dependency information
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include (FindPkgConfig)

if (WIN32)
	set (Boost_USE_STATIC_LIBS ON)
endif ()
find_package (Boost REQUIRED COMPONENTS program_options)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set (SRC
	audio_random.cpp
	diceware8k.c
	main.cpp
	md5.c
	spass_utils.cpp
)

set (LIBS ${Boost_LIBRARIES})
	
if (WITH_ALSA)
	find_package (ALSA REQUIRED)
	set (AUDIO_SRC audio_random_alsa.cpp)
	set (AUDIO_LIBS ${ALSA_LIBRARIES})
	include_directories (${ALSA_INCLUDE_DIRS})
	set (AUDIO_RANDOM_BACKEND AudioRandomAlsa)
	message(STATUS "Using ALSA as audio backend")
endif ()

if (WITH_PORTAUDIO)
	find_package (Portaudio REQUIRED portaudio-2.0)
	set (AUDIO_SRC audio_random_portaudio.cpp)
	set (AUDIO_LIBS ${PORTAUDIO_LIBRARIES})
	include_directories (${PORTAUDIO_INCLUDE_DIRS})
	set (AUDIO_RANDOM_BACKEND AudioRandomPortAudio)
	message(STATUS "Using PortAudio as audio backend")
endif ()

if (WITH_OSS)
	set (AUDIO_SRC audio_random_oss.cpp)
	set (AUDIO_RANDOM_BACKEND AudioRandomOSS)
	message(STATUS "Using OSS as audio backend")
endif ()

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${PROJECT_SOURCE_DIR})

add_executable (spass
	${SRC}
	${AUDIO_SRC})
target_link_libraries (spass ${LIBS} ${AUDIO_LIBS})

configure_file (
	${CMAKE_SOURCE_DIR}/config.h.cmake
	${CMAKE_BINARY_DIR}/config.h
)

install(TARGETS spass RUNTIME DESTINATION bin/)

add_custom_target(dist
	COMMAND git archive --format=tar --prefix=${PACKAGE_STRING}/ HEAD
		| xz > ${CMAKE_BINARY_DIR}/${PACKAGE_STRING}.tar.xz
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Packaging
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Secure password/passpharse generator")
set (CPACK_PACKAGE_VENDOR "Guy Rutenberg")
set (CPACK_PACKAGE_VERSION ${VERSION})
set (CPACK_PACKAGE_CONTACT "Guy Rutenberg <guyrutenberg@gmail.com>")
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://sourceforge.net/projects/spass")
set (CPACK_DEBIAN_PACKAGE_SECTION "Utilities")
exec_program ("dpkg" ARGS "--print-architecture"
	OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
set (CPACK_PACKAGE_FILE_NAME
	"${PACKAGE}_${VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

include (CPack) # this has to come after all the packaging configuration

project (spass-qt)
#find_package (Qt4 REQUIRED MULTIMEDIAKIT)
# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

#include (${QT_USE_FILE})
include_directories (${CMAKE_CURRENT_BINARY_DIR})

include_directories (${ALSA_INCLUDE_DIRS})

set (spass-qt_SOURCES
	audio_random.cpp
	diceware8k.c
	main_qt.cpp
	mainwindow.cpp
	md5.c
	spass_utils.cpp
	)
	
set (spass-qt_HEADERS mainwindow.h)
set (spass-qt_FORMS mainwindow.ui)

set (QT_LIBRARIES ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES})


add_definitions (${QT_DEFINITIONS})
add_executable(spass-qt
	${spass-qt_SOURCES}
	${AUDIO_SRC}
)


if (WIN32)
	# don't show console window in window when compiling in Release
	set_target_properties(spass-qt PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
	set_target_properties(spass-qt PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif ()

target_link_libraries (spass-qt Qt5::Widgets ${AUDIO_LIBS})
install(TARGETS spass-qt RUNTIME DESTINATION bin/)
