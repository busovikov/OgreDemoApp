#include "Hlms.h"

#include "OgreCamera.h"
#include "OgreRoot.h"
#include "OgreWindow.h"
#include "Compositor/OgreCompositorManager2.h"
#include "OgreWindowEventUtilities.h"

class MyWindowEventListener : public Ogre::WindowEventListener
{
    bool mQuit;

public:
    MyWindowEventListener() : mQuit( false ) {}
    virtual void windowClosed( Ogre::Window *rw ) { mQuit = true; }

    bool getQuit( void ) const { return mQuit; }
};

int main( int argc, const char *argv[] )
{
    using namespace Ogre;

    const String pluginsFolder = "./data/";
    const String writeAccessFolder = "./";

#ifndef OGRE_STATIC_LIB
#    if OGRE_DEBUG_MODE
    const char *pluginsFile = "plugins_d.cfg";
#    else
    const char *pluginsFile = "plugins.cfg";
#    endif
#endif
    Root *root = OGRE_NEW Root( pluginsFolder + pluginsFile,    
                                writeAccessFolder + "ogre.cfg",  
                                writeAccessFolder + "Ogre.log" );

    Ogre::RenderSystem* renderSystem = root->getRenderSystemByName("Direct3D11 Rendering Subsystem");
	if (!(renderSystem))
	{
		printf("Render system not found!\n");
		return -1;
	}
    // Initialize Root
    root->setRenderSystem(renderSystem);
    root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );
    root->getRenderSystem()->setConfigOption( "Full Screen", "No" );
    Window *window = root->initialise( true, "Tutorial 00: Basic" );

    //registerHlms();

    // Create SceneManager
    const size_t numThreads = 1u;
    SceneManager *sceneManager = root->createSceneManager( ST_GENERIC, numThreads, "ExampleSMInstance" );

    // Create & setup camera
    Camera *camera = sceneManager->createCamera( "Main Camera" );

    // Position it at 500 in Z direction
    camera->setPosition( Vector3( 0, 5, 15 ) );
    // Look back along -Z
    camera->lookAt( Vector3( 0, 0, 0 ) );
    camera->setNearClipDistance( 0.2f );
    camera->setFarClipDistance( 1000.0f );
    camera->setAutoAspectRatio( true );

    // Setup a basic compositor with a blue clear colour
    CompositorManager2 *compositorManager = root->getCompositorManager2();
    const String workspaceName( "Demo Workspace" );
    const ColourValue backgroundColour( 0.2f, 0.4f, 0.6f );
    compositorManager->createBasicWorkspaceDef( workspaceName, backgroundColour, IdString() );
    compositorManager->addWorkspace( sceneManager, window->getTexture(), camera, workspaceName, true );

    MyWindowEventListener myWindowEventListener;
    WindowEventUtilities::addWindowEventListener( window, &myWindowEventListener );

    bool bQuit = false;

    while( !bQuit )
    {
        WindowEventUtilities::messagePump();
        bQuit = myWindowEventListener.getQuit() || !root->renderOneFrame();
    }

    WindowEventUtilities::removeWindowEventListener( window, &myWindowEventListener );

    OGRE_DELETE root;
    root = 0;

    return 0;
}