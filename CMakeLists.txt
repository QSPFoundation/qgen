cmake_minimum_required(VERSION 3.21)

set(APP_VERSION CACHE STRING "0.0.0")

project(qgen VERSION ${APP_VERSION} HOMEPAGE_URL "https://qsp.org")

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_CXX_STANDARD 11)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
	set(PROJECT_COMPILER_FLAGS "-Wall")
	set(PROJECT_LINKER_FLAGS "")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-char-subscripts -Wno-unused-variable")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
	set(PROJECT_COMPILER_FLAGS "/W4")
	set(PROJECT_LINKER_FLAGS "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang")
	set(PROJECT_COMPILER_FLAGS "")
	set(PROJECT_LINKER_FLAGS "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")
else()
	message(FATAL_ERROR "Compiler is not supported")
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
	set(wxUSE_WEBVIEW OFF)

	set(wxBUILD_MONOLITHIC OFF)
	set(wxBUILD_SAMPLES OFF)
	set(wxBUILD_TESTS OFF)
	set(wxBUILD_DEMOS OFF)
	set(wxBUILD_BENCHMARKS OFF)
	set(wxBUILD_OPTIMISE ON)
	set(wxBUILD_STRIPPED_RELEASE ON)
	set(wxBUILD_SHARED OFF)

	if (MSVC)
		set(wxBUILD_USE_STATIC_RUNTIME ON)
	else()
		set(wxBUILD_USE_STATIC_RUNTIME OFF)
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
	list(APPEND QGEN_SOURCES qgen/rsc/winresource.rc)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/misc/common/icons/qgen.ico" "${CMAKE_BINARY_DIR}/misc/icons/qgen.ico" COPYONLY)
	add_executable(qgen WIN32 ${QGEN_SOURCES})
	target_compile_definitions(qgen PRIVATE _CRT_SECURE_NO_WARNINGS)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT qgen)
elseif (APPLE)
	add_executable(qgen MACOSX_BUNDLE ${QGEN_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/qgen/bitmaps/qgen.icns")
	set_target_properties(qgen PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "QGen"
		MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}"
		MACOSX_BUNDLE_COPYRIGHT "(c) QSP Foundation"
		MACOSX_BUNDLE_GUI_IDENTIFIER "org.qsp.qgen"
		MACOSX_BUNDLE_ICON_FILE "qgen.icns"
		MACOSX_BUNDLE_INFO_STRING "QGen"
		MACOSX_BUNDLE_LONG_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
		RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/qgen/bitmaps/qgen.icns")
elseif (UNIX)
	add_executable(qgen ${QGEN_SOURCES})
else()
	message(FATAL_ERROR "Platform is not supported")
endif()

target_compile_definitions(qgen PRIVATE _UNICODE)
target_compile_options(qgen PRIVATE ${PROJECT_COMPILER_FLAGS})
target_link_options(qgen PRIVATE ${PROJECT_LINKER_FLAGS})
target_link_libraries(qgen PRIVATE wxbase wxcore wxadv wxaui wxhtml wxstc wxxml)

# Installation

if (WIN32)
	set(BUNDLE_NAME qgen.exe)
	set(BINDIR ${CMAKE_INSTALL_BINDIR})
	set(LIBDIR ${CMAKE_INSTALL_LIBDIR})
	set(RLIBDIR ${CMAKE_INSTALL_BINDIR})
	set(HEADERDIR ${CMAKE_INSTALL_INCLUDEDIR})
	set(DATADIR ${CMAKE_INSTALL_BINDIR})
	set(INSTALL_RPATH "")
elseif (APPLE)
	set(BUNDLE_NAME qgen.app)
	set(BINDIR ${BUNDLE_NAME}/Contents/MacOS)
	set(LIBDIR ${BUNDLE_NAME}/Contents/Frameworks)
	set(RLIBDIR ${BUNDLE_NAME}/Contents/Frameworks)
	set(HEADERDIR ${BUNDLE_NAME}/Contents/Frameworks)
	set(DATADIR ${BUNDLE_NAME}/Contents/Resources)
	set(INSTALL_RPATH "@loader_path;@loader_path/../Frameworks")
elseif (UNIX)
	set(BUNDLE_NAME qgen)
	set(BINDIR ${CMAKE_INSTALL_BINDIR})
	set(LIBDIR ${CMAKE_INSTALL_LIBDIR})
	set(RLIBDIR ${CMAKE_INSTALL_LIBDIR})
	set(HEADERDIR ${CMAKE_INSTALL_INCLUDEDIR})
	set(DATADIR ${CMAKE_INSTALL_DATADIR}/qgen)
	set(INSTALL_RPATH "$ORIGIN;$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
else()
	message(FATAL_ERROR "Platform is not supported")
endif()

set_target_properties(qgen PROPERTIES INSTALL_RPATH "${INSTALL_RPATH}")
install(TARGETS qgen
		COMPONENT Main
		RUNTIME DESTINATION ${BINDIR}
		LIBRARY DESTINATION ${RLIBDIR}
		ARCHIVE DESTINATION ${LIBDIR}
		PRIVATE_HEADER DESTINATION ${HEADERDIR}
		PUBLIC_HEADER DESTINATION ${HEADERDIR}
		BUNDLE DESTINATION .)
if (NOT USE_INSTALLED_WX)
	set_target_properties(wxbase wxcore wxadv wxaui wxhtml wxstc PROPERTIES INSTALL_RPATH "${INSTALL_RPATH}")
	install(TARGETS wxbase wxcore wxadv wxaui wxhtml wxstc
			COMPONENT Libs
			RUNTIME DESTINATION ${BINDIR}
			LIBRARY DESTINATION ${RLIBDIR}
			ARCHIVE DESTINATION ${LIBDIR}
			PRIVATE_HEADER DESTINATION ${HEADERDIR}
			PUBLIC_HEADER DESTINATION ${HEADERDIR})
endif()

if (APPLE)
	set(DEP_DIRS "${CMAKE_INSTALL_PREFIX}/${RLIBDIR}")
	install(CODE
			"include(BundleUtilities)
			fixup_bundle(\"${CMAKE_INSTALL_PREFIX}/${BUNDLE_NAME}\" \"\" \"${DEP_DIRS}\")"
			COMPONENT Main)
elseif (UNIX)
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/linux/qgen.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications" COMPONENT Main)
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/common/icons/qgen.svg" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps" COMPONENT Main)
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/misc/common/langs" DESTINATION "${DATADIR}" COMPONENT Main)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/misc/common/keywords.xml" DESTINATION "${DATADIR}" COMPONENT Main)

# Packaging

if (WIN32)
	set(CPACK_PACKAGE_NAME "QGen")
else()
	set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
endif()
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "QGen game editor")
set(CPACK_PACKAGE_VENDOR "QSP Foundation")
set(CPACK_PACKAGE_CONTACT "byte@qsp.org")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_STRIP_FILES ON)
set(CPACK_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_PACKAGE_EXECUTABLES qgen "QSP Classic")
set(CPACK_CREATE_DESKTOP_LINKS qgen)
set(CPACK_NSIS_INSTALLED_ICON_NAME "${BINDIR}\\qgen.exe")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_MANIFEST_DPI_AWARE ON)

set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL Main)

include(CPack)

cpack_add_component(Main DISPLAY_NAME "QGen" REQUIRED)
