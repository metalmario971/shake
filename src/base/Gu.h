    /**
*
*    @file Gu.h
*    @date November 6, 2016
*    @author Derek Page
*
*    � 2016 
*
*
*/
#pragma once
#ifndef __GU_14784108333124451735_H__
#define __GU_14784108333124451735_H__

#include "../base/BaseHeader.h"
#include "../gfx/GfxHeader.h"
#include "../math/MathHeader.h"
#include "../world/WorldHeader.h"
#include "../base/Stopwatch.h"


namespace Game {
/**
*    @class Gu
*    @brief Global Utils For everything.
*
*/
class Gu : public GameMemory {
    //Required Globals (cannot be NULL)
    static std::shared_ptr<GraphicsContext> _pContext;
    static std::shared_ptr<TexCache> _pTexCache;
    static std::shared_ptr<CameraNode> _pCamera;
    static std::shared_ptr<Party> _pParty;
    static std::shared_ptr<Sequencer> _pSequencer;
    static std::shared_ptr<AppBase> _pAppBase;
    static std::shared_ptr<Fingers> _pFingers;
    static std::shared_ptr<FpsMeter> _pFpsMeter;
    static std::shared_ptr<FrameSync> _pFrameSync;
    static std::shared_ptr<SoundCache> _pSoundCache;
    static std::shared_ptr<ShaderMaker> _pShaderMaker;
    static std::shared_ptr<LightManager> _pLightManager;
    static std::shared_ptr<ModelCache> _pModelCache;
    static std::shared_ptr<Picker> _pPicker;
    static std::shared_ptr<PhysicsWorld> _pPhysicsWorld;
    static std::shared_ptr<Package> _pPackage;
    static std::shared_ptr<RenderSettings> _pRenderSettings;
    static std::shared_ptr<Gui2d> _pGui2d;
    static std::shared_ptr<RenderPipe> _pRenderPipe;
    static std::shared_ptr<EngineConfig> _pEngineConfig;
    static std::shared_ptr<Logger> _pLogger;
    static std::shared_ptr<Engine> _pEngine;
    static std::shared_ptr<GraphicsApi> _pGraphicsApi;

    static t_string _strCachedProf ;
    static std::stack<Stopwatch> _stopw;

	static int _iSupportedDepthSize;

    static void pulsePerf();
    
public: 
    static std::shared_ptr<GLContext> getGraphicsContext();
    static std::shared_ptr<RenderSettings> getRenderSettings();
    static std::shared_ptr<Package> getPackage();
    static std::shared_ptr<ModelCache> getModelCache();
    static std::shared_ptr<Sequencer> getSequencer();
    static std::shared_ptr<Fingers> getFingers();
    static std::shared_ptr<FpsMeter> getFpsMeter();
    static std::shared_ptr<FrameSync> getFrameSync();
    static std::shared_ptr<SoundCache> getSoundCache();
    static std::shared_ptr<ShaderMaker> getShaderMaker();
    static std::shared_ptr<AppBase> getApp();
    static std::shared_ptr<TexCache> getTexCache();
    static std::shared_ptr<LightManager> getLightManager();
    static std::shared_ptr<Picker> getPicker();
    static std::shared_ptr<Gui2d> getGui();
    static std::shared_ptr<PhysicsWorld> getPhysicsWorld();
    static std::shared_ptr<Party> getParty();
    static std::shared_ptr<CameraNode> getCamera();
    static std::shared_ptr<EngineConfig> getConfig();
    static std::shared_ptr<RenderPipe> getRenderPipe();
    static std::shared_ptr<Logger> getLogger();
    static std::shared_ptr<EngineConfig> getEngineConfig();
    static std::shared_ptr<Engine> getEngine();
    static std::shared_ptr<GraphicsApi> getGraphicsApi();
    static std::shared_ptr<WindowViewport> getViewport();

    static std::shared_ptr<Window> getWindow();

    static void setContext(std::shared_ptr<GraphicsContext> rb);
    static void setRenderPipe(std::shared_ptr<RenderPipe> r);
    static void setPhysicsWorld(std::shared_ptr<PhysicsWorld> p);
    static void setCamera(std::shared_ptr<CameraNode> pc);
    static void setRoom(std::shared_ptr<AppBase> b);
    static void setEngine(std::shared_ptr<Engine> engine);
    static void setGraphicsApi(std::shared_ptr<GraphicsApi> api);

    static void update(float dt);

    static void createManagers();


    static void checkErrorsDbg();
    static void checkErrorsRt();
    static void initGlobals(std::shared_ptr<AppBase> rb, std::vector<std::string>& args);
    static void deleteGlobals();

    static void debugBreak();
    static void beginPerf();
    static void endPerf();
    static void pushPerf();
    static void popPerf();

    static bool isDebug();

    static t_timeval  getMicroSeconds();
    static t_timeval  getMilliSeconds();

    static bool is64Bit();
    static bool isBigEndian();

    static std::shared_ptr<Img32> loadImage(std::string path);
    static bool saveImage(std::string path, std::shared_ptr<Img32> spec);
    static void freeImage(std::shared_ptr<Img32> b);
    
    static int loadSound(std::string path, int& iChannels, int& iSampleRate, int16_t*& pData, int& nSamples, int& iDataLenBytes);

    static std::vector<t_string> argsToVectorOfString(int argc, char** argv, char delimiter = '=');

    static void inlineDrawBoxCont(const Box3f* b);
    static float fade(float t);
    static void checkMemory();
    static t_string getOperatingSystemName();
    static uint32_t getCurrentThreadId();
    static int32_t getSupportedDepthSize();
    static void setSupportedDepthSize(int32_t ee) { _iSupportedDepthSize =ee; }

    static std::string getCPPVersion();

    static void print(char msg);
    static void print(const char* msg);
    static void print(const t_string& msg);
    static uint64_t getFrameNumber();

    static void guiQuad2d(Box2f& pq, std::shared_ptr<WindowViewport> vp);//Transforms a quad for the matrix-less 

    template < typename Tx >
    static bool addIfDoesNotContain(std::vector<Tx>& vec, Tx& rhs) { 
        for(size_t i=0; i<vec.size(); ++i){
            if(vec[i] == rhs) { 
                return false;
            }
        }
        vec.push_back(rhs);
        return true;
    }
    template <typename Tx> 
    static void incrementEnum(Tx& __in_ eValue, Tx iMaxValue){
        eValue = (Tx)((int)eValue + 1);
        if ((int)eValue >= iMaxValue) { 
            eValue = (Tx)0;
        }
    }

};

#define Graphics Gu::getGraphicsContext()
#define Config Gu::getEngineConfig()


}//ns Game



#endif
