#include "stdafx.h"
#include "ClipboardService.h"

namespace qmladbook
{
    void ClipboardService::PutText(const QString & data)
    {
        QClipboard * clipboard = QGuiApplication::clipboard();
        clipboard->setText(data);
    }
}
