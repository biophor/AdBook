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

using adbookcli;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace WpfAdBook.DAL
{
    public class AdRepository : IRepository
    {
        private readonly AdAccessFactory _adAccessFactory;
        private AdSearchTask.Arguments _taskArguments;

        private int _numFound;
        private AdSearchTask _searchTask;

        public event EventHandler<ItemFoundEventArgs> ItemFound;

        public event EventHandler SearchStarted;

        public event EventHandler SearchStopped;

        public AdRepository()
        {
            _adAccessFactory = AdAccessFactory.CreateInstance();
        }

        private void _searchTask_PersonFoundEvent(object sender, PersonFoundEventArgs e)
        {
            ++_numFound;
            ItemFound?.Invoke(this, new ItemFoundEventArgs() { Adp = e.Person });
        }

        private void _searchTask_SearchStartedEvent(object sender, EventArgs e)
        {
            _numFound = 0;
            SearchStarted?.Invoke(this, EventArgs.Empty);
        }

        public bool IsSearchActive()
        {
            return _searchTask?.IsCompleted == false;
        }

        public void CancelSearch()
        {
            var st = _searchTask;
            if (st?.IsCompleted == false) {
                st.Cancel();
            }
        }

        public void EnsureSearchIsStopped()
        {
            var searchTask = _searchTask;
            if (null == searchTask) {
                return;
            }
            try {
                if (searchTask.IsCompleted) {
                    return;
                }
                searchTask.Cancel();
                try {
                    searchTask.Wait();
                }
                catch (AggregateException ) {
                    if (searchTask.IsCanceled) {
                        return;
                    }
                    throw;
                }
            }
            finally {
                searchTask.Dispose();
                _searchTask = null;
            }
        }

        public Task StartSearchAsync(ConnectionParams connectionParams, LdapRequestBuilder ldapRequest)
        {
            if (connectionParams == null) {
                throw new ArgumentNullException(nameof(connectionParams));
            }
            if (ldapRequest == null) {
                throw new ArgumentNullException(nameof(ldapRequest));
            }
            using (var adc = _adAccessFactory.CreateConnector()) {
                try {
                    adc.Connect(connectionParams);
                }
                catch (Exception exc) {
                    throw new RepositoryInaccessibleError(Properties.Resources.RepositoryInaccessibleError, exc);
                }
            }
            _taskArguments = new AdSearchTask.Arguments(ldapRequest, connectionParams);
            _searchTask = _adAccessFactory.CreateSearchTask(ldapRequest, connectionParams);
            _searchTask.SearchStartedEvent += _searchTask_SearchStartedEvent;
            _searchTask.PersonFoundEvent += _searchTask_PersonFoundEvent;
            _searchTask.SearchStoppedEvent += _searchTask_SearchStoppedEvent;
            _searchTask.Start();
            return _searchTask;
        }

        private void _searchTask_SearchStoppedEvent(object sender, EventArgs e)
        {
            SearchStopped?.Invoke(this, EventArgs.Empty);
        }

        private static void WriteAttribute(AttrId attrId, AdPerson adp, AdConnector adc)
        {
            if (!adp.IsAttributeWritable(attrId)) {
                throw new DataModificationError(Properties.Resources.DataModificationError);
            }
            var attrInfo = AdAttributes.Get(attrId);
            if (attrId == AttrId.Dn) {
                // Dn cannot be changed directly
                throw new DataModificationError(Properties.Resources.DataModificationError);
            }
            if (attrId == AttrId.CommonName) {
                // special case for CN
                var origCn = adc.DownloadStringAttr(attrInfo.LdapName);
                string cn = adp.get_StringAttr(attrInfo.LdapName);
                if (origCn != cn) {
                    adc.Rename(cn);
                }
                return;
            }
            if (attrInfo.IsString) {
                adc.UploadStringAttr(attrInfo.LdapName, adp.get_StringAttr(attrInfo.LdapName));
            }
            else {
                adc.UploadBinaryAttr(attrInfo.LdapName, adp.get_BinaryAttr(attrInfo.LdapName));
            }
        }

        public IEnumerable<AttrId> WriteAdPerson(ConnectionParams connectionParams, AdPerson adp)
        {
            if (connectionParams == null) {
                throw new ArgumentNullException(nameof(connectionParams));
            }
            if (adp == null) {
                throw new ArgumentNullException(nameof(adp));
            }
            if (string.IsNullOrWhiteSpace(adp.Dn)) {
                throw new ArgumentException(nameof(adp));
            }
            List<AttrId> skippedAttributes = new List<AttrId>();
            using (var adc = _adAccessFactory.CreateConnector()) {
                try {
                    adc.Connect(connectionParams, adp.Dn);
                }
                catch (Exception exc) {
                    throw new RepositoryInaccessibleError(Properties.Resources.RepositoryInaccessibleError, exc);
                }
                try {
                    foreach (var attrId in AdAttributes.AttrIds) {
                        if (!adp.IsAttributeWritable(attrId)) {
                            skippedAttributes.Add(attrId);
                            continue;
                        }
                        if (attrId == AttrId.Dn) {
                            continue;
                        }
                        WriteAttribute(attrId, adp, adc);
                    }
                }
                catch (Exception exc) {
                    throw new DataModificationError(Properties.Resources.DataModificationError, exc);
                }
            }
            return skippedAttributes;
        }

        public AdPerson ReadAdPerson(ConnectionParams connectionParams, string dn)
        {
            if (connectionParams == null) {
                throw new ArgumentNullException(nameof(connectionParams));
            }
            if (dn == null) {
                throw new ArgumentNullException(nameof(dn));
            }
            if (string.IsNullOrWhiteSpace(dn)) {
                throw new ArgumentException(nameof(dn));
            }

            using (var adc = _adAccessFactory.CreateConnector()) {
                try {
                    adc.Connect(connectionParams, dn);
                }
                catch (Exception exc) {
                    throw new RepositoryInaccessibleError(Properties.Resources.RepositoryInaccessibleError, exc);
                }
                try {
                    AdPerson adp = new AdPerson();
                    foreach (var attrId in AdAttributes.AttrIds) {
                        if (attrId == AttrId.Dn) {
                            continue;
                        }
                        var attrInfo = AdAttributes.Get(attrId);
                        if (attrInfo.IsString) {
                            adp.set_StringAttr(attrInfo.LdapName, adc.DownloadStringAttr(attrInfo.LdapName));
                        }
                        else {
                            adp.set_BinaryAttr(attrInfo.LdapName, adc.DownloadBinaryAttr(attrInfo.LdapName));
                        }
                    }
                    return adp;
                }
                catch (Exception exc) {
                    throw new DataModificationError(Properties.Resources.DataModificationError, exc);
                }
            }
        }

        public bool IsAvailable(ConnectionParams connectionParams)
        {
            if (connectionParams == null) {
                throw new ArgumentNullException(nameof(connectionParams));
            }
            using (var adc = _adAccessFactory.CreateConnector()) {
                try {
                    adc.Connect(connectionParams);
                }
                catch (Exception) {
                    return false;
                }
            }
            return true;
        }

        #region IDisposable Support

        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue) {
                if (disposing) {
                    EnsureSearchIsStopped();
                    _adAccessFactory?.Dispose();
                }
                disposedValue = true;
            }
        }
        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion IDisposable Support
    }
}