cmake_minimum_required(VERSION 3.13)
project(qgen)

if (WIN32)
	if (NOT (MSVC OR MINGW))
		message(FATAL_ERROR "Only Visual C++ and MinGW are supported for Windows targets")
	endif()
elseif (APPLE OR NOT UNIX)
	message(FATAL_ERROR "Only Windows and Linux targets are supported")
endif()

if (CMAKE_COMPILER_IS_GNUCC)
	set(PROJECT_COMPILER_FLAGS "-Wall")
	set(PROJECT_LINKER_FLAGS "-no-pie")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-char-subscripts -Wno-unused-variable")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")
endif()
if (MSVC)
	set(PROJECT_COMPILER_FLAGS "/W4")
	set(PROJECT_LINKER_FLAGS "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if (USE_INSTALLED_WX)
	find_package(wxWidgets REQUIRED base core adv aui html stc)
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
	execute_process(COMMAND git checkout --recurse-submodules -- .
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/wxWidgets-src"
	)
	execute_process(COMMAND git apply --ignore-whitespace --reject "${CMAKE_SOURCE_DIR}/build_wx/wxPatch.diff"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/wxWidgets-src"
	)

	# Disable wxWidgets features we don't need
	set(wxUSE_ACTIVEX OFF)
	set(wxUSE_LIBTIFF OFF)
	set(wxUSE_REGEX OFF)
	set(wxUSE_SECRETSTORE OFF)
	set(wxUSE_LIBSDL OFF)
	set(wxUSE_LIBMSPACK OFF)
	set(wxUSE_GSTREAMER OFF)
	set(wxUSE_OPENGL OFF)
	set(wxUSE_RICHTEXT OFF)
	set(wxUSE_RIBBON OFF)
	set(wxUSE_MEDIACTRL OFF)
	set(wxUSE_PROPGRID OFF)
	set(wxUSE_DEBUGREPORT OFF)
	set(wxUSE_SOCKETS OFF)
	set(wxUSE_PROTOCOL OFF)
	set(wxUSE_URL OFF)
	set(wxUSE_FS_INET OFF)

	set(wxBUILD_MONOLITHIC OFF)
	set(wxBUILD_SAMPLES OFF)
	set(wxBUILD_TESTS OFF)
	set(wxBUILD_DEMOS OFF)
	set(wxBUILD_BENCHMARKS OFF)
	set(wxBUILD_OPTIMISE ON)
	set(wxBUILD_STRIPPED_RELEASE ON)

	if (WIN32)
		set(wxBUILD_SHARED OFF)
		set(wxBUILD_USE_STATIC_RUNTIME ON)
	else()
		set(wxBUILD_SHARED ON)
	endif()

	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(wxWidgets_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add wxWidgets directly to our build
	set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)
	set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
	add_subdirectory("${CMAKE_BINARY_DIR}/wxWidgets-src"
					 "${CMAKE_BINARY_DIR}/wxWidgets-build"
	)
endif()

set(QGEN_SOURCES
	qgen/optionsdialog.cpp
	qgen/chksyshotkey.cpp
	qgen/actioncode.cpp
	qgen/mainframe.cpp
	qgen/actionslistbox.cpp
	qgen/optionshotkeysdialog.cpp
	qgen/locationcode.cpp
	qgen/locationslistbox.cpp
	qgen/application.cpp
	qgen/datacontainer.cpp
	qgen/hotkeytextbox.cpp
	qgen/toolbar.cpp
	qgen/locationpage.cpp
	qgen/keywordsstore.cpp
	qgen/initevent.cpp
	qgen/keysparser.cpp
	qgen/locationsnotebook.cpp
	qgen/searchdialog.cpp
	qgen/hotkeysstore.cpp
	qgen/locationtip.cpp
	qgen/game.cpp
	qgen/syntaxtextbox.cpp
	qgen/mergedialog.cpp
	qgen/locationactions.cpp
	qgen/imagepathtextbox.cpp
	qgen/searchdatastore.cpp
	qgen/locationdesc.cpp
	qgen/controls.cpp
	qgen/settings.cpp
	qgen/actionspanel.cpp
)
if (WIN32)
	list(APPEND QGEN_SOURCES qgen/winresource.rc)
	configure_file("${CMAKE_SOURCE_DIR}/qgen/bitmaps/qgen.ico" "${CMAKE_BINARY_DIR}/misc/icons/qgen.ico" COPYONLY)
	add_executable(qgen WIN32 ${QGEN_SOURCES})
	target_compile_definitions(qgen PRIVATE _CRT_SECURE_NO_WARNINGS)
	set(RESOURCES_DIR "${CMAKE_INSTALL_PREFIX}/bin")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT qgen)
else()
	add_executable(qgen ${QGEN_SOURCES})
	set(RESOURCES_DIR "${CMAKE_INSTALL_PREFIX}/share/qgen")
endif()
target_compile_definitions(qgen PRIVATE _UNICODE)
target_compile_options(qgen PRIVATE ${PROJECT_COMPILER_FLAGS})
target_link_options(qgen PRIVATE ${PROJECT_LINKER_FLAGS})
target_link_libraries(qgen PRIVATE wxbase wxcore wxadv wxaui wxhtml wxstc wxxml)
install(TARGETS qgen)
if (NOT USE_INSTALLED_WX)
	install(TARGETS wxbase wxcore wxadv wxaui wxhtml wxstc)
endif()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/misc/common/" DESTINATION "${RESOURCES_DIR}")
