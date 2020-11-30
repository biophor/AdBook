#include "QmlAdBookTestSuiteBase.h"


qmladbook::ConnectionSettings QmlAdBookTestSuiteBase::GetConnectionSettingsForTestAdLdsInstance() {
    qmladbook::ConnectionSettings settings;
    settings.ConnectCurrentDomain(false);
    settings.ConnectAsCurrentUser(true);
    settings.SetAddress("LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");
    return settings;
}