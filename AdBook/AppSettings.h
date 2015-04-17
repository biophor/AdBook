#pragma once
#include "ConnectionSettings.h"
#include "MainWndSettings.h"

class AppSettings
{
public:
    AppSettings();
    ~AppSettings();
    
    MainWndSettings & GetMainWndSettings() {
        return mws_;
    }
    const MainWndSettings & GetMainWndSettings() const {
        return mws_;
    }
    ConnectionSettings & GetConnectionSettings() {
        return cs_;
    }
    const ConnectionSettings & GetConnectionSettings() const {
        return cs_;
    }

    void SaveSettings();
    void LoadSettings();

private:
    ConnectionSettings cs_;
    MainWndSettings mws_;
};

