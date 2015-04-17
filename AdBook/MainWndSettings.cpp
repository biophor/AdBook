#include "stdafx.h"
#include "MainWndSettings.h"

MainWndSettings::MainWndSettings()
{
}

MainWndSettings::~MainWndSettings()
{
}

void MainWndSettings::SetRect(const CRect & rect)
{
    rect_ = rect;
}

const CRect & MainWndSettings::GetRect() const
{
    return rect_;
}

void MainWndSettings::SetFilterColWidth(const std::vector<int>& fcw)
{
    filterColWidth_ = fcw;
}

std::vector<int> MainWndSettings::GetFilterColWidth() const
{
    return filterColWidth_;
}

void MainWndSettings::CloseToTray(const bool close)
{
    closeToTray_ = close;
}

bool MainWndSettings::CloseToTray() const
{
    return closeToTray_;
}

void MainWndSettings::MinimizeToTray(const bool minimize)
{
    minimizeToTray_ = minimize;
}

bool MainWndSettings::MinimizeToTray() const
{
    return minimizeToTray_;
}

