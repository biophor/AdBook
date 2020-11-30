#pragma once
#include <QByteArray>
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT Password final
    {
    public:
        Password() = default;
        Password(const Password &);
        Password(Password &&) = default;
        Password & operator = (Password &&) = default;
        Password & operator = (const Password &);
        ~Password() = default;

        bool TryToSet(const QString & plainTextPassword);
        bool TryToGetAsPlainTextString(QString & plainTextString) const;

        QString Serialize() const;
        void Deserialize(const QString & serializedPassword);

        bool IsEmpty() const;

        friend bool operator == (const Password & p1, const Password & p2) {
            return p1._encodedData == p2._encodedData;
        }
    private:
        std::vector<BYTE> _encodedData;
    };

}
