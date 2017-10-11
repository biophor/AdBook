// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Goncharov Andrei.

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
#include "error.h"
#include "shared.h"
#include "dllmain.h"
#include "resource.h"
#include "AdPersonDescList.h"

namespace adbook
{
    
namespace
{
const wchar_t personsTableName[] = L"persons";
const wchar_t indexColName[] = L"position";
const wchar_t writableAttrsColName[] = L"writableAttributes";
const wchar_t mutexName[] = L"{F22EEC20-56B6-443A-8C65-5D1DC38F1D78}";

class SimpleInterprocessSync
{
public:
    SimpleInterprocessSync(const wchar_t * mtxName)
    {
        _mutexHandle = CreateMutexW(nullptr, FALSE, mtxName);
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

namespace {
    std::wstring GetSqliteDbFileName()
    {
        std::wstring appName = LoadString(IDS_APP_TITLE);
        wchar_t fullPath[MAX_PATH];
        const HRESULT hr = SHGetFolderPathAndSubDirW(nullptr, CSIDL_FLAG_CREATE | CSIDL_APPDATA,
            nullptr, SHGFP_TYPE_CURRENT, appName.c_str(), fullPath);
        if (FAILED(hr))
        {
            HR_ERROR(hr);
        }
        if (!PathAppendW(fullPath, L"SearchResult.sqlite3"))
        {
            HR_ERROR(E_UNEXPECTED);
        }
        return fullPath;
    }
}
void AdPersonDescList::Load()
{
    try
    {
        // Another instance of AdBook may do the same operations at the same time.
        // So we need to serialize.
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

class SqliteSimpleWrapper
{
public:
    SqliteSimpleWrapper(const SqliteSimpleWrapper &) = delete;
    SqliteSimpleWrapper & operator = (const SqliteSimpleWrapper &) = delete;
    SqliteSimpleWrapper(SqliteSimpleWrapper &&) = delete;
    SqliteSimpleWrapper & operator = (SqliteSimpleWrapper &&) = delete;

    SqliteSimpleWrapper(const std::wstring & dbFileName, DWORD flags)
    {        
        const int rc = sqlite3_open_v2(CW2A(dbFileName.c_str()), &db_, flags, nullptr);
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_open_v2() failed.", __FUNCTIONW__);
        }
    }
    static void InitSqlite()
    {
        int rc = sqlite3_initialize();
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_initialize() failed.", __FUNCTIONW__);
        }
    }
    static void DeinitSqlite()
    {
        const int rc = sqlite3_shutdown();
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_shutdown() failed.", __FUNCTIONW__);
        }
    }
    void CloseDb()
    {
        if (db_ != nullptr)
        {
            int rc = sqlite3_close(db_);
            if (rc != SQLITE_OK)
            {
                throw Sqlite3Error(rc, L"sqlite3_close() failed.", __FUNCTIONW__);
            }
            db_ = nullptr;
        }
    }
    void PrepareStatement(const std::wstring & sql)
    {
        const int rc = sqlite3_prepare16_v2(db_, sql.c_str(), -1, &stmt_, nullptr);
        if (rc != SQLITE_OK)
        {
            auto m = std::wstring(L"sqlite3_prepare16_v2 failed. sql: ") + sql;
            throw Sqlite3Error(rc, m.c_str(), __FUNCTIONW__);
        }
    }
    int Step()
    {
        const int rc = sqlite3_step(stmt_);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW)
        {
            throw Sqlite3Error(rc, L"sqlite3_step() failed", __FUNCTIONW__);
        }
        return rc;
    }    
    void FinalizeStatement()
    {
        const int rc = sqlite3_finalize(stmt_);
        stmt_ = nullptr;
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_finalize() failed.", __FUNCTIONW__);
        }
    }
    void ResetStatement()
    {
        const int rc = sqlite3_reset(stmt_);
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_reset() failed.", __FUNCTIONW__);
        }
    }
    void ClearBindings()
    {
        const int rc = sqlite3_clear_bindings(stmt_);
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_clear_bindings() failed.", __FUNCTIONW__);
        }
    }
    int GetParameterIndex(const std::wstring & paramName)
    {
        const int paramIndex = sqlite3_bind_parameter_index(stmt_, CW2A(paramName.c_str()));
        if (0 == paramIndex)
        {
            throw HrError(E_INVALIDARG, L"sqlite3_bind_parameter_index() failed.", __FUNCTIONW__);
        }
        return paramIndex;
    }
    void BindText(const std::wstring & paramName, const wchar_t * value)
    {
        const int paramIndex = GetParameterIndex(paramName);
        const int rc = sqlite3_bind_text16(stmt_, paramIndex, value, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_bind_text16()", __FUNCTIONW__);
        }
    }
    void BindBlob(const std::wstring & paramName, const BYTE * value, int len)
    {
        const int paramIndex = GetParameterIndex(paramName);
        const int rc = sqlite3_bind_blob(stmt_, paramIndex, value, len, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
            throw Sqlite3Error(rc, L"sqlite3_bind_blob()", __FUNCTIONW__);
        }
    }
    int GetColumnCount()
    {
        const int rc = sqlite3_column_count(stmt_);
        if (0 == rc)
        {
            throw Sqlite3Error(rc, L"sqlite3_column_count()", __FUNCTIONW__);
        }
        return rc;
    }
    std::wstring GetColName(int index)
    {
        const wchar_t * p = reinterpret_cast<const wchar_t*>(sqlite3_column_name16(stmt_, index));
        if (nullptr == p)
        {
            throw Sqlite3Error(SQLITE_FAIL, L"sqlite3_column_name16()", __FUNCTIONW__);
        }
        return std::wstring(p);
    }
    int GetColumnType(int index)
    {
        int rc = sqlite3_column_type(stmt_, index);
        switch (rc)
        {
        case SQLITE_INTEGER: case SQLITE_FLOAT: case SQLITE_TEXT: case SQLITE_BLOB: case SQLITE_NULL:
            return rc;
        }
        throw Sqlite3Error(rc, L"sqlite3_column_type()", __FUNCTIONW__);
    }
    std::wstring GetColumnText(int index)
    {
        const wchar_t * text = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt_, index));
        const int sizeInBytes = sqlite3_column_bytes16(stmt_, index);
        if (0 == sizeInBytes || nullptr == text)
        {
            return std::wstring();
        }
        return std::wstring(text, text + boost::numeric_cast<size_t>(sizeInBytes) / sizeof(wchar_t));
    }
    std::vector<BYTE> GetColumnBlob(int index)
    {
        const BYTE * data = reinterpret_cast<const BYTE*>(sqlite3_column_blob(stmt_, index));
        int sizeInBytes = sqlite3_column_bytes(stmt_, index);
        if (nullptr == data || 0 == sizeInBytes)
        {
            return std::vector<BYTE>();
        }
        return std::vector<BYTE>(data, data + boost::numeric_cast<size_t>(sizeInBytes));
    }
    ~SqliteSimpleWrapper()
    {
        if (stmt_ != nullptr)
        {
            sqlite3_finalize(stmt_);            
        }
        if (db_ != nullptr)
        {
            sqlite3_close(db_);
        }
    }
private:
    sqlite3 * db_ = nullptr;
    sqlite3_stmt * stmt_ = nullptr;
};

void AdPersonDescList::InternalSave()
{
    const std::wstring tmpFileName = GetSqliteDbFileName() + L".tmp";
    SqliteSimpleWrapper::InitSqlite();
    if (PathFileExistsW(tmpFileName.c_str()))
    {
        if (!DeleteFileW(tmpFileName.c_str()))
        {
            throw HrError(HRESULT_FROM_WIN32(GetLastError()), L"DeleteFile() failed.", __FUNCTIONW__);
        }
    }
    // begin transaction
    SqliteSimpleWrapper sqlite(tmpFileName, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    std::wstring sql = L"BEGIN TRANSACTION";
    sqlite.PrepareStatement(sql);
    sqlite.Step();
    sqlite.FinalizeStatement();

    // create a table for the contacts
    const auto & attrMgr = Attributes::GetInstance();
    const auto attrIds = attrMgr.GetAttrIds();
    sql = L"CREATE TABLE contacts(rowid INTEGER PRIMARY KEY NOT NULL";
    for (auto id : attrIds)
    {
        const std::wstring colName = attrMgr.GetLdapAttrName(id);
        if (attrMgr.IsString(id))
        {
            sql += L", " + colName + L" TEXT";
        }
        else
        {
            sql += L", " + colName + L" BLOB";
        }
    }
    sql += L", ";
    sql += writableAttrsColName;
    sql += L" BLOB";
    sql += L");";
    sqlite.PrepareStatement(sql);
    sqlite.Step();
    sqlite.FinalizeStatement();

    // populate the table
    sql = L"INSERT INTO contacts (";
    for (auto id : attrIds)
    {
        const std::wstring colName = attrMgr.GetLdapAttrName(id);
        sql += colName + L",";
    }
    sql += writableAttrsColName;
    sql += L") VALUES (";
    for (auto id : attrIds)
    {
        const std::wstring colName = attrMgr.GetLdapAttrName(id);
        sql += L":" + colName + L",";
    }
    sql += L":";
    sql += writableAttrsColName;
    sql += L")";
    sqlite.PrepareStatement(sql);
    for (auto iter = cbegin(), end = cend(); iter != end; ++iter)
    {
        const auto & personDesc = *iter;
        std::set<adbook::Attributes::AttrId> wa = personDesc.GetWritableAttributes(); 
        std::vector<BYTE> waVec;
        std::transform(wa.cbegin(), wa.cend(), std::back_inserter(waVec),
            [](adbook::Attributes::AttrId attrId) { return static_cast<BYTE>(attrId); }
        );
        if (!waVec.empty())
        {
            std::wstring paramName = std::wstring(L":") + writableAttrsColName;
            sqlite.BindBlob(paramName, &waVec[0], boost::numeric_cast<int>(waVec.size()));            
        }
        for (const auto id : attrIds)
        {
            const auto attrName = attrMgr.GetLdapAttrName(id);
            if (!personDesc.IsAttributeSet(attrName))
            {
                continue;
            }
            std::wstring paramName = std::wstring(L":") + attrName;                
            if (attrMgr.IsString(id))
            {
                const wchar_t * attrValue = personDesc.GetStringAttrPtr(attrName);
                if (attrValue)
                {
                    sqlite.BindText(paramName, attrValue);
                }
            }
            else
            {
                size_t attrSize = 0;
                const BYTE * attrValue = personDesc.GetBinaryAttrPtr(attrName, attrSize);
                if (attrValue)
                {
                    sqlite.BindBlob(paramName, attrValue, boost::numeric_cast<int>(attrSize));                    
                }
            }            
        }
        sqlite.Step();
        sqlite.ResetStatement();
        sqlite.ClearBindings();
    }
    sqlite.FinalizeStatement();
    sql = L"COMMIT";
    sqlite.PrepareStatement(sql);
    sqlite.Step();
    sqlite.FinalizeStatement();
    sqlite.CloseDb();
    if (!MoveFileEx(tmpFileName.c_str(), GetSqliteDbFileName().c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
    {
        throw HrError(HRESULT_FROM_WIN32(GetLastError()), L"MoveFile() failed.", __FUNCTIONW__);
    }
#pragma warning(push)
#pragma warning(disable: 4459)
    BOOST_SCOPE_EXIT(void)
    {
        SqliteSimpleWrapper::DeinitSqlite();
    } BOOST_SCOPE_EXIT_END
#pragma warning(pop)    
}

void AdPersonDescList::InternalLoad()
{
    const std::wstring fileName = GetSqliteDbFileName();    
    if (!PathFileExistsW(fileName.c_str()))
    {
        return;
    }
    const auto & attrMgr = Attributes::GetInstance();
    SqliteSimpleWrapper::InitSqlite();
    SqliteSimpleWrapper sqlite(fileName, SQLITE_OPEN_READONLY);
    sqlite.PrepareStatement(L"SELECT * from contacts ORDER BY rowid ASC;");
    const int columnCount = sqlite.GetColumnCount();
    
    while (sqlite.Step() == SQLITE_ROW)
    {
        AdPersonDesc personDesc;
        for (int i = 0; i < columnCount; ++i)
        {
            const int colType = sqlite.GetColumnType(i);
            std::wstring colName = sqlite.GetColName(i);
            if (!attrMgr.IsAttrSupported(colName.c_str()))
            {
                if (colName == writableAttrsColName)
                {
                    if (SQLITE_BLOB == colType)
                    {
                        std::vector<BYTE> attrValue = sqlite.GetColumnBlob(i);
                        AdPersonDesc::AttrIds wa;
                        std::transform(attrValue.cbegin(), attrValue.cend(), std::inserter(wa, wa.end()),
                            [](const BYTE b) { return boost::numeric_cast<Attributes::AttrId>(b); });
                        personDesc.SetWritableAttributes(wa);                        
                    }
                    continue;
                }
                if (colName == L"rowid")
                {
                    continue;
                }
                HR_ERROR(E_UNEXPECTED);
            }
            if (SQLITE_TEXT == colType)
            {
                std::wstring attrValue = sqlite.GetColumnText(i);
                personDesc.SetStringAttr(colName, attrValue);
            }
            else if (SQLITE_BLOB == colType)
            {
                std::vector<BYTE> attrValue = sqlite.GetColumnBlob(i);
                personDesc.SetBinaryAttr(colName, attrValue);
            }
            else if (SQLITE_NULL == colType)
            {
                continue;
            }
            else
            {
                HR_ERROR(E_UNEXPECTED);
            }
        }
        push_back(std::move(personDesc));
    }
    sqlite.FinalizeStatement();

#pragma warning(push)
#pragma warning(disable: 4459)
    BOOST_SCOPE_EXIT(void)
    {
        SqliteSimpleWrapper::DeinitSqlite();
    } BOOST_SCOPE_EXIT_END
#pragma warning(pop)    
}

}   // namespace adbook

