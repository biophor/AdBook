#pragma once
#include "adsi.h"
#include "ConnectionSettings.h"

class AdConnector
{
public:
    AdConnector(const ConnectionSettings & cs);
    ~AdConnector();

    void Connect();
    bool IsConnected() const;
    IDirectoryObjectPtr GetDirectoryObject() const;
    
private:
    IDirectoryObjectPtr objectPtr_;
    ConnectionSettings cs_;
};

