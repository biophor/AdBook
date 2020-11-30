#include "stdafx.h"
#include "AContactPhotoProvider.h"

namespace qmladbook
{
    AContactPhotoProvider::AContactPhotoProvider()
        : QQuickImageProvider(QQmlImageProviderBase::ImageType::Pixmap)
    {

    }

    AContactPhotoProvider::~AContactPhotoProvider() = default;

}