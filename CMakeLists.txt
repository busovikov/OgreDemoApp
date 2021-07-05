cmake_minimum_required (VERSION 3.8)

project ("DemoApp")

if(NOT OGRE_SDK)
	message(FATAL_ERROR "OGRE SDK not found! Please define sdk path by providing -DOGRE_SDK")
endif()

list(APPEND CMAKE_MODULE_PATH "${OGRE_SDK}/CMake")

find_package(OGRE)

set(SOURCES 
	main.cpp
	)

set(HEADERS
	Hlms.h
)

set(PROJECT_LIBS 
	${OGRE_LIBRARIES} 
	${OGRE_HlmsPbs_LIBRARIES} 
	${OGRE_HlmsUnlit_LIBRARIES} 
	${OGRE_Overlay_LIBRARIES} 
	)

set(PROJECT_INCLUDES
	${OGRE_INCLUDE_DIRS}
	${OGRE_Plugin_CgProgramManager_INCLUDE_DIRS}
	${OGRE_Plugin_ParticleFX_INCLUDE_DIRS}
	${OGRE_RenderSystem_GL_INCLUDE_DIRS}
	${OGRE_RenderSystem_GL3Plu_INCLUDE_DIRS}
	${OGRE_RenderSystem_GLES_INCLUDE_DIRS}
	${OGRE_RenderSystem_GLES_INCLUDE_DIRS}
	${OGRE_RenderSystem_Direct3D9_INCLUDE_DIRS}
	${OGRE_RenderSystem_Direct3D_INCLUDE_DIRS}
	${OGRE_Paging_INCLUDE_DIRS}
	${OGRE_Terrain_INCLUDE_DIRS}
	${OGRE_Volume_INCLUDE_DIRS}
	${OGRE_Overlay_INCLUDE_DIRS}
	${OGRE_HlmsPbs_INCLUDE_DIRS}
	${OGRE_HlmsUnlit_INCLUDE_DIRS}
	${OGRE_HlmsUnlit_INCLUDE_DIRS}/../Common
)

add_executable (${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})

if(CMAKE_BUILD_TYPE STREQUAL  "Debug" )
	set(DEBUG_POSTFIX "_d")
	set(OGRE_DLLS 
		${OGRE_BINARY_DBG} 
		${OGRE_HlmsPbs_BINARY_DBG} 
		${OGRE_HlmsUnlit_BINARY_DBG} 
		${OGRE_Overlay_BINARY_DBG} 
		${OGRE_SDK}/bin/Debug/amd_ags_x64.dll
	)
else()
	set(OGRE_DLLS 
		${OGRE_BINARY_REL} 
		${OGRE_HlmsPbs_BINARY_REL} 
		${OGRE_HlmsUnlit_BINARY_REL} 
		${OGRE_Overlay_BINARY_REL}
		${OGRE_SDK}/bin/Release/amd_ags_x64.dll
	)
	endif()
	
message( STATUS "Copying DLLs")
foreach( DLL_NAME ${OGRE_DLLS} )
	file( COPY "${DLL_NAME}"	DESTINATION "${CMAKE_BINARY_DIR}" )
endforeach()

set(DATA_DIR "${CMAKE_BINARY_DIR}/data")
set(PLUGINS_CFG "Plugins${DEBUG_POSTFIX}.cfg")
set(RESOURCES_CFG "Resources.cfg")

message( STATUS "Generating [${PLUGINS_CFG}, ${RESOURCES_CFG}]" )
configure_file("${PLUGINS_CFG}.in" "${DATA_DIR}/${PLUGINS_CFG}")
configure_file("${RESOURCES_CFG}.in" "${DATA_DIR}/${RESOURCES_CFG}")

message( STATUS "Copying Hlms data files from Ogre repository" )
file( COPY "${OGRE_MEDIA_DIR}/Hlms/Common"	DESTINATION "${DATA_DIR}/Hlms" )
file( COPY "${OGRE_MEDIA_DIR}/Hlms/Pbs"		DESTINATION "${DATA_DIR}/Hlms" )
file( COPY "${OGRE_MEDIA_DIR}/Hlms/Unlit"	DESTINATION "${DATA_DIR}/Hlms" )

message( STATUS "Copying Common data files from Ogre repository" )
file( COPY "${OGRE_MEDIA_DIR}/2.0/scripts/materials/Common"	DESTINATION "${DATA_DIR}/Materials" )
file( COPY "${OGRE_MEDIA_DIR}/packs/DebugPack.zip"	DESTINATION "${DATA_DIR}" )