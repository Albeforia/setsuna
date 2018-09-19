#
# Find stb image
#
# Try to find stb image: Image loading/decoding from stb.
# This module defines
# - STBI_INCLUDE_DIRS
# - STBI_FOUND
#
# The following variables can be set as arguments for the module.
# - STB_ROOT_DIR : Root library directory of stb
#

# Additional modules
include(FindPackageHandleStandardArgs)

if (WIN32)
	# Find include files
	find_path(
		STBI_INCLUDE_DIR
		NAMES stb_image.h
		PATHS
		$ENV{PROGRAMFILES}/include
		${STB_ROOT_DIR}
		DOC "The directory where stb_image.h resides")
else()
	# Find include files
	find_path(
		STBI_INCLUDE_DIR
		NAMES stb_image.h
		PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${STB_ROOT_DIR}
		DOC "The directory where stb_image.h resides")
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(STBI DEFAULT_MSG STBI_INCLUDE_DIR)

# Define STBI_INCLUDE_DIRS
if (STBI_FOUND)
	set(STBI_INCLUDE_DIRS ${STBI_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(STBI_INCLUDE_DIR)