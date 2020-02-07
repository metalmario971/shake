#include "../base/Base.h"
#include "../app/AppBase.h"
#include "../base/GLContext.h"
#include "../base/Gu.h"
#include "../base/SDLUtils.h"
#include "../base/Delta.h"
#include "../base/FrameSync.h"
#include "../base/Perf.h"
#include "../base/InputManager.h"

#include "../gfx/WindowViewport.h"
#include "../gfx/ShaderMaker.h"
#include "../gfx/Picker.h"
#include "../gfx/ParticleManager.h"
#include "../gfx/LightManager.h"
#include "../gfx/TexCache.h"
#include "../gfx/GraphicsApi.h"
#include "../gfx/RenderPipe.h"
#include "../gfx/UiControls.h"
#include "../gfx/GraphicsApi.h"
#include "../gfx/OpenGLApi.h"
#include "../gfx/VulkanApi.h"
#include "../gfx/UiControls.h"

#include "../model/ModelCache.h"

#include "../app/GraphicsWindow.h"
#include "../app/OpenGLWindow.h"

namespace Game {

//Called exclusively by the graphics API
GraphicsWindow::GraphicsWindow(bool ismain, t_string title, RenderSystem::e sys) {
  this->_bIsMainWindow = ismain;
  _title = title;

}
GraphicsWindow::~GraphicsWindow() {
  //Delete Global Managers
  _pTexCache = nullptr;
  _pCamera = nullptr;
  _pParty = nullptr;
  _pShaderMaker = nullptr;
  _pLightManager = nullptr;
  _pModelCache = nullptr;
  _pPicker = nullptr;
  _pPhysicsWorld = nullptr;
  _pDelta = nullptr;
  _pFpsMeter = nullptr;
  _pFrameSync = nullptr;

  //_pGraphicsApi = nullptr;

  if (_pSDLWindow != nullptr) {
    SDL_DestroyWindow(_pSDLWindow);
    _pSDLWindow = nullptr;
  }
}
void GraphicsWindow::init() {
  createManagers();
}
void GraphicsWindow::createManagers() {
  //BroLogInfo("Creating Graphics API.");
  //if (Gu::getEngineConfig()->getRenderSystem() == RenderSystem::OpenGL) {
  //  _pGraphicsApi = std::make_shared<OpenGLApi>();
  //}
  //else if (Gu::getEngineConfig()->getRenderSystem() == RenderSystem::Vulkan) {
  //  _pGraphicsApi = std::make_shared<VulkanApi>();
  //}
  //else {
  //  BroThrowException("Invalid render engine.");
  //}

  BroLogInfo("Creating Renderer.");
  initRenderSystem();

  BroLogInfo("Creating SDL Window");
  createSDL_OpenGLWindow(_title);

  BroLogInfo("Creating Window UI");
  _pScreen = std::make_shared<UiScreen>(getThis<GraphicsWindow>());

  BroLogInfo("GLContext - Creating Particles");
  _pParty = std::make_shared<ParticleManager>(getGraphicsContext());

  BroLogInfo("GLContext - Creating FpsMeter");
  _pFpsMeter = std::make_shared<FpsMeter>();

  BroLogInfo("GLContext - Creating FrameSync");
  _pFrameSync = std::make_shared<FrameSync>();

  //This was commented out.  Why? 11/6
  BroLogInfo("GLContext - Creating ShaderMaker & base shaders");
  _pShaderMaker = std::make_shared<ShaderMaker>();
  _pShaderMaker->initialize(Gu::getApp());

  BroLogInfo("GLContext - Creating Texture Cache");
  _pTexCache = std::make_shared<TexCache>(getGraphicsContext());

  BroLogInfo("GLContext -  Lights");
  _pLightManager = std::make_shared<LightManager>(getGraphicsContext());

  BroLogInfo("GLContext - Model Cache");
  _pModelCache = std::make_shared<ModelCache>();

  BroLogInfo("GLContext - Picker");
  _pPicker = std::make_shared<Picker>(getGraphicsContext());

  BroLogInfo("Delta");
  _pDelta = std::make_shared<Delta>();
}
void GraphicsWindow::createSDL_OpenGLWindow(t_string windowTitle) {

  int minGLVersion;
  int minGLSubversion;
  const int c_iMax_Profs = 2;
  GLContext::GLProfile profs[c_iMax_Profs];
  int iProfile = SDL_GL_CONTEXT_PROFILE_CORE;
  bool bVsync = false;

#ifdef BRO_OS_IPHONE
  minGLVersion = 3;
  minGLSubversion = 0;
  iProfile = SDL_GL_CONTEXT_PROFILE_ES;
#elif BRO_OS_WINDOWS
  minGLVersion = 3;
  minGLSubversion = 3;
  iProfile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
#endif

  // if (getMainWindow() == nullptr) {
  profs[0].make(32, minGLVersion, minGLSubversion, iProfile, bVsync);
  profs[1].make(24, minGLVersion, minGLSubversion, iProfile, bVsync);

  for (int iProf = 0; iProf < c_iMax_Profs; ++iProf) {
    //This must be called before creating the window because this sets SDL's PixelFormatDescritpro
    GLContext::setWindowAndOpenGLFlags(profs[iProf]);

    makeSDLWindow(windowTitle, SDL_WINDOW_OPENGL);

    _pContext = std::make_shared<GLContext>();
    if (_pContext->create(getThis<GraphicsWindow>(), profs[iProf]) == false) {
      _pContext = nullptr;
    }

  //  //Unfortunately because SDL needs a window, we need to wait to create the context.
  //  if (getGraphicsContext() == nullptr) {
  //    if (std::dynamic_pointer_cast<OpenGLApi>(getGraphicsApi())->makeContext(getThis<GraphicsWindow>(), profs[iProf]) != nullptr) {
  //      //Couldn't make context, try again.
  //      break;
  //    }
  //  }
  }
}

void GraphicsWindow::makeCurrent() {
  SDL_GL_MakeCurrent(getSDLWindow(), getGraphicsContext()->getSDLGLContext());
}
void GraphicsWindow::getDrawableSize(int* w, int* h) {
  SDL_GL_GetDrawableSize(getSDLWindow(), w, h);
}
void GraphicsWindow::swapBuffers() {
  SDL_GL_SwapWindow(getSDLWindow());
}

void GraphicsWindow::makeSDLWindow(t_string windowTitle, int render_system) {
  t_string title;
  bool bFullscreen = false;

  int x, y, w, h, flags;
#ifdef BRO_OS_IPHONE
  x = 0, y = 0, w = 320, h = 480, flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
  title = "";
#elif BRO_OS_WINDOWS

  //SDL_WINDOW_OPENGL | SDL_WINDOW_VULKAN;

  if (bFullscreen) {
    x = 0; y = 0;
    w = 1920; h = 1080;
    flags = render_system;
  }
  else {
    x = 100, y = 100, w = 800, h = 600, flags = SDL_WINDOW_SHOWN | render_system | SDL_WINDOW_RESIZABLE;
  }
  title = windowTitle;
#endif

  //No0te: This calls SDL_GL_LOADLIBRARY if SDL_WINDOW_OPENGL is set.
  _pSDLWindow = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
  SDLUtils::checkSDLErr();

  //Fullscreen nonsense
  if (bFullscreen) {
    SDL_SetWindowFullscreen(_pSDLWindow, SDL_WINDOW_FULLSCREEN);
  }
  SDLUtils::checkSDLErr();

  //Note we may need to adjust for actual start width/height if start width is too large , etc.
  //*Set room width / height
  _iLastWidth = Gu::getConfig()->getDefaultScreenWidth();
  _iLastHeight = Gu::getConfig()->getDefaultScreenHeight();
  _pWindowViewport = std::make_shared<WindowViewport>(_iLastWidth, _iLastHeight);

#ifdef BRO_OS_WINDOWS
  BroLogError("We are not making the window icon because there's an error somewhere in SDL here.");
  //**There is an error here
//  Gu::SDLTrySetWindowIcon(_pWindow, "./data-dc/tex/tx64-icon.png");//_pApp->getIconFullPath());
#endif
}
void GraphicsWindow::initRenderSystem() {
  if (_pSDLWindow == nullptr) {
    BroThrowException("You need to make the SDL window before initializing render system.");
  }

  SDLUtils::trySetWindowIcon(_pSDLWindow, Gu::getApp()->getIconFullPath());

  if (Gu::getConfig()->getForceAspectRatio()) {
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
      BroLogError("SDL_GetDesktopDisplayMode failed: " + SDL_GetError());
    }
    else {
      float ar = (float)dm.h / (float)dm.w;
      int newHeight = (int)((float)_iLastWidth * ar);
      if (newHeight != _iLastHeight) {
        BroLogInfo("Adjusting window dims from " + _iLastWidth + " x " + _iLastHeight + " to " + _iLastWidth + " x " + newHeight);
        _iLastHeight = newHeight;
      }
    }
  }

  BroLogInfo("Setting window size to, " + _iLastWidth + " x " + _iLastHeight);
  SDL_SetWindowSize(_pSDLWindow, _iLastWidth, _iLastHeight);
  updateWidthHeight(_iLastWidth, _iLastHeight, true);

  if (Gu::getConfig()->getStartFullscreen() == true) {
    BroLogInfo("Setting window fullscreen.");
    toggleFullscreen();
  }

  createRenderPipe();


}

void GraphicsWindow::updateWidthHeight(uint32_t w, uint32_t h, bool bForce) {
  //update view/cam
  if (_iLastWidth != w || bForce) {
    _pWindowViewport->setWidth(w);
    if (_iLastHeight != h || bForce) {
      _pWindowViewport->setHeight(h);
    }
    if (_iLastHeight != h || _iLastWidth != w || bForce) {
      if (_pRenderPipe != nullptr) {
        _pRenderPipe->resizeScreenBuffers((int32_t)w, (int32_t)h);
      }
      //   _pApp->screenChanged(w, h, _bFullscreen);

      if (_pScreen) {
        _pScreen->screenChanged(w, h, _bFullscreen);
      }

    }
    _iLastWidth = w;
    _iLastHeight = h;
  }
}
void GraphicsWindow::toggleFullscreen() {
  if (_bFullscreen == false) {
    //get the fullscreen resolution
    int32_t iFsW = Gu::getEngineConfig()->getFullscreenWidth();
    int32_t iFsH = Gu::getEngineConfig()->getFullscreenHeight();
    if (iFsW <= 0 || iFsH <= 0) {
      SDL_DisplayMode DM;
      SDL_GetCurrentDisplayMode(0, &DM);
      iFsW = DM.w;
      iFsH = DM.h;
    }
    //Save pre-fullscreen width/height
    _iFullscreenToggleWidth = _iLastWidth;
    _iFullscreenToggleHeight = _iLastHeight;
    SDL_SetWindowSize(_pSDLWindow, iFsW, iFsH);

    if (SDL_SetWindowFullscreen(_pSDLWindow, SDL_WINDOW_FULLSCREEN) != 0) {
      BroLogError("Failed to go fullscreen.");
    }
    else {
      _bFullscreen = true;
      //_pApp->screenChanged(iFsW, iFsH, _bFullscreen);
    }
  }
  else {
    if (_iFullscreenToggleWidth > 0 && _iFullscreenToggleHeight > 0) {
      //Restore pre-fullscreen width/height
      SDL_SetWindowSize(_pSDLWindow, _iFullscreenToggleWidth, _iFullscreenToggleHeight);
    }
    if (SDL_SetWindowFullscreen(_pSDLWindow, 0) != 0) {
      BroLogError("Failed to exit fullscreen.");
    }
    else {
      _bFullscreen = false;
      //_pApp->screenChanged(_iLastWidth, _iLastHeight, _bFullscreen);
    }
  }

}
void GraphicsWindow::printHelpfulDebug() {
  int dw, dh;
  SDL_DisplayMode mode;
  SDL_Window* win = _pSDLWindow;

  SDL_GetCurrentDisplayMode(0, &mode);
  BroLogInfo("Screen BPP    : " + SDL_BITSPERPIXEL(mode.format));
  BroLogInfo("Swap Interval : " + SDL_GL_GetSwapInterval());
  SDL_GetWindowSize(win, &dw, &dh);
  BroLogInfo("Initial Window Size   : " + dw + "x" + dh);
  SDL_GL_GetDrawableSize(win, &dw, &dh);
  BroLogInfo("Draw Size     : " + dw + "x" + dh);


  SDLUtils::checkSDLErr();
}
void GraphicsWindow::createRenderPipe() {
  //Deferred Renderer
  _pRenderPipe = std::make_shared<RenderPipe>(getThis<GraphicsWindow>());
  _pRenderPipe->init(getViewport()->getWidth(), getViewport()->getHeight(), Gu::getApp()->makeAssetPath(Gu::getApp()->getEnvTexturePath()));
  // Gu::setRenderPipe(_pRenderPipe);

  //_pRenderPipe->getPipeBits().set();
}
void GraphicsWindow::beginRender() {

  //Make this window current *critical*
  //OPTIMIZE:TODO:NOTE: if there is only 1 window we don't have to call this.
  makeCurrent();

  //Update the widnow size
  int w, h;
  getDrawableSize(&w, &h);
  updateWidthHeight(w, h, false);

  Perf::pushPerf();
}
void GraphicsWindow::endRender() {
  swapBuffers();

  Perf::popPerf();
}
void GraphicsWindow::step() {
  
  //Managers
  if (_pDelta != nullptr) {
    _pDelta->update();
  }
  if (_pParty != nullptr) {
    _pParty->update(getDelta()->get());
  }
  if (_pFpsMeter != nullptr) {
    _pFpsMeter->update();
  }
  if (_pPicker != nullptr) {
    _pPicker->update(Gu::getInputManager());
  }

  beginRender();
  {
    if (Gu::getInputManager()->keyPress(SDL_SCANCODE_F11)) {
      toggleFullscreen();
    }

    getGraphicsContext()->setLoopState(EngineLoopState::SyncBegin);
    getFrameSync()->syncBegin();
    {
      getGraphicsContext()->setLoopState(EngineLoopState::Update);

      //...?

      //App Update steps.
     // Gu::getApp()->step((float)_fDelta);

      getGraphicsContext()->setLoopState(EngineLoopState::Render);

      RenderPipe::PipeBits pipebits;
      pipebits.set();
      //Gu::getRenderSettings()->getDOF()

      //Main Render
      _pRenderPipe->renderScene(this->shared_from_this(), Gu::getApp(), pipebits);
    }
    getGraphicsContext()->setLoopState(EngineLoopState::SyncEnd);
    getFrameSync()->syncEnd();
  }

  endRender();

  //Do not remove
  getGraphicsContext()->chkErrRt();

}
void GraphicsWindow::checkErrorsDbg() {
  getGraphicsContext()->chkErrDbg();
}
void GraphicsWindow::checkErrorsRt() {
  getGraphicsContext()->chkErrRt();
}


}//ns Game