//#include "../gfx/.h"
#include "../gfx/RenderSettings.h"


namespace Game {
///////////////////////////////////////////////////////////////////


void RenderSettings::init(){
    _pRenderDebug = std::make_shared<RenderDebug>();
}


}//ns Game