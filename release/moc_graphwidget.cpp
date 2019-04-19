/****************************************************************************
** Meta object code from reading C++ file 'graphwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gv_test/graphwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GraphWidget_t {
    QByteArrayData data[13];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphWidget_t qt_meta_stringdata_GraphWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "GraphWidget"
QT_MOC_LITERAL(1, 12, 13), // "Nodes_Created"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "Nodes_Error"
QT_MOC_LITERAL(4, 39, 14), // "Generate_Graph"
QT_MOC_LITERAL(5, 54, 6), // "zoomIn"
QT_MOC_LITERAL(6, 61, 7), // "zoomOut"
QT_MOC_LITERAL(7, 69, 16), // "Start_End_Change"
QT_MOC_LITERAL(8, 86, 15), // "Unpack_ShortCut"
QT_MOC_LITERAL(9, 102, 5), // "Edge*"
QT_MOC_LITERAL(10, 108, 5), // "Start"
QT_MOC_LITERAL(11, 114, 16), // "CHP_state_change"
QT_MOC_LITERAL(12, 131, 22) // "Arc_Flags_state_change"

    },
    "GraphWidget\0Nodes_Created\0\0Nodes_Error\0"
    "Generate_Graph\0zoomIn\0zoomOut\0"
    "Start_End_Change\0Unpack_ShortCut\0Edge*\0"
    "Start\0CHP_state_change\0Arc_Flags_state_change"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    0,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   68,    2, 0x0a /* Public */,
       5,    0,   69,    2, 0x0a /* Public */,
       6,    0,   70,    2, 0x0a /* Public */,
       7,    2,   71,    2, 0x0a /* Public */,
       8,    1,   76,    2, 0x0a /* Public */,
      10,    0,   79,    2, 0x0a /* Public */,
      11,    0,   80,    2, 0x0a /* Public */,
      12,    0,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GraphWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GraphWidget *_t = static_cast<GraphWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Nodes_Created((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->Nodes_Error(); break;
        case 2: _t->Generate_Graph(); break;
        case 3: _t->zoomIn(); break;
        case 4: _t->zoomOut(); break;
        case 5: _t->Start_End_Change((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->Unpack_ShortCut((*reinterpret_cast< Edge*(*)>(_a[1]))); break;
        case 7: _t->Start(); break;
        case 8: _t->CHP_state_change(); break;
        case 9: _t->Arc_Flags_state_change(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GraphWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphWidget::Nodes_Created)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GraphWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphWidget::Nodes_Error)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GraphWidget::staticMetaObject = { {
    &QGraphicsView::staticMetaObject,
    qt_meta_stringdata_GraphWidget.data,
    qt_meta_data_GraphWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GraphWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphWidget.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int GraphWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void GraphWidget::Nodes_Created(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GraphWidget::Nodes_Error()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
