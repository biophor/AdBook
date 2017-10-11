#pragma once

#include "NativeObjectPtr.h"

using namespace System;
using namespace System::Collections::Generic;

namespace adbookcli
{

public ref class NativeAdPersonDescKeeperPtr
    : public NativeAbstractObjectPtr<adbook::AbstractAdPersonDescKeeper>
{
public:
    NativeAdPersonDescKeeperPtr(adbook::AbstractAdPersonDescKeeper * p)
        : NativeAbstractObjectPtr<adbook::AbstractAdPersonDescKeeper>(p) {}
protected:
    virtual void ReleaseNativeResources() override;
};


public ref class AdPersonDescKeeper sealed: IEnumerable<AdPerson^>, Collections::IEnumerable
{
public:
    void Add(AdPerson^ adp);
    void SetNameByConnectionParams(ConnectionParams ^ connectionParams);
    void Save();
    void Load();
    AdPersonDescKeeper(adbook::AbstractAdPersonDescKeeper * p);

    virtual IEnumerator<adbookcli::AdPerson ^> ^ GetEnumerator();
private:
    List<AdPerson^>^ _managedStore;
    NativeAdPersonDescKeeperPtr _nativeStore;
private:

    virtual Collections::IEnumerator ^ IEnumerable_GetEnumerator() sealed =
        Collections::IEnumerable::GetEnumerator;
};


}   // namespace adbookcli