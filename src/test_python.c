#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include <stddef.h>
#include <string.h>
#include "test.h"



// Py wrappers

// ATTR A
typedef struct {
    PyObject_HEAD
    AttrTypeA data;
} PyAttrTypeA;

// __new__
static PyObject* PyAttrTypeA_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyAttrTypeA *self;
    self = (PyAttrTypeA *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->data.name = NULL;
        self->data.value = NULL;
    }
    return (PyObject *)self;
}

// __init__
static int PyAttrTypeA_init(PyAttrTypeA *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"name", "value", NULL};
    const char *name = NULL;
    const char *value = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|ss", kwlist, &name, &value)) {
        return -1;
    }

    if (self->data.name) {
        free(self->data.name);
        self->data.name = NULL;
    }
    if (self->data.value) {
        free(self->data.value);
        self->data.value = NULL;
    }

    if (name) self->data.name = strdup(name);
    if (value) self->data.value = strdup(value);

    return 0;
}

// __dealloc__
static void PyAttrTypeA_dealloc(PyAttrTypeA *self) {
    if (self->data.name) free(self->data.name);
    if (self->data.value) free(self->data.value);

    Py_TYPE(self)->tp_free((PyObject *)self);
}

// THIS WILL NOT WORK FOR char* IT HAS TO BE char[]
// static PyMemberDef PyAttrTypeA_members[] = {
//     {"name", T_STRING, offsetof(PyAttrTypeA, data.name), 0, "name string"},
//     {"value", T_STRING, offsetof(PyAttrTypeA, data.value), 0, "value string"},
//     {NULL}
// };

// custom getter
static PyObject* PyAttrTypeA_get_name(PyAttrTypeA *self, void *closure) {
    if (self->data.name) {
        return PyUnicode_FromString(self->data.name);
    }
    Py_RETURN_NONE;
}

static PyObject* PyAttrTypeA_get_value(PyAttrTypeA *self, void *closure) {
    if (self->data.value) {
        return PyUnicode_FromString(self->data.value);
    }
    Py_RETURN_NONE;
}

// custom setter
static int PyAttrTypeA_set_name(PyAttrTypeA *self, PyObject *value, void *closure) {
    if (!PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The name attribute value must be a string");
        return -1;
    }
    const char *tmp = PyUnicode_AsUTF8(value);
    if (!tmp) return -1;
    if (self->data.name) free(self->data.name);
    self->data.name = strdup(tmp);
    return 0;
}

static int PyAttrTypeA_set_value(PyAttrTypeA *self, PyObject *value, void *closure) {
    if (!PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The value attribute value must be a string");
        return -1;
    }
    const char *tmp = PyUnicode_AsUTF8(value);
    if (!tmp) return -1;
    if (self->data.value) free(self->data.value);
    self->data.value = strdup(tmp);
    return 0;
}

static PyGetSetDef PyAttrTypeA_getsetters[] = {
    {"name", (getter)PyAttrTypeA_get_name, (setter)PyAttrTypeA_set_name, "name", NULL},
    {"value", (getter)PyAttrTypeA_get_value, (setter)PyAttrTypeA_set_value, "value", NULL},
    {NULL}
};

static PyTypeObject PyAttrTypeAType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "test.AttrTypeA",
    .tp_basicsize = sizeof(PyAttrTypeA),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "AttrTypeA object",
    .tp_new = PyAttrTypeA_new,
    .tp_init = (initproc)PyAttrTypeA_init,
    .tp_dealloc = (destructor)PyAttrTypeA_dealloc,
    .tp_getset = PyAttrTypeA_getsetters,
};

// ATTR B
typedef struct {
    PyObject_HEAD
    AttrTypeB data;
} PyAttrTypeB;


// Module Definition
static PyModuleDef testmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "test",
    .m_doc = "test module",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_test_module(void) {
    PyObject *m;
    if (PyType_Ready(&PyAttrTypeAType) < 0)
        return NULL;

    m = PyModule_Create(&testmodule);
    if (m == NULL) return NULL;

    Py_INCREF(&PyAttrTypeAType);
    PyModule_AddObject(m, "AttrTypeA", (PyObject*)&PyAttrTypeAType);

    return m;
}
