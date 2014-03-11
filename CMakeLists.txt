cmake_minimum_required(VERSION 2.8)

project(vtfthumbcreator)

# ---- dependencies -----------------------------------------------------------
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroOptionalAddSubdirectory)

# ---- compiler flags ---------------------------------------------------------
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -Werror -Wno-write-strings -O3")
endif()

# ---- build library ----------------------------------------------------------
set(vtfthumbcreator_SRCS VTFThumbCreator.cpp)

kde4_add_plugin(vtfthumbcreator ${vtfthumbcreator_SRCS} VTFThumbCreator.h)
target_link_libraries(vtfthumbcreator ${KDE4_KIO_LIBS})

# ---- install target ---------------------------------------------------------
install(TARGETS vtfthumbcreator DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES vtfthumbcreator.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# ---- uninstall target -------------------------------------------------------
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
