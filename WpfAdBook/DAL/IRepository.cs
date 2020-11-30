// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2017 Andrei Goncharov.

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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using adbookcli;
using WpfAdBook.ViewModels;
using System.Security;

namespace WpfAdBook.DAL
{
    public class ItemFoundEventArgs : EventArgs
    {
        public AdPerson Adp { get; set; }
    }

    [Serializable]
    public class DataModificationError: Exception
    {
        public DataModificationError() : base()
        {
        }

        public DataModificationError(string message) : base(message)
        {
        }

        public DataModificationError(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected DataModificationError(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context) : base(info, context)
        {
        }
    }

    [Serializable]
    public class RepositoryInaccessibleError: Exception
    {
        public RepositoryInaccessibleError() : base()
        {
        }

        public RepositoryInaccessibleError(string message) : base(message)
        {
        }

        public RepositoryInaccessibleError(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected RepositoryInaccessibleError(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context) : base(info, context)
        {
        }
    }

    public interface IRepository: IDisposable
    {
        Task StartSearchAsync(ConnectionParams connectionParams, LdapRequestBuilder ldapRequest);

        void CancelSearch();    // cancel the search and continue
        void EnsureSearchIsStopped();   // cancel the search and wait until it stopped
        bool IsSearchActive();

        // Writes the attributes of the specified AdPerson.
        // AdPerson is identified by AdPerson.Dn.
        // Throws RepositoryInaccessibleError if 'AdPerson.Dn' is incorrect
        // Returns the list of attributes not allowed to be changed.
        // Can throw DataModificationError exception.
        IEnumerable<AttrId> WriteAdPerson(ConnectionParams connectionParams, AdPerson adp);

        AdPerson ReadAdPerson(ConnectionParams connectionParams, string dn);

        bool IsAvailable(ConnectionParams connectionParams);
        // the following events are raised in a thread pool thread
        event EventHandler SearchStarted;
        event EventHandler SearchStopped;
        event EventHandler<ItemFoundEventArgs> ItemFound;
    }
}
