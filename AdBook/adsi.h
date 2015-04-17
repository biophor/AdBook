#pragma once
typedef CComQIPtr<IADs, &IID_IADs>                          IADsPtr;
typedef CComQIPtr<IADsUser, &IID_IADsUser>                  IADsUserPtr;
typedef CComQIPtr<IADsPathname, &IID_IADsPathname>          IADsPathNamePtr;
typedef CComQIPtr<IADsContainer, &IID_IADsContainer>        IADsContainerPtr;
typedef CComQIPtr<IDirectoryObject, &IID_IDirectoryObject>  IDirectoryObjectPtr;
typedef CComQIPtr<IDirectorySearch, &IID_IDirectorySearch>  IDirectorySearchPtr;

typedef std::vector<wchar_t> WcharBuf;  // Some ADSI functions required non-const IN arguments.    