#include <QSettings>
#include <QCoreApplication>
#include <QFileInfo>

struct AppConfig
{
    QString defaultModelDir;
    QString defaultModel;
    QString defaultIBL;
    QString defaultImguiTtf;
};

static AppConfig loadAppConfig()
{
    QString configPath =
        QCoreApplication::applicationDirPath() + "/config/app.ini";

    QSettings settings(configPath, QSettings::IniFormat);

    AppConfig cfg;
    cfg.defaultModelDir = settings.value("DefaultAssets/DefaultModelDir").toString();
    cfg.defaultModel = settings.value("DefaultAssets/DefaultModel").toString();
    cfg.defaultIBL = settings.value("DefaultAssets/DefaultIBL").toString();
    cfg.defaultImguiTtf = settings.value("DefaultAssets/defaultImguiTtf").toString();

    return cfg;
}
