/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../login.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Login_t {
    QByteArrayData data[18];
    char stringdata0[254];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Login_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Login_t qt_meta_stringdata_Login = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Login"
QT_MOC_LITERAL(1, 6, 5), // "close"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 20), // "on_btn_login_clicked"
QT_MOC_LITERAL(4, 34, 21), // "on_btn_regist_clicked"
QT_MOC_LITERAL(5, 56, 14), // "slot_minWindow"
QT_MOC_LITERAL(6, 71, 16), // "slot_closeWindow"
QT_MOC_LITERAL(7, 88, 16), // "slot_setLanguage"
QT_MOC_LITERAL(8, 105, 11), // "slot_actGrp"
QT_MOC_LITERAL(9, 117, 8), // "QAction*"
QT_MOC_LITERAL(10, 126, 3), // "act"
QT_MOC_LITERAL(11, 130, 11), // "slot_timer1"
QT_MOC_LITERAL(12, 142, 11), // "slot_timer2"
QT_MOC_LITERAL(13, 154, 25), // "on_cBox_account_activated"
QT_MOC_LITERAL(14, 180, 5), // "index"
QT_MOC_LITERAL(15, 186, 23), // "on_btn_edit_pwd_clicked"
QT_MOC_LITERAL(16, 210, 35), // "on_cBox_account_currentIndexC..."
QT_MOC_LITERAL(17, 246, 7) // "refresh"

    },
    "Login\0close\0\0on_btn_login_clicked\0"
    "on_btn_regist_clicked\0slot_minWindow\0"
    "slot_closeWindow\0slot_setLanguage\0"
    "slot_actGrp\0QAction*\0act\0slot_timer1\0"
    "slot_timer2\0on_cBox_account_activated\0"
    "index\0on_btn_edit_pwd_clicked\0"
    "on_cBox_account_currentIndexChanged\0"
    "refresh"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Login[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    1,   85,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    1,   90,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,
      16,    1,   94,    2, 0x08 /* Private */,
      17,    0,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,

       0        // eod
};

void Login::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Login *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->close(); break;
        case 1: _t->on_btn_login_clicked(); break;
        case 2: _t->on_btn_regist_clicked(); break;
        case 3: _t->slot_minWindow(); break;
        case 4: _t->slot_closeWindow(); break;
        case 5: _t->slot_setLanguage(); break;
        case 6: _t->slot_actGrp((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 7: _t->slot_timer1(); break;
        case 8: _t->slot_timer2(); break;
        case 9: _t->on_cBox_account_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_btn_edit_pwd_clicked(); break;
        case 11: _t->on_cBox_account_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->refresh(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Login::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Login::close)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Login::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Login.data,
    qt_meta_data_Login,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Login::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Login::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Login.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Login::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Login::close()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
