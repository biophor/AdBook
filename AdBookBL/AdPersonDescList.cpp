#include "stdafx.h"
#include "error.h"
#include "shared.h"
#include "dllmain.h"
#include "resource.h"
#include "AdPersonDescList.h"

namespace adbook
{

std::wstring ExtractDirFromFilePath(const std::wstring & filePath)
{
    if (filePath.empty())
    {
        throw HrError(E_INVALIDARG, __FUNCTIONW__);
    }
    std::vector<wchar_t> buf(filePath.cbegin(), filePath.cend());
    buf.push_back(L'\0');
    LPWSTR namePtr = PathFindFileName(&buf[0]);
    if (namePtr == &buf[0] || !namePtr)
    {
        return std::wstring();
    }
    *namePtr = L'\0';
    return std::wstring(&buf[0]);
}

namespace
{
const wchar_t personsTableName[] = L"persons";
const wchar_t indexColName[] = L"position";
const wchar_t writableAttrsColName[] = L"writableAttributes";
const wchar_t mutexName[] = L"{F22EEC20-56B6-443A-8C65-5D1DC38F1D78}";

class SimpleInterprocessSync
{
public:
    SimpleInterprocessSync(const wchar_t * mutexName)
    {
        _mutexHandle = CreateMutexW(nullptr, FALSE, mutexName);
        if (nullptr == _mutexHandle)
        {
            throw HrError(HRESULT_FROM_WIN32(GetLastError()), __FUNCTIONW__);
        }
        BOOST_VERIFY(WAIT_OBJECT_0 == WaitForSingleObject(_mutexHandle, INFINITE));
    }
    ~SimpleInterprocessSync()
    {
        BOOST_VERIFY(ReleaseMutex(_mutexHandle));
        BOOST_VERIFY(CloseHandle(_mutexHandle));
    }
private:
    HANDLE _mutexHandle = nullptr;
};

}   // anon namespace

std::wstring GetFileName()
{
    std::wstring appName = LoadString(IDS_APP_TITLE);    
    wchar_t fullPath[MAX_PATH];
    const HRESULT hr = SHGetFolderPathAndSubDir(nullptr, CSIDL_FLAG_CREATE | CSIDL_APPDATA,
        nullptr, SHGFP_TYPE_CURRENT, appName.c_str(), fullPath);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    if (!PathAppendW(fullPath, L"SearchResult.edb"))
    {
        HR_ERROR(E_UNEXPECTED);
    }
    return fullPath;
}
namespace
{

}
void AdPersonDescList::Load()
{    
    try
    {        
        // Another instance of AdBook may do the same operations at the same time.
        // So we neen to serialize.
        SimpleInterprocessSync simpleSync(mutexName);
        InternalLoad();
    }
    catch (std::exception & )
    {        
        HR_ERROR(E_UNEXPECTED);
    }
}

void AdPersonDescList::Save()
{
    try
    {
        // Another instance of AdBook may do the same operations at the same time.
        // So we neen to serialize.
        SimpleInterprocessSync simpleSync(mutexName);
        InternalSave();
    }
    catch (std::exception & )
    {        
        HR_ERROR(E_UNEXPECTED);
    }
}

void AdPersonDescList::InternalSave()
{
    auto edbFilePath = GetFileName();
    auto edbFileDir = ExtractDirFromFilePath(edbFilePath);
    jet::FactoryPtr factory(jet::CreateFactory());    
    jet::InstancePtr inst(factory->CreateInstance2(nullptr));
    jet::ThrowIfNull(inst, factory);    
    jet::ThrowIfError(inst->SetLogsDir(edbFileDir.c_str()), inst);
    jet::ThrowIfError(inst->SetCheckpointDir(edbFileDir.c_str()), inst);
    jet::ThrowIfError(inst->EnableCircularLog(true), inst);
    jet::ThrowIfError(inst->Init(), inst);
    jet::SessionPtr ses(inst->CreateSession());
    jet::ThrowIfNull(ses, inst);
    jet::DatabasePtr db(ses->CreateDatabase(edbFilePath.c_str(), jet::OverwritePolicy::Overwrite, 
                                            jet::RecoveryPolicy::Recover));
    jet::ThrowIfNull(db, ses);
    jet::TablePtr tbl(db->CreateTable(personsTableName));
    jet::ThrowIfNull(tbl, db);    

    const auto & attrMgr = Attributes::GetInstance();
    const auto attrIds = attrMgr.GetAttrIds();

    std::map<Attributes::AttrId, jet::ColumnPtr> cols;

    jet::ColumnConstructorPtr colConstructionPtr(tbl->CreateColumnConstructor());
    colConstructionPtr->SelectAutoIncInt32();
    jet::ColumnPtr indexCol(colConstructionPtr->CreateColumn(indexColName));
    jet::ThrowIfNull(indexCol, colConstructionPtr);
#ifdef max
#undef max
#endif
    colConstructionPtr->SelectBinaryVariableSize(std::numeric_limits<BYTE>::max(),
                                                 jet::ValQuantityPolicy::SingleValued,
                                                 jet::NullPolicy::MayBeNull);
    jet::ColumnPtr waCol(colConstructionPtr->CreateColumn(writableAttrsColName));
    jet::ThrowIfNull(waCol, tbl);

    for (const auto id : attrIds)
    {
        const std::wstring colName = attrMgr.GetLdapAttrName(id);
        if (attrMgr.IsString(id))
        {
            colConstructionPtr->SelectUtf16TextVariableSize(boost::numeric_cast<ULONG>(attrMgr.GetAttrMaxLength(id)), 
                                                            jet::ValQuantityPolicy::SingleValued,
                                                            jet::NullPolicy::MayBeNull);
        }
        else
        {
            colConstructionPtr->SelectBinaryVariableSize(boost::numeric_cast<ULONG>(attrMgr.GetAttrMaxLength(id)), 
                                                         jet::ValQuantityPolicy::SingleValued,
                                                         jet::NullPolicy::MayBeNull);
        }
        jet::ThrowIfNull(cols[id] = std::move(jet::ColumnPtr(colConstructionPtr->CreateColumn(colName.c_str()))), tbl);
    }
    jet::IndexConstructorPtr indexCtrPtr(tbl->CreateIndexConstructor());

    jet::ThrowIfError(indexCtrPtr->SetIndexName(L"PrimaryKey"), indexCtrPtr);
    jet::ThrowIfError(indexCtrPtr->Primary(), indexCtrPtr);
    jet::ThrowIfError(indexCtrPtr->AddColumn(indexColName, jet::IIndexConstructor::SortDirection::Ascending), indexCtrPtr);
    jet::ThrowIfError(indexCtrPtr->CreateIndex(), indexCtrPtr);

    for (auto iter = cbegin(), end = cend(); iter != end; ++iter)
    {
        jet::TransactionHelper th(ses);
        jet::UpdateHelper uh(tbl, jet::UpdateHelper::UpdateType::Insert);
        const auto & personDesc = *iter;
        auto wa = personDesc.GetWritableAttributes();
        std::vector<BYTE> waVec(wa.cbegin(), wa.cend());
        if (!waVec.empty())
        {
            jet::ThrowIfError(waCol->SetBinary(&waVec[0], boost::numeric_cast<uint32_t>(waVec.size())), waCol);
        }
        for (const auto id : attrIds)
        {
            const auto attrName = attrMgr.GetLdapAttrName(id);
            if (personDesc.IsAttributeSet(attrName))
            {
                if (attrMgr.IsString(id))
                {
                    jet::ThrowIfError(cols.at(id)->SetUtf16String(
                        personDesc.GetStringAttr(attrName).c_str()), cols[id]
                    );
                }
                else
                {
                    auto ba = personDesc.GetBinaryAttr(attrName);
                    if (!ba.empty())
                    {
                        jet::ThrowIfError(cols.at(id)->SetBinary(
                            &ba[0], boost::numeric_cast<uint32_t>(ba.size())), cols[id]
                        );
                    }
                }
            }
        }
        uh.Update();
        th.Commit(jet::ISession::CommitOption::DontWaitFlush);
    }
    jet::ThrowIfError(tbl->ComputeStats(), tbl);
    jet::ThrowIfError(tbl->Close(), tbl);
    jet::ThrowIfError(db->Close(), db);
    jet::ThrowIfError(ses->Close(), ses);
    jet::ThrowIfError(inst->Close(), inst);
}

void AdPersonDescList::InternalLoad()
{    
    const auto fileName = GetFileName();
    if (!PathFileExistsW(fileName.c_str()))
    {
        return;
    }
    auto edbFilePath = GetFileName();
    auto edbFileDir = ExtractDirFromFilePath(edbFilePath);
    jet::FactoryPtr factory(jet::CreateFactory());    
    jet::InstancePtr inst(factory->CreateInstance2(nullptr));
    jet::ThrowIfNull(inst, factory);
    jet::ThrowIfError(inst->SetLogsDir(edbFileDir.c_str()), inst);
    jet::ThrowIfError(inst->SetCheckpointDir(edbFileDir.c_str()), inst);
    jet::ThrowIfError(inst->EnableRecovery(false), inst);
    jet::ThrowIfError(inst->Init(), inst);

    jet::SessionPtr ses(inst->CreateSession());
    jet::ThrowIfNull(ses, inst);

    using jet::ISession;
    jet::ThrowIfError(ses->AttachDatabase(fileName.c_str(), jet::DbAccessLevel::ReadOnly), ses);
    jet::DatabasePtr db(ses->OpenDatabase(fileName.c_str(), jet::DbAccessLevel::ReadOnly, jet::DbAccessType::Exclusive));
    jet::ThrowIfNull(db, ses);

    std::vector<std::wstring> tableNames;
    uint32_t numTables = 0;
    jet::ThrowIfError(db->GetNumTables(numTables), db);
    for (uint32_t i = 0; i < numTables; ++i)
    {
        wchar_t tableName[jet::ITable::TableNameMaxLenInWcharsWithZero];
        uint32_t tableNameSize = _countof(tableName);
        jet::ThrowIfError(db->GetTableName(i, tableName, tableNameSize), db);
        tableNames.emplace_back(tableName);
    }
    if (tableNames.end() == std::find(tableNames.cbegin(), tableNames.cend(), personsTableName))
    {
        throw std::exception("personsTable was not found");
    }
    jet::TablePtr table(db->OpenTable(personsTableName, jet::TableAccessLevel::ReadOnly));
    jet::ThrowIfNull(table, db);

    std::map<std::wstring, jet::ColumnPtr> cols;
    std::vector<std::wstring> colNames;
    uint32_t numCols = 0;
    jet::ThrowIfError(table->GetNumCols(numCols), table);
    for (uint32_t i = 0; i < numCols; ++i)
    {
        wchar_t colName[jet::IColumn::ColNameMaxLenInWcharsWithZero];
        uint32_t colNameSize = _countof(colName);
        jet::ThrowIfError(table->GetColName(i, colName, colNameSize), table);
        colNames.emplace_back(colName);
    }
    for (const auto & colName : colNames)
    {
        jet::ThrowIfNull(cols[colName] = std::move(jet::ColumnPtr(table->GetColumn(colName.c_str()))), table);
    }
    uint32_t numRecords = 0;
    jet::ThrowIfError(table->GetNumRecords(numRecords), table);
    if (0 == numRecords)
    {
        return;
    }
    jet::ThrowIfError(table->MoveCursorToBegin(), table);

    const auto & attrMgr = Attributes::GetInstance();
    std::vector<BYTE> binary(attrMgr.GetBinaryAttrMaxLength());
    std::vector<wchar_t> utf16Text(attrMgr.GetTextAttrMaxLength() + 1);
    resize(numRecords);
    for (auto i = 0u; i < numRecords; ++i)
    {
        AdPersonDesc personDesc;
        jet::ColumnPtr waCol(table->GetColumn(writableAttrsColName));
        jet::ThrowIfNull(waCol, table);
        uint32_t buffSize = static_cast<uint32_t>(binary.size());
        auto hr = waCol->GetBinary(&binary[0], buffSize);
        if (SUCCEEDED(hr))
        {
            std::vector<BYTE> waVec(binary.cbegin(), binary.cbegin() + buffSize);
            AdPersonDesc::AttrIds wa;
            std::transform(waVec.cbegin(), waVec.cend(), std::inserter(wa, wa.end()),
                           [](const BYTE b) { return boost::numeric_cast<Attributes::AttrId>(b); });
            personDesc.SetWritableAttributes(wa);
        }
        if (E_VALUE_NOT_SET != hr)
        {
            jet::ThrowIfError(hr, waCol);
        }
        
        for (auto & col : cols)
        {
            if (!attrMgr.IsAttrSupported(col.first.c_str()))
            {
                continue;
            }
            if (attrMgr.IsString(attrMgr.GetAttrId(col.first.c_str())))
            {
                CComBSTR bstr;
                if (SUCCEEDED(col.second->GetUtf16String2(bstr.m_str)))
                {
                    personDesc.SetStringAttr(col.first.c_str(), bstr);
                }
            }
            else
            {
                buffSize = static_cast<uint32_t>(binary.size());
                if (SUCCEEDED(col.second->GetBinary(&binary[0], buffSize)) && buffSize)
                {
                    std::vector<BYTE> bav(binary.cbegin(), binary.cbegin() + buffSize);
                    personDesc.SetBinaryAttr(col.first.c_str(), bav);
                }
            }
        }
        at(i) = personDesc;
        if (i + 1 < numRecords)
        {
            jet::ThrowIfError(table->MoveCursor(1), table);
        }
    }
    jet::ThrowIfError(table->Close(), table);
    jet::ThrowIfError(db->Close(), db);
    jet::ThrowIfError(ses->Close(), ses);
    jet::ThrowIfError(inst->Close(), inst);
}

}   // namespace adbook