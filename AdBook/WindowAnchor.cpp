#include "stdafx.h"
#include "WindowAnchor.h"


class WndItem
{
	friend class WindowAnchor;
private:

    WndItem() : wndHandle_(NULL), next_(NULL), flags_(0), cxLeft_(1.0f), cxRight_(1.0f),
        cyTop_(1.0f), cyBottom_(1.0f), initRect_(rect_)
    {        
    }

    WndItem(HWND hWnd, UINT uFlag, const CRect & rect, float cxLeft = 1.0f, float cxRight = 1.0f,
        float cyTop = 1.0f, float cyBottom = 1.0f
    )
        : wndHandle_(hWnd), next_(NULL), flags_(uFlag), cxLeft_(cxLeft), cxRight_(cxRight),
          cyTop_(cyTop), cyBottom_(cyBottom), rect_(rect), initRect_(rect_)
    {        
    }
    
private:
    HWND wndHandle_;
    UINT flags_;
    CRect rect_;
    CRect initRect_;
    float cxLeft_, cxRight_, cyTop_, cyBottom_;
    WndItem * next_;  // next item
};

WindowAnchor::WindowAnchor(): nextControl_(new WndItem)
{
    wndHandle_ = NULL;    
    initRect_ = rect_;
}

bool WindowAnchor::Init(HWND wndHandle)
{
    if (IsWindow(wndHandle))
    {
        wndHandle_ = wndHandle;

        if (GetClientRect(wndHandle_, &rect_))
        {
            initRect_ = rect_;
            return true;
        }
    }
    return false;
}

bool WindowAnchor::Add(HWND wndHandle, UINT flags, float cxLeft, float cxRight, float cyTop, float cyBottom)
{
    WndItem * item = nextControl_.get();
    while (NULL != item->next_)
    {
        item = item->next_;
    }
    CRect rect;
    if (GetWindowRect(wndHandle, &rect))
    {     
        ScreenToClient(wndHandle_, &rect.TopLeft());     
        ScreenToClient(wndHandle_, &rect.BottomRight());
        item->next_ = new WndItem(wndHandle, flags, &rect, cxLeft, cxRight, cyTop, cyBottom);
    }
    return item->next_ ? true: false;
}

bool WindowAnchor::Add(UINT id, UINT flags, float cxLeft, float cxRight, float cyTop, float cyBottom)
{
    return Add(GetDlgItem(wndHandle_, id), flags, cxLeft, cxRight, cyTop, cyBottom);
}

bool WindowAnchor::Remove(UINT id)
{
    return Remove(GetDlgItem(wndHandle_, id));
}

bool WindowAnchor::Remove(HWND wndHandle)
{
    WndItem * item = nextControl_.get();

    while (NULL != item->next_)
    {
        if (item->next_->wndHandle_ == wndHandle)
        {
            WndItem * s = item->next_->next_;
            delete item->next_;
            item->next_ = s;
            return true;
        }
        item = item->next_;
    }
    return false;
}

bool WindowAnchor::Update(UINT id)
{
    return Update(GetDlgItem(wndHandle_, id));
}

bool WindowAnchor::Update(HWND wndHandle)
{
    WndItem * item = nextControl_->next_;

    while (NULL != item)
    {
        if (item->wndHandle_ == wndHandle)
        {
            if (GetWindowRect(wndHandle, &item->rect_))
            {
                ScreenToClient(wndHandle_, &item->rect_.TopLeft());
                ScreenToClient(wndHandle_, &item->rect_.BottomRight());
                return true;
            }
        }
        item = item->next_;
    }
    return false;
}

void WindowAnchor::UpdateAll()
{
    WndItem * item = nextControl_->next_;

    while (NULL != item)
    {
        if (GetWindowRect(item->wndHandle_, &item->rect_))
        {
            ScreenToClient(wndHandle_, &item->rect_.TopLeft());
            ScreenToClient(wndHandle_, &item->rect_.BottomRight());
        }
        item = item->next_;
    }
}

void WindowAnchor::RemoveAll()
{
    RemoveIt(nextControl_->next_);
}

WindowAnchor::~WindowAnchor()
{
    RemoveAll();
}

BOOL WindowAnchor::OnSize()
{
    const int ControlCount = GetControlCount();
    if (0 == ControlCount)
    {
        return FALSE;
    }    
    CRect rect;
    HDWP hdwp = BeginDeferWindowPos(ControlCount);

    if (FALSE == GetClientRect(wndHandle_, &rect))
    {
        return FALSE;
    }

    WndItem * item = nextControl_->next_;

    while (NULL != item)
    {
        // horizontal
        if ((item->flags_ & Left) && (item->flags_ & Right))
        {
            const LONG wndDiff = rect.right - initRect_.right;
            const LONG ctlRDiff = static_cast<LONG>(ceilf(wndDiff * item->cxRight_));
            item->rect_.right = item->initRect_.right + ctlRDiff;

            const LONG ctlLDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item->cxLeft_));
            item->rect_.left = item->initRect_.left + ctlLDiff;
        }
        else if (item->flags_ & Left)
        {
            const LONG wndDiff = rect.right - initRect_.right;
            const LONG ctlDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item->cxLeft_));
            item->rect_.left = item->initRect_.left + ctlDiff;
        }
        else if (item->flags_ & Right)
        {
            const LONG wndDiff = rect.right - initRect_.right;
            const LONG ctlDiff = static_cast<LONG>(ceilf(wndDiff * item->cxRight_));
            item->rect_.right = item->initRect_.right + ctlDiff;
            item->rect_.left = item->initRect_.left + ctlDiff;
        }
        else
        {
            // relative move
            const LONG sx = ((rect.right - rect.left) / 2) - ((rect_.right - rect_.left) / 2);
            item->rect_.right += sx;
            item->rect_.left += sx;
        }
        // vertical
        if ((item->flags_ & Top) && (item->flags_ & Bottom))
        {
            const LONG wndDiff = rect.bottom - initRect_.bottom;
            const LONG ctlBDiff = static_cast<LONG>(ceilf(wndDiff * item->cyBottom_));
            item->rect_.bottom = item->initRect_.bottom + ctlBDiff;

            const LONG ctlTDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item->cyTop_));
            item->rect_.top = item->initRect_.top + ctlTDiff;
        }
        else if (item->flags_ & Top)
        {
            const LONG wndDiff = rect.bottom - initRect_.bottom;
            const LONG ctlDiff = wndDiff - static_cast<LONG>(ceilf(wndDiff * item->cyTop_));
            item->rect_.top = item->initRect_.top + ctlDiff;
        }
        else if (item->flags_ & Bottom)
        {
            const LONG wndDiff = rect.bottom - initRect_.bottom;
            const LONG ctlDiff = static_cast<LONG>(ceilf(wndDiff * item->cyBottom_));
            item->rect_.bottom = item->initRect_.bottom + ctlDiff;
            item->rect_.top = item->initRect_.top + ctlDiff;
        }
        else
        {
            // relative move
            const LONG sy = ((rect.bottom - rect.top) / 2) - ((rect_.bottom - rect_.top) / 2);
            item->rect_.bottom += sy;
            item->rect_.top += sy;
        }

        if (item->flags_ & DockTop)    item->rect_.top = rect.top;
        if (item->flags_ & DockLeft)   item->rect_.left = rect.left;
        if (item->flags_ & DockRight)  item->rect_.right = rect.right;
        if (item->flags_ & DockBottom) item->rect_.bottom = rect.bottom;
        
        DeferWindowPos( hdwp, item->wndHandle_, NULL, item->rect_.left, item->rect_.top,
            item->rect_.right - item->rect_.left, item->rect_.bottom - item->rect_.top,
            SWP_NOZORDER );
        item = item->next_;
    }
    rect_ = rect;
    EndDeferWindowPos(hdwp);
    return TRUE;
}

void WindowAnchor::RemoveIt(WndItem * item)
{
    if (NULL != item) {
        RemoveIt(item->next_);
        delete item;
    }
}

int WindowAnchor::GetControlCount()
{
    int count = 0;
    WndItem * NextItem = nextControl_->next_;
    
    while (NextItem)
    {
        ++count;
        NextItem = NextItem->next_;
    }
    return count;
}
