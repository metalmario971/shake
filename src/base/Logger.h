/**
*  @file Logger.h
*  @date December 14, 2013
*  @author MetalMario971
*/
#pragma once
#ifndef __LOGGER_16421047220665463212149_H__
#define __LOGGER_16421047220665463212149_H__

#include "../base/BaseHeader.h"
#include "../base/Gu.h"

namespace BR2 {
/**
*  @class Logger
*  @brief Logs to a text file in /log/ and/or the console.
*/
class Logger_Internal;
class Logger : public OperatingSystemMemory {
public:
  Logger(bool async, bool disabled = false);
  virtual ~Logger() override;

  void init(string_t cacheFolder);

  void enableLogToConsole(bool bLogToConsole);
  void enableLogToFile(bool bLogToFile);

  void logInfo(string_t msg);
  void logInfo(string_t msg, int line, char* file, BR2::Exception* e = nullptr, bool hideStackTrace = true);
  void logWarn(string_t msg, int line, char* file, BR2::Exception* e = nullptr, bool hideStackTrace = true);    // - Log an error with exception contents.
  void logError(string_t msg, int line, char* file, BR2::Exception* e = nullptr, bool hideStackTrace = false);    // - Log an error with exception contents.
  void logDebug(string_t msg, int line, char* file, BR2::Exception* e = nullptr, bool hideStackTrace = true);
  void logWarnCycle(string_t msg, int line, char* file, BR2::Exception* e = nullptr, int iCycle = 60, bool force = false);    // - Log an error with exception contents.
  void logErrorCycle(string_t msg, int line, char* file, BR2::Exception* e = nullptr, int iCycle = 60, bool force = false);    // - Log an error with exception contents.
  void logDebugCycle(string_t msg, int line, char* file, BR2::Exception* e = nullptr, int iCycle = 60, bool force = false);    // - Log an error with exception contents.

  string_t getLogPath();
private:
  Logger_Internal* _pint = nullptr;
};
}//ns game

#define BRLogDebug(x) BR2::Gu::getLogger()->logDebug(Stz x,__LINE__,__FILE__ )
#define BRLogInfo(x) BR2::Gu::getLogger()->logInfo(Stz x,__LINE__,__FILE__ )
#define BRLogWarn(x) BR2::Gu::getLogger()->logWarn(Stz x,__LINE__,__FILE__, nullptr )
#define BRLogError(x) BR2::Gu::getLogger()->logError(Stz x,__LINE__,__FILE__, nullptr )
#define BRLogErrorNoStack(x) BR2::Gu::getLogger()->logError(Stz x,__LINE__,__FILE__, nullptr, true)
#define BRLogErrorEx(x, aex) BR2::Gu::getLogger()->logError(x,__LINE__,__FILE__, aex )

#define BRLogErrorOnce(x) \
{ \
static bool ___logged_ = false; \
if(___logged_ == false) { \
___logged_ = true; \
BR2::Gu::getLogger()->logError(Stz x,__LINE__,__FILE__, nullptr ); \
} \
}
#define BRLogDebugOnce(x) \
{ \
static bool ___logged_ = false; \
if(___logged_ == false) { \
___logged_ = true; \
BR2::Gu::getLogger()->logDebug(Stz x,__LINE__,__FILE__); \
} \
}
#define BRLogWarnCycle(x)\
{\
static bool ___logged_ = true;\
BR2::Gu::getLogger()->logWarnCycle(Stz x, __LINE__, __FILE__, nullptr, 60, ___logged_);\
if (___logged_ == true) {\
    ___logged_ = false;\
}\
}
#define BRLogErrorCycle(x)\
{\
static bool ___logged_ = true;\
BR2::Gu::getLogger()->logErrorCycle(Stz x, __LINE__, __FILE__, nullptr, 60, ___logged_);\
if (___logged_ == true) {\
    ___logged_ = false;\
}\
}
#define BRLogDebugCycle(x)\
{\
static bool ___logged_ = true;\
BR2::Gu::getLogger()->logDebugCycle(Stz x, __LINE__, __FILE__, nullptr, 60, ___logged_);\
if (___logged_ == true) {\
    ___logged_ = false;\
}\
}


#endif
