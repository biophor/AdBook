#pragma once

#include <memory>
#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"

using namespace qmladbook;

class DependencyResolverForTesting : public DependencyResolver
{
public:
    /*std::shared_ptr<AbstractSettingsKeeper> GetSettingsKeeper() override;
    std::shared_ptr<AbstractContactsKeeper> GetContactsKeeper() override;
    std::shared_ptr<adbook::AbstractAdAccessFactory> GetAdFactory() override;
    std::shared_ptr<AbstractContactUpdater> GetContactUpdater() override;
    std::shared_ptr<AbstractDataExchanger> GetDataExchanger() override;
    std::shared_ptr<AbstractContactFinder> GetContactFinder() override;
    std::shared_ptr<AbstractSettingsChecker> GetSettingsChecker() override;
    */
    AContactPhotoProvider * GetContactPhotoProvider() override;


};

