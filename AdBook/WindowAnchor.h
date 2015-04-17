#pragma once

class WndItem;

class WindowAnchor
{
public:
    enum
    {
        Top=0x1, Left=0x2, Right=0x4, Bottom=0x8, TopLeft=0x3, TopRight=0x5, BottomLeft=0xA,
        BottomRight=0xC, All=0xF, DockTop=0x100, DockLeft=0x200, DockRight=0x400, DockBottom=0x800, 
        DockFill=0xF00
    };
    WindowAnchor();
    bool Init(HWND windowHandle);
    bool Add(HWND hWnd, UINT flag,float cxLeft = 1.0f, float cxRight = 1.0f, float cyTop = 1.0f,
        float cyBottom = 1.0f);
    bool Add(UINT id, UINT flag, float cxLeft = 1.0f, float cxRight = 1.0f, float cyTop = 1.0f,
        float cyBottom = 1.0f);
    bool Remove(UINT id);
    bool Remove(HWND hWnd);
    bool Update(UINT id);
    bool Update(HWND wndHandle);
    void UpdateAll();
    void RemoveAll();
    ~WindowAnchor();
    BOOL OnSize();
    
private:    
    void RemoveIt(WndItem * item);
    int GetControlCount();
    
    HWND wndHandle_;
    CRect initRect_;
    CRect rect_;
    std::shared_ptr<WndItem> nextControl_;
};
