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


namespace adbookcli
{
using System::IntPtr;
using System::Security::SecurityCriticalAttribute;
using System::Runtime::CompilerServices::RuntimeHelpers;
using System::Runtime::InteropServices::SafeHandle;




template <class T>
public ref class NativeAbstractObjectPtr abstract : SafeHandle
{
public:
    NativeAbstractObjectPtr(T * p) : SafeHandle(IntPtr(p), true) { }

    virtual property bool IsInvalid {
        [SecurityCritical]
        bool get() override {
            return handle == IntPtr::Zero;
        }
    }

    static T * operator -> (NativeAbstractObjectPtr % nlrp) {
        return reinterpret_cast<T *>(nlrp.handle.ToPointer());
    }

#ifdef _DEBUG
    [SecurityCriticalAttribute]
    ~NativeAbstractObjectPtr() {
        if (!IsInvalid) {
            ReleaseNativeResources();
        }
    }
    !NativeAbstractObjectPtr() {
        FINALISER_CALL_WARNING;
        if (!IsInvalid) {
            ReleaseNativeResources();
        }
    }
#endif

protected:
    [SecurityCriticalAttribute]
    virtual bool ReleaseHandle() override {
        if (!IsInvalid) {
            ReleaseNativeResources();
        }
        return true;
    }

    virtual void ReleaseNativeResources() abstract;
};


template <class T>
public ref class NativeObjectPtr abstract : public NativeAbstractObjectPtr<T>
{
public:
    NativeObjectPtr(T * p) : NativeAbstractObjectPtr(p) { }

    static T operator * (NativeObjectPtr % nlrp) {
        return *reinterpret_cast<T *>(nlrp.handle.ToPointer());
    }
};


}   // namespace adbookcli



