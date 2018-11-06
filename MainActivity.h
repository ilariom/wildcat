#ifndef _WKT_MAIN_ACTIVITY_H
#define _WKT_MAIN_ACTIVITY_H

#include "config.h"

namespace wkt
{

class MainActivity
{
public:
    MainActivity() = default;
    MainActivity(const MainActivity&) = delete;
    MainActivity(MainActivity&&) = delete;
    ~MainActivity() = default;

    MainActivity& operator=(const MainActivity&) = delete;
    MainActivity& operator=(MainActivity&) = delete;
    
public:
    void onCreate(StartupConfig& conf);
    void onStart();
    void onResume();
    void onPause();
    void onStop();
    void onDestroy();
};

}

#endif