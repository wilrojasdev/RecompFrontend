if (ANDROID)
	include(FetchContent)

	set(FT_DISABLE_ZLIB     ON CACHE BOOL "" FORCE)
	set(FT_DISABLE_BZIP2    ON CACHE BOOL "" FORCE)
	set(FT_DISABLE_PNG      ON CACHE BOOL "" FORCE)
	set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
	set(FT_DISABLE_BROTLI   ON CACHE BOOL "" FORCE)

	set(_ft_prev_shared "${BUILD_SHARED_LIBS}")
	set(BUILD_SHARED_LIBS OFF)

	FetchContent_Declare(
		freetype
		GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
		GIT_TAG        VER-2-14-1
		GIT_SHALLOW    TRUE
	)
	FetchContent_MakeAvailable(freetype)

	set(BUILD_SHARED_LIBS "${_ft_prev_shared}")

	if (NOT TARGET Freetype::Freetype)
		add_library(Freetype::Freetype ALIAS freetype)
	endif()

	set(FREETYPE_FOUND TRUE)
	set(Freetype_FOUND TRUE)
	set(FREETYPE_VERSION_STRING "2.14.1")
else()
	set(FREETYPE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../freetype-windows-binaries/include)
	set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../freetype-windows-binaries/release static/vs2015-2022/win64/freetype.lib")
	add_library(Freetype::Freetype STATIC IMPORTED)
	set_target_properties(Freetype::Freetype PROPERTIES
		IMPORTED_LOCATION ${FREETYPE_LIBRARIES}
	)
	target_include_directories(Freetype::Freetype INTERFACE
		${FREETYPE_INCLUDE_DIRS}
	)
endif()
