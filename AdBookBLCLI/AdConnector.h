/*
Copyright (C) 2015-2020 Andrei Goncharov.

This file is part of the 'Active Directory Contact Book'.
'Active Directory Contact Book' is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

'Active Directory Contact Book' is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
'Active Directory Contact Book'. If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "NativeObjectPtr.h"
#include "ConnectionParams.h"

namespace adbookcli
{
using System::String;
using System::Byte;

using System::Runtime::CompilerServices::RuntimeHelpers;

public ref class NativeAdConnectorPtr : NativeAbstractObjectPtr<adbook::AbstractAdConnector>
{
public:
    NativeAdConnectorPtr(adbook::AbstractAdConnector * adc)
        : NativeAbstractObjectPtr<adbook::AbstractAdConnector>(adc) {}

protected:
    virtual void ReleaseNativeResources() override;
};


public ref class AdConnector sealed
{
public:
    explicit AdConnector(adbook::AbstractAdConnector * nativeAdConnector);

    void Connect(ConnectionParams ^ cs);
    void Connect(ConnectionParams ^ cs, String ^ dn);
    void Disconnect();
    bool IsConnected();

    void Rename(String ^ newName);
    void UploadStringAttr(String ^ attrName, String ^ attrVal);
    String ^ DownloadStringAttr(String ^ attrName);

    void UploadBinaryAttr(String ^ attrName, cli::array<Byte> ^ bav);
    cli::array<Byte> ^ DownloadBinaryAttr(String ^ attrName);
private:
    NativeAdConnectorPtr _nativeAdConnectorPtr;
};

}   // adbookcli

