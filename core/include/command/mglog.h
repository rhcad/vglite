#ifndef TOUCHVG_LOG_H
#define TOUCHVG_LOG_H

#ifndef LOGD

#ifdef __ANDROID__
#include <android/log.h>
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"vgjni",__VA_ARGS__)
#else
#define LOGD(...)
#endif

#endif // LOGD

#endif // TOUCHVG_LOG_H
