#include "stdafx.h"
#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>

void SetupApplicationProperties()
{
    QGuiApplication::setApplicationName("Active Directory Contact Book");
    QGuiApplication::setOrganizationName("biophor");
    QGuiApplication::setOrganizationDomain("biophor.local");
    QGuiApplication::setWindowIcon(QIcon(":/app.ico"));
}

void RegisterCustomTypesForQML()
{
    qmlRegisterType<qmladbook::SettingsDlgViewModel>("biophor.local", 1, 0, "SettingsDlgViewModel");
    qmlRegisterType<qmladbook::MainWindowViewModel>("biophor.local", 1, 0, "MainWindowViewModel");
    qmlRegisterType<qmladbook::AttrEditorViewModel>("biophor.local", 1, 0, "AttrEditorViewModel");
    qmlRegisterType<qmladbook::ContactsModel>("biophor.local", 1, 0, "ContactsModel");
    qmlRegisterType<qmladbook::ContactDetailsModel>("biophor.local", 1, 0, "ContactDetailsModel");
    qmlRegisterType<qmladbook::SearchFilterTypesModel>("biophor.local", 1, 0, "SearchFilterTypesModel");
    qmlRegisterType<qmladbook::SearchFilterRulesModel>("biophor.local", 1, 0, "SearchFilterRulesModel");
    qmlRegisterType<qmladbook::SearchFiltersModel>("biophor.local", 1, 0, "SearchFiltersModel");
}

void LoadApplicationSettings()
{
    auto dependencyResolver = qmladbook::GetDependencyResolver();
    dependencyResolver->GetSettingsKeeper()->Load(
        QGuiApplication::organizationName(),
        QGuiApplication::applicationName()
    );
}

void SetupUiStyleProperties()
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QQuickStyle::setStyle("Fusion");
}

void SaveApplicationSettings()
{
    auto dependencyResolver = qmladbook::GetDependencyResolver();
    dependencyResolver->GetSettingsKeeper()->Save(
        QGuiApplication::organizationName(),
        QGuiApplication::applicationName()
    );
}

void FixTheBoldFontProblem_Qt_5_12_6()
{
    qputenv("QML_DISABLE_DISTANCEFIELD", "1");
}

void SetupContactPhotoImageProvider(QQmlApplicationEngine & engine)
{
    auto dependencyResolver = qmladbook::GetDependencyResolver();
    auto contactPhotoProvider = dependencyResolver->GetContactPhotoProvider();
    engine.addImageProvider(contactPhotoProvider->GetProviderName(), contactPhotoProvider);
    // now the engine owns contactPhotoProvider. the engine will delete contactPhotoProvider.
}

bool CreateMainWindow(QQmlApplicationEngine & engine)
{
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    if (engine.rootObjects().isEmpty()) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FixTheBoldFontProblem_Qt_5_12_6();
    SetupApplicationProperties();
    RegisterCustomTypesForQML();
    SetupUiStyleProperties();
    LoadApplicationSettings();

    QQmlApplicationEngine engine;
    SetupContactPhotoImageProvider(engine);

    if (!CreateMainWindow(engine)) {
        return -1;
    }
    const int exitCode = app.exec();
    SaveApplicationSettings();
    return exitCode;
}

