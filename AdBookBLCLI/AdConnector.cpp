// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
Copyright (C) 2015-2017 Goncharov Andrei.

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

AdConnector::AdConnector(ConnectionParams ^ cs)
    : _underlyingConnector(new adbook::AdConnector(cs->ToUnderlyingType()))
{
    GC::KeepAlive(this);
}

AdConnector::AdConnector(ConnectionParams ^ cs, String ^ dn)
    : _underlyingConnector(new adbook::AdConnector(cs->ToUnderlyingType(), StringToStdWstring(dn)))
{
    GC::KeepAlive(this);
}

AdConnector::~AdConnector() {
	CleanupUnmanaged();
    GC::KeepAlive(this);
}

void AdConnector::CleanupUnmanaged()
{
	try {
		if (_underlyingConnector != nullptr) {
			if (_underlyingConnector->IsConnected()) {
				_underlyingConnector->Disconnect();
			}
            delete _underlyingConnector;
            _underlyingConnector = nullptr;
		}
	}
	catch (const adbook::Error & e) {
		OutputDebugStringW(e.What().c_str());
	}
	catch (const std::exception & e) {
		OutputDebugStringA(e.what());
	}
}

AdConnector::!AdConnector()
{
	FINALISER_CALL_WARNING;
	CleanupUnmanaged();
}

void AdConnector::Connect()
{
    try {
        _underlyingConnector->Connect();
		GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

void AdConnector::Disconnect() 
{
    try {
        _underlyingConnector->Disconnect();
		GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

bool AdConnector::IsConnected() 
{
    try {
		auto isConnected = _underlyingConnector->IsConnected();
		GC::KeepAlive(this);
		return isConnected;
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

String ^ AdConnector::GetLdapPath()
{
    try {
        auto r = gcnew String(_underlyingConnector->GetLdapPath().c_str());        
		GC::KeepAlive(this);
        return r;
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

String ^ AdConnector::GetRDN() 
{
    try {
        auto r = gcnew String(_underlyingConnector->GetRDN().c_str());
		GC::KeepAlive(this);
        return r;
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }    
}

void AdConnector::Rename(String ^ newName)
{
    try {
        _underlyingConnector->Rename(StringToStdWstring(newName));
		GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

void AdConnector::UploadStringAttr(String ^ attrName, String ^ attrVal)
{
    try {
        _underlyingConnector->UploadStringAttr (
            StringToStdWstring(attrName),
            StringToStdWstring(attrVal)
        );
		GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

String ^ AdConnector::DownloadStringAttr(String ^ attrName)
{
    try {
        auto r = gcnew String(_underlyingConnector->DownloadStringAttr(StringToStdWstring(attrName)).c_str());
		GC::KeepAlive(this);
        return r;
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

void AdConnector::UploadBinaryAttr(String ^ attrName, cli::array<Byte> ^ bav)
{
    try {
        if (bav->Length > 0) {
            pin_ptr<Byte> pinPtr = &bav[0];
            Byte * p = pinPtr;
            adbook::BinaryAttrVal nbav(p, p + bav->Length);
            _underlyingConnector->UploadBinaryAttr(StringToStdWstring(attrName), nbav);
			GC::KeepAlive(this);
        }
        else {
            _underlyingConnector->UploadBinaryAttr(StringToStdWstring(attrName), adbook::BinaryAttrVal());
        }
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

cli::array<Byte> ^ AdConnector::DownloadBinaryAttr(String ^ attrName)
{
    try {
        adbook::BinaryAttrVal nbav = _underlyingConnector->DownloadBinaryAttr(StringToStdWstring(attrName));
		GC::KeepAlive(this);
        cli::array<Byte>^ bav = gcnew cli::array<Byte>(boost::numeric_cast<Int32>(nbav.size()));
        pin_ptr<Byte> pinPtr = &bav[0];
        memcpy_s(pinPtr, bav->Length, &nbav[0], nbav.size());
        return bav;
    }
    catch (const adbook::Error & e) {
        throw gcnew Exception(gcnew String(e.What().c_str()));
    }
}

}   // namespace adbookcli
