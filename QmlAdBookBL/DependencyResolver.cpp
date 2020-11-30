#include "stdafx.h"
#include "DependencyResolver.h"
#include "SettingsKeeper.h"
#include "ContactsKeeper.h"
#include "ContactUpdater.h"
#include "DataExchanger.h"
#include "ContactsKeeper.h"
#include "ContactFinder.h"
#include "SettingsChecker.h"
#include "ContactPhotoProvider.h"
#include "ClipboardService.h"
#include "FileSystemService.h"

namespace qmladbook {

    std::shared_ptr<ASettingsKeeper> DependencyResolver::GetSettingsKeeper()
    {
        if (!_settingsKeeper) {
            _settingsKeeper = std::make_shared<SettingsKeeper>();
        }
        return _settingsKeeper;
    }

    std::shared_ptr<AContactsKeeper> DependencyResolver::GetContactsKeeper()
    {
        if (!_contactsKeeper) {
            _contactsKeeper = std::make_shared<ContactsKeeper>();
        }
        return _contactsKeeper;
    }

    std::shared_ptr<adbook::AbstractAdAccessFactory> DependencyResolver::GetAdFactory()
    {
        return adbook::GetAdAccessFactory();
    }

    std::shared_ptr<AContactUpdater> DependencyResolver::GetContactUpdater() {
        if (!_contactUpdater)
        {
            _contactUpdater = std::make_shared<ContactUpdater> (
                GetAdFactory(),
                GetSettingsKeeper(),
                GetFileSystemService()
                );
        }
        return _contactUpdater;
    }

    std::shared_ptr<ADataExchanger> DependencyResolver::GetDataExchanger()
    {
        if (!_dataExchanger) {
            _dataExchanger = std::make_shared<DataExchanger>();
        }
        return _dataExchanger;
    }

    std::shared_ptr<AContactFinder> DependencyResolver::GetContactFinder()
    {
        if (!_contactFinder) {
            _contactFinder = std::make_shared<ContactFinder>(GetAdFactory());
        }
        return _contactFinder;
    }

    std::shared_ptr<ASettingsChecker> DependencyResolver::GetSettingsChecker() {
        if (!_settingsChecker) {
            _settingsChecker = std::make_shared<SettingsChecker>(GetAdFactory());
        }
        return _settingsChecker;
    }

    AContactPhotoProvider * DependencyResolver::GetContactPhotoProvider() {
        if (!_photoProvider) {
            _photoProvider = new qmladbook::ContactPhotoProvider();
        }
        return _photoProvider;
    }

    std::shared_ptr<AClipboardService> DependencyResolver::GetClipboardService() {
        if (!_clipboardService) {
            _clipboardService = std::make_shared<ClipboardService>();
        }
        return _clipboardService;
    }

    std::shared_ptr<AFileSystemService> DependencyResolver::GetFileSystemService()
    {
        if (!_fileSystemService) {
            _fileSystemService = std::make_shared<FileSystemService>();
        }
        return _fileSystemService;
    }
}

