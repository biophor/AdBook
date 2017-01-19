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
#pragma once

namespace adbookcli
{
using System::String;
using System::Security::SecureString;


public ref class ConnectionParams
{
public:
    ~ConnectionParams()
    {
        delete _password;
    }
    property String^ Dc {
        String^ get() {
            return _dc;
        }
        void set(String^ dc) {
            _dc = dc;
        }
    }
    property String^ Login {
        String^ get() {
            return _login;
        }
        void set(String^ login) {
            _login = login;
        }
    }
    property SecureString^ Password {
        SecureString^ get() {
            return _password;
        }
        void set(SecureString^ password) {
            _password = password;
        }
    }
    property bool UseCurrentUserCredentials {
        bool get() {
            return _useCurrentUserCredentials;
        }
        void set(bool value) {
            _useCurrentUserCredentials = value;
        }
    }
    property bool UseDomainYouAreLoggedIn {
        bool get() {
            return _useDomainYouAreLoggedIn;
        }
        void set(bool value) {
            _useDomainYouAreLoggedIn = value;
        }
    }    
    adbook::ConnectionParams ToUnderlyingType();
private:
    bool _useDomainYouAreLoggedIn = true;
    bool _useCurrentUserCredentials = true;
    SecureString^ _password;
    String ^ _dc, ^ _login;    
    
};

}   // namespace adbookcli


