
#pragma once
#include "AdAttributes.h"
#include "AdConnector.h"
#include "AdSearchTask.h"
#include "AdPersonDescKeeper.h"

using namespace System;

namespace adbookcli
{

    using System::Runtime::CompilerServices::RuntimeHelpers;

    public ref class NativeAdAccessFactoryPtr : NativeAbstractObjectPtr<adbook::AbstractAdAccessFactory>
    {
    public:
        NativeAdAccessFactoryPtr(adbook::AbstractAdAccessFactory * adf)
            : NativeAbstractObjectPtr<adbook::AbstractAdAccessFactory>(adf) {}

    protected:
        virtual void ReleaseNativeResources() override;
    };


public ref class AdAccessFactory
{
public:
    AdConnector^ CreateConnector();
    AdSearchTask^ CreateSearchTask(LdapRequestBuilder^ requestBuilder, ConnectionParams^ connectionParams);
    AdPersonDescKeeper^ GetPersonDescKeeper();

    static AdAccessFactory^ GetInstance() {
        return factory->Value;
    }

    static AdAccessFactory^ CreateInstance() {
        return CreateFactory();
    }

    static void EnsureDisposed() {
        if (factory->IsValueCreated) {
            delete factory->Value;
        }
    }
private:
    AdAccessFactory();

    static AdAccessFactory^ CreateFactory() {
        return gcnew AdAccessFactory();
    }

    static AdAccessFactory() {
        factory = gcnew Lazy<AdAccessFactory^>(gcnew Func<AdAccessFactory^>(CreateFactory));
    }

    static System::Lazy<AdAccessFactory^>^ factory;
private:
    NativeAdAccessFactoryPtr _nativeFactoryPtr;
};


}   // namespace adbookcli
