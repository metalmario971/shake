/**
*
*    @file WindowManager.h
*    @date February 7, 2020
*    @author MetalMario971
*
*    � 2020 
*
*
*/
#pragma once
#ifndef __WINDOWMANAGER_1581056461792934002_H__
#define __WINDOWMANAGER_1581056461792934002_H__

#include "../app/AppHeader.h"

namespace Game {
/**
*    @class WindowManager
*    @brief
*
*/
class WindowManager : public VirtualMemory {
  std::vector<std::shared_ptr<GraphicsWindow>> _windows;
public:
    WindowManager();
    virtual ~WindowManager() override;
    std::shared_ptr<GraphicsWindow> createWindow(std::string title, RenderSystem::e re = RenderSystem::e::OpenGL);
    void step();
};

}//ns Game



#endif