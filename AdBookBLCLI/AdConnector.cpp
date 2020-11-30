// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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


#include "stdafx.h"
#include "AdConnector.h"
#include "InteropHelpers.h"

namespace adbookcli
{
using System::GC;
using System::Int32;
using System::Exception;
using System::Diagnostics::Trace;

void NativeAdConnectorPtr::ReleaseNativeResources()
{
    try {}
    finally{// preventing memory leaks due to ThreadAbortException
        try {
            adbook::AbstractAdConnector * adc = reinterpret_cast<adbook::AbstractAdConnector *>(handle.ToPointer());
            if (adc != nullptr) {
                if (adc->IsConnected()) {
                    adc->Disconnect();
                }
                delete adc;
                handle = IntPtr::Zero;
            }
        }
        catch (const adbook::Error & e) {
            MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        }
        catch (const std::exception & e) {
            MY_TRACE("%s %s", __FUNCTION__, e.what());
        }
    }
}


AdConnector::AdConnector(adbook::AbstractAdConnector * nativeAdConnector)
    : _nativeAdConnectorPtr(nativeAdConnector)
{
    GC::KeepAlive(this);
}

void AdConnector::Connect(ConnectionParams ^ cs)
{
    try {
        _nativeAdConnectorPtr->Connect(cs->ToUnderlyingType());
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

void AdConnector::Connect(ConnectionParams ^ cs, String ^ dn)
{
    try {
        std::wstring wdn = StringToStdWstring(dn);
        _nativeAdConnectorPtr->Connect(cs->ToUnderlyingType(), wdn);
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

void AdConnector::Disconnect()
{
    try {
        _nativeAdConnectorPtr->Disconnect();
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

bool AdConnector::IsConnected()
{
    try {
        auto isConnected = _nativeAdConnectorPtr->IsConnected();
        GC::KeepAlive(this);
        return isConnected;
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What()));
    }
}

void AdConnector::Rename(String ^ newName)
{
    try {
        _nativeAdConnectorPtr->Rename(StringToStdWstring(newName));
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

void AdConnector::UploadStringAttr(String ^ attrName, String ^ attrVal)
{
    try {
        _nativeAdConnectorPtr->UploadStringAttr(
            StringToStdWstring(attrName),
            StringToStdWstring(attrVal)
        );
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

String ^ AdConnector::DownloadStringAttr(String ^ attrName)
{
    try {
        auto r = gcnew String(_nativeAdConnectorPtr->DownloadStringAttr(StringToStdWstring(attrName)).c_str());
        GC::KeepAlive(this);
        return r;
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

void AdConnector::UploadBinaryAttr(String ^ attrName, cli::array<Byte> ^ bav)
{
    try {
        if (bav->Length > 0) {
            pin_ptr<Byte> pinPtr = &bav[0];
            Byte * p = pinPtr;
            adbook::BinaryAttrVal nbav(p, p + bav->Length);
            _nativeAdConnectorPtr->UploadBinaryAttr(StringToStdWstring(attrName), nbav);
        }
        else {
            _nativeAdConnectorPtr->UploadBinaryAttr(StringToStdWstring(attrName), adbook::BinaryAttrVal());
        }
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

cli::array<Byte> ^ AdConnector::DownloadBinaryAttr(String ^ attrName)
{
    try {
        adbook::BinaryAttrVal nbav = _nativeAdConnectorPtr->DownloadBinaryAttr(StringToStdWstring(attrName));
        GC::KeepAlive(this);
        cli::array<Byte>^ bav = gcnew cli::array<Byte>(static_cast<Int32>(nbav.size()));
        if (nbav.size() == 0) {
            return bav;
        }
        pin_ptr<Byte> pinPtr = &bav[0];
        memcpy_s(pinPtr, bav->Length, &nbav[0], nbav.size());
        return bav;
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew Exception(gcnew String(e.What()));
    }
}

}   // namespace adbookcli
