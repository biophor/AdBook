#pragma once
class MainWndSettings
{
public:
    MainWndSettings::MainWndSettings();
    MainWndSettings::~MainWndSettings();    // 'virtual' is not needed

    void SetRect(const CRect & rect);
    const CRect & GetRect() const;
    void SetFilterColWidth(const std::vector<int> & fcw);
    std::vector<int> GetFilterColWidth() const;
    void CloseToTray(const bool close);
    bool CloseToTray() const;
    void MinimizeToTray(const bool minimize);
    bool MinimizeToTray() const;
private:
    bool closeToTray_ = false;
    bool minimizeToTray_ = false;
    CRect rect_;
    std::vector<int> filterColWidth_;
};

