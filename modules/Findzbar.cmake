include(LibFindMacros)

# Include dir
find_path(zbar_INCLUDE_DIR
	NAMES 
	zbar.h
	PATHS 
	/usr/local/include
)

# Finally the library itself
find_library(zbar_LIBRARY
	NAMES 
	zbar
	PATHS 
	/usr/local/lib
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
libfind_process(zbar)

