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
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Media.Imaging;
using System.Collections;

namespace WpfAdBook.ViewModels
{
    public class TextAttributeVM: INotifyPropertyChanged, ICloneable
    {
        private readonly AdPerson _adp;
        public AdAttribute AttrProperties { get; }

        public TextAttributeVM(AdPerson adp, AttrId id)
        {
            _adp = adp;
            AttrProperties = AdAttributes.Get(id);
        }

        public TextAttributeVM(AdPerson adp, string ldapAttrName)
        {
            _adp = adp;
            AttrProperties = AdAttributes.Get(ldapAttrName);
        }

        public bool IsReadOnly {
            get {
                return !_adp.IsAttributeWritable(AttrProperties.Id);
            }
        }
        public string Value {
            get {
                return _adp.get_StringAttr(AttrProperties.LdapName);
            }
            set {
                _adp.set_StringAttr(AttrProperties.LdapName, value);
                RaisePropertyChanged(nameof(Value));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public object Clone()
        {
            var r = new TextAttributeVM(_adp, AttrProperties.Id);
            return r;
        }
    }

    public class BinaryAttributeVM : INotifyPropertyChanged, ICloneable
    {
        private readonly AdPerson _adp;
        public AdAttribute AttrProperties { get; }

        public BinaryAttributeVM(AdPerson adp, AttrId id)
        {
            _adp = adp;
            AttrProperties = AdAttributes.Get(id);
        }

        public BinaryAttributeVM(AdPerson adp, string ldapAttrName)
        {
            _adp = adp;
            AttrProperties = AdAttributes.Get(ldapAttrName);
        }

        public bool IsReadOnly {
            get {
                return !_adp.IsAttributeWritable(AttrProperties.Id);
            }
        }

        public byte[] Value {
            get {
                return _adp.get_BinaryAttr(AttrProperties.LdapName);
            }
            set {
                _adp.set_BinaryAttr(AttrProperties.LdapName, value);
                RaisePropertyChanged(nameof(Value));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public object Clone()
        {
            return new BinaryAttributeVM(_adp, AttrProperties.Id);
        }
    }

    public class TextAttributesVM: IEnumerable<TextAttributeVM>
    {
        private readonly Dictionary<string, TextAttributeVM> _attrs = new Dictionary<string, TextAttributeVM>();

        public TextAttributesVM(IEnumerable<TextAttributeVM> attrs)
        {
            foreach(var attr in attrs) {
                _attrs[attr.AttrProperties.LdapName] = attr;
            }
        }

        public TextAttributeVM this[string ldapAttrName] {
            get {
                return _attrs[ldapAttrName];
            }
        }

        public void CopyFrom(TextAttributesVM ta)
        {
            foreach (KeyValuePair<string, TextAttributeVM> v in ta._attrs) {
                if (_attrs.ContainsKey(v.Key)) {
                    _attrs[v.Key].Value = v.Value.Value;
                }
                else {
                    _attrs.Add(v.Key, (TextAttributeVM)v.Value.Clone());
                }
            }
        }

        public IEnumerator<TextAttributeVM> GetEnumerator()
        {
            return _attrs.Values.GetEnumerator();

        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _attrs.Values.GetEnumerator();
        }
    }

    public class BinaryAttributesVM
    {
        private readonly Dictionary<string, BinaryAttributeVM> _attrs = new Dictionary<string, BinaryAttributeVM>();

        public BinaryAttributesVM(IEnumerable<BinaryAttributeVM> attrs)
        {
            foreach (var attr in attrs) {
                _attrs[attr.AttrProperties.LdapName] = attr;
            }
        }

        public BinaryAttributeVM this[string ldapAttrName] {
            get {
                return _attrs[ldapAttrName];
            }
        }

        public void CopyFrom(BinaryAttributesVM ta)
        {
            foreach (KeyValuePair<string, BinaryAttributeVM> v in ta._attrs) {
                if (_attrs.ContainsKey(v.Key)) {
                    _attrs[v.Key].Value = v.Value.Value;
                }
                else {
                    _attrs.Add(v.Key, (BinaryAttributeVM)v.Value.Clone());
                }
            }
        }
    }

    public class AdPersonVM : INotifyPropertyChanged, IDisposable, ICloneable
    {
        private TextAttributesVM _textAttrs;
        public TextAttributesVM TextAttrs {
            get {
                ThrowIfDisposed();
                return _textAttrs;
            }
            set {
                ThrowIfDisposed();
                if (_textAttrs != value) {
                    _textAttrs = value;
                }
                RaisePropertyChanged();
            }
        }

        private BinaryAttributesVM _binaryAttrs;
        public BinaryAttributesVM BinaryAttrs {
            get {
                ThrowIfDisposed();
                return _binaryAttrs;
            }

            set {
                ThrowIfDisposed();
                if (_binaryAttrs != value) {
                    _binaryAttrs = value;
                }
                RaisePropertyChanged();
            }
        }
        private AdPerson _adp;

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void InitProperties()
        {
            var ta = new List<TextAttributeVM>();
            var ba = new List<BinaryAttributeVM>();

            foreach (var v in AdAttributes.LdapAttrNames) {
                var a = AdAttributes.Get(v);
                if (a.IsString) {
                    ta.Add(new TextAttributeVM(_adp, v));
                }
                else {
                    ba.Add(new BinaryAttributeVM(_adp, v));
                }
            }

            TextAttrs = new TextAttributesVM(ta);
            BinaryAttrs = new BinaryAttributesVM(ba);
        }

        public AdPersonVM()
        {
            _adp = new AdPerson();
            InitProperties();
        }

        public AdPersonVM(AdPerson adp)
        {
            if (null == adp) {
                throw new ArgumentNullException("adp");
            }
            _adp = adp;
            InitProperties();
        }

        public AdPerson GetUnderlyingType()
        {
            ThrowIfDisposed();
            return _adp;
        }

        public void CopyFrom(AdPersonVM adp)
        {
            ThrowIfDisposed();
            TextAttrs.CopyFrom(adp.TextAttrs);
            RaisePropertyChanged(nameof(TextAttrs));
            BinaryAttrs.CopyFrom(adp.BinaryAttrs);
            RaisePropertyChanged(nameof(BinaryAttrs));
        }

        public void SetStringAttr(AttrId attrId, string value)
        {
            ThrowIfDisposed();
            _adp.set_StringAttr(AdAttributes.Get(attrId).LdapName, value);
        }

        public string GetStringAttr(AttrId attrId)
        {
            ThrowIfDisposed();
            return _adp.get_StringAttr(AdAttributes.Get(attrId).LdapName);
        }

        public void SetBinaryAttr(AttrId attrId, byte[] value)
        {
            ThrowIfDisposed();
            _adp.set_BinaryAttr(AdAttributes.Get(attrId).LdapName, value);
        }

        public byte[] GetBinaryAttr(AttrId attrId)
        {
            ThrowIfDisposed();
            return _adp.get_BinaryAttr(AdAttributes.Get(attrId).LdapName);
        }

        public bool IsAttributeWrittable(AttrId id)
        {
            ThrowIfDisposed();
            return _adp.IsAttributeWritable(id);
        }

        public void SetAttributeWritable(AttrId id, bool writable)
        {
            ThrowIfDisposed();
            _adp.SetAttributeWritable(id, writable);
        }

        public string Dn {
            get {
                ThrowIfDisposed();
                return _adp.Dn;
            }
        }

        public object Clone()
        {
            ThrowIfDisposed();
            return new AdPersonVM((AdPerson)_adp.Clone());
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        private void ThrowIfDisposed()
        {
            if (disposedValue) {
                throw new ObjectDisposedException(GetType().FullName);
            }
        }
        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue) {
                if (disposing) {
                    if (_adp != null) {
                        _adp.Dispose();
                        _adp = null;
                    }
                }
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion
    }
}
