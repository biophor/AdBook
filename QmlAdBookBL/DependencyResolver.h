#pragma once
#include "qmladbookbl_global.h"

#include "ADependencyResolver.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT DependencyResolver: public ADependencyResolver
    {
    public:
        std::shared_ptr<ASettingsKeeper> GetSettingsKeeper() override;
        std::shared_ptr<AContactsKeeper> GetContactsKeeper() override;
        std::shared_ptr<adbook::AbstractAdAccessFactory> GetAdFactory() override;
        std::shared_ptr<AContactUpdater> GetContactUpdater() override;
        std::shared_ptr<ADataExchanger> GetDataExchanger() override;
        std::shared_ptr<AContactFinder> GetContactFinder() override;
        AContactPhotoProvider * GetContactPhotoProvider() override;
        std::shared_ptr<ASettingsChecker> GetSettingsChecker() override;
        std::shared_ptr<AClipboardService> GetClipboardService() override;
        std::shared_ptr<AFileSystemService> GetFileSystemService() override;
    protected:
        std::shared_ptr<ASettingsKeeper> _settingsKeeper;
        std::shared_ptr<AContactsKeeper> _contactsKeeper;
        std::shared_ptr<adbook::AbstractAdAccessFactory> _factory;
        std::shared_ptr<AContactUpdater> _contactUpdater;
        std::shared_ptr<ADataExchanger> _dataExchanger;
        std::shared_ptr<AContactFinder> _contactFinder;
        std::shared_ptr<ASettingsChecker> _settingsChecker;
        std::shared_ptr<AClipboardService> _clipboardService;
        std::shared_ptr <AFileSystemService> _fileSystemService;
        AContactPhotoProvider * _photoProvider{};
    };
}

