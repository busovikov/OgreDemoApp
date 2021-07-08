#pragma once

#include "OgreConfigFile.h"
#include <OgreArchiveManager.h>
#include "OgreHlmsManager.h"
#include <OgreHlmsPbs.h>
#include "OgreHlmsUnlit.h"

template<class T>
T* CreateHlms(const Ogre::String& rootFolder);

void registerHlms( void )
{
    using namespace Ogre;

    String resourcePath = "data/";

    ConfigFile cf;
    cf.load( resourcePath + "Resources.cfg" );
    String rootHlmsFolder = resourcePath + cf.getSetting( "DoNotUseAsResource", "Hlms", "" );

    if( rootHlmsFolder.empty() )
        rootHlmsFolder = "./";
    else if( *( rootHlmsFolder.end() - 1 ) != '/' )
        rootHlmsFolder += "/";

    // At this point rootHlmsFolder should be a valid path to the Hlms data folder

    auto mng = Root::getSingleton().getHlmsManager();
    mng->registerHlms( CreateHlms<HlmsUnlit>(rootHlmsFolder) );
    mng->registerHlms( CreateHlms<HlmsPbs>(rootHlmsFolder) );
}

template<class T>
T* CreateHlms(const Ogre::String& rootFolder)
{
    using namespace Ogre;

    // For retrieval of the paths to the different folders needed
    String mainFolderPath;
    StringVector libraryFoldersPaths;
    StringVector::const_iterator libraryFolderPathIt;
    StringVector::const_iterator libraryFolderPathEn;

    ArchiveManager &archiveManager = ArchiveManager::getSingleton();

    // Create & Register T (Hlms implementation)
    // Get the path to all the subdirectories used by HlmsUnlit
    T::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
    ArchiveVec archiveLibraryFolders;
    Archive *archive =
        archiveManager.load( rootFolder + mainFolderPath, "FileSystem", true );
    libraryFolderPathIt = libraryFoldersPaths.begin();
    libraryFolderPathEn = libraryFoldersPaths.end();
    while( libraryFolderPathIt != libraryFolderPathEn )
    {
        Archive *archiveLibrary =
            archiveManager.load( rootFolder + *libraryFolderPathIt, "FileSystem", true );
        archiveLibraryFolders.push_back( archiveLibrary );
        ++libraryFolderPathIt;
    }

    // Create and register the Hlms
    T* hlms = OGRE_NEW T( archive, &archiveLibraryFolders );

    RenderSystem *renderSystem = Root::getSingletonPtr()->getRenderSystem();
    if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
    {
        // Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
        // and below to avoid saturating AMD's discard limit (8MB) or
        // saturate the PCIE bus in some low end machines.
        bool supportsNoOverwriteOnTextureBuffers;
        renderSystem->getCustomAttribute( "MapNoOverwriteOnDynamicBufferSRV",
                                        &supportsNoOverwriteOnTextureBuffers );

        if( !supportsNoOverwriteOnTextureBuffers )
        {
            hlms->setTextureBufferDefaultSize( 512 * 1024 );
        }
    }
    return hlms;
}
        