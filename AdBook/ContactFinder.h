#pragma once
#include "ConnectionSettings.h"

class ContactFinder
{
public:
    ContactFinder();
    ~ContactFinder();

    void Start(const ConnectionSettings & cs);
    void Stop();
    bool IsSearching() const;

    typedef std::function<void()> OnStop;
    typedef std::function<void()> OnNewContact;
    
    void OnStopHandler(OnStop stopHandler);    
    void OnNewContactFoundHandler(OnNewContact newContact);

private:
    OnStop onStop_;
    OnNewContact onNewContact_;
    ConnectionSettings cs_;
};

