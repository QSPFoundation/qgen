cmake_minimum_required(VERSION 3.13)
project(qgen)

if(WIN32)
	if(NOT MSVC OR MSVC_TOOLSET_VERSION LESS 110)
		message(FATAL_ERROR "Only Visual Studio 2012 and later is supported for Windows targets")
	endif()
elseif(APPLE OR NOT UNIX)
	message(FATAL_ERROR "Only Windows and Linux targets are supported")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	set(PROJECT_COMPILER_FLAGS "-Wall")
	set(PROJECT_LINKER_FLAGS "-no-pie")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()
if(MSVC)
	set(PROJECT_COMPILER_FLAGS "/W4")
	set(PROJECT_LINKER_FLAGS "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
endif()

if(USE_INSTALLED_WX)
	find_package(wxWidgets REQUIRED base core adv aui html stc xml)
	include(${wxWidgets_USE_FILE})
else()
	# Download and unpack wxWidgets at configure time
	configure_file(CMakeLists-wxWidgets.txt.in "${CMAKE_BINARY_DIR}/wxWidgets-download/CMakeLists.txt")
	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/wxWidgets-download"
	)
	execute_process(COMMAND "${CMAKE_COMMAND}" --build .
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/wxWidgets-download"
	)
	# Patch wxWidgets
	execute_process(COMMAND git apply "${CMAKE_SOURCE_DIR}/build_wx/wxPatch.diff"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/wxWidgets-src"
	)

	# Disable wxWidgets features we don't need
	set(wxUSE_LIBTIFF OFF)
	set(wxUSE_REGEX OFF)
	set(wxUSE_SECRETSTORE OFF)
	set(wxUSE_LIBSDL OFF)
	set(wxUSE_LIBMSPACK OFF)
	set(wxUSE_GSTREAMER OFF)

	# Prevent wxWidgets from overriding our compiler/linker options
	# when building with Visual Studio
	set(wxWidgets_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add wxWidgets directly to our build
	add_subdirectory("${CMAKE_BINARY_DIR}/wxWidgets-src"
					 "${CMAKE_BINARY_DIR}/wxWidgets-build"
	)
endif()

set(QGEN_SOURCES
	src/optionsdialog.cpp
	src/chksyshotkey.cpp
	src/actioncode.cpp
	src/mainframe.cpp
	src/actionslistbox.cpp
	src/optionshotkeysdialog.cpp
	src/locationcode.cpp
	src/locationslistbox.cpp
	src/application.cpp
	src/datacontainer.cpp
	src/hotkeytextbox.cpp
	src/toolbar.cpp
	src/locationpage.cpp
	src/keywordsstore.cpp
	src/initevent.cpp
	src/keysparser.cpp
	src/locationsnotebook.cpp
	src/searchdialog.cpp
	src/hotkeysstore.cpp
	src/locationtip.cpp
	src/game.cpp
	src/syntaxtextbox.cpp
	src/mergedialog.cpp
	src/locationactions.cpp
	src/imagepathtextbox.cpp
	src/searchdatastore.cpp
	src/locationdesc.cpp
	src/controls.cpp
	src/settings.cpp
	src/actionspanel.cpp
)
if(WIN32)
	list(APPEND QGEN_SOURCES src/winresource.rc)
	configure_file("${CMAKE_SOURCE_DIR}/src/bitmaps/qgen.ico" "${CMAKE_BINARY_DIR}/misc/icons/qgen.ico" COPYONLY)
	add_executable(qgen WIN32 ${QGEN_SOURCES})
	target_compile_definitions(qgen PRIVATE WXUSINGDLL _CRT_SECURE_NO_WARNINGS)
else()
	add_executable(qgen ${QGEN_SOURCES})
endif()
target_compile_definitions(qgen PRIVATE _UNICODE)
target_compile_options(qgen PRIVATE ${PROJECT_COMPILER_FLAGS})
target_link_options(qgen PRIVATE ${PROJECT_LINKER_FLAGS})
target_link_libraries(qgen PRIVATE wxbase wxcore wxadv wxaui wxhtml wxstc wxxml)
install(TARGETS qgen wxbase wxcore wxadv wxaui wxhtml wxstc wxxml
		RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
		LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/static"
)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/misc/common/"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)
