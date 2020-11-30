#include <memory>
#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "DependencyResolverForTesting.h"

int main(int argc, char *argv[])
{
    //QGuiApplication a(argc, argv);
    //a.setApplicationName("acme_app");
    //a.setOrganizationName("acme_org");

    //return a.exec();

    SetDependencyResolver(std::make_shared<DependencyResolverForTesting>());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
