/****************************************************************************
** Meta object code from reading C++ file 'formzastav.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "formzastav.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formzastav.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FormZastav_t {
    QByteArrayData data[12];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FormZastav_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FormZastav_t qt_meta_stringdata_FormZastav = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FormZastav"
QT_MOC_LITERAL(1, 11, 4), // "init"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 7), // "destroy"
QT_MOC_LITERAL(4, 25, 13), // "keyPressEvent"
QT_MOC_LITERAL(5, 39, 10), // "QKeyEvent*"
QT_MOC_LITERAL(6, 50, 1), // "e"
QT_MOC_LITERAL(7, 52, 15), // "mousePressEvent"
QT_MOC_LITERAL(8, 68, 12), // "QMouseEvent*"
QT_MOC_LITERAL(9, 81, 10), // "SetKatalog"
QT_MOC_LITERAL(10, 92, 5), // "char*"
QT_MOC_LITERAL(11, 98, 14) // "languageChange"

    },
    "FormZastav\0init\0\0destroy\0keyPressEvent\0"
    "QKeyEvent*\0e\0mousePressEvent\0QMouseEvent*\0"
    "SetKatalog\0char*\0languageChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FormZastav[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    1,   46,    2, 0x0a /* Public */,
       7,    1,   49,    2, 0x0a /* Public */,
       9,    1,   52,    2, 0x0a /* Public */,
      11,    0,   55,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    6,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void,

       0        // eod
};

void FormZastav::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FormZastav *_t = static_cast<FormZastav *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->destroy(); break;
        case 2: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 3: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->SetKatalog((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 5: _t->languageChange(); break;
        default: ;
        }
    }
}

const QMetaObject FormZastav::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FormZastav.data,
      qt_meta_data_FormZastav,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FormZastav::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FormZastav::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FormZastav.stringdata0))
        return static_cast<void*>(const_cast< FormZastav*>(this));
    return QDialog::qt_metacast(_clname);
}

int FormZastav::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
