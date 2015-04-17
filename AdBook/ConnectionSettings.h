#pragma once

class ConnectionSettings
{
public:
    ConnectionSettings();
    void SetDC(const CString & dc);
    CString GetDC() const;
    void SetLogin(const CString & login);
    void SetPassword(const CString & password);
    void CurrentUserCredentials(const bool currentUserCred);
    bool CurrentUserCredentials() const;
    void CurrentDomain(const bool currentDomain);
    bool CurrentDomain() const;
    CString GetLogin() const;
    CString GetPassword() const;
    void ForgetPassword(const bool forget);
    bool ForgetPassword() const;
    void DisplayPassword(const bool display);
    bool DisplayPassword() const;
private:
    CString dc_;    // domain controller name
    bool currentUserCred_ = true;
    bool currentDomain_ = true;
    bool displayPassword_ = false;
    bool forgetPassword_ = true;
    CString login_, password_;
};

