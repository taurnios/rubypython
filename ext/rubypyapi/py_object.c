#include "py_object.h"

#include "ptor.h"

RUBY_EXTERN VALUE mRubyPyApi;

VALUE cRubyPyObject;

static VALUE PyStructAlloc(VALUE);
static void PyStructFree(PyStruct*);


//Create a new PyStruct
static
VALUE PyStructAlloc(VALUE klass)
{
	PyStruct* self = ALLOC(PyStruct);
	self->pObject = NULL;
	
	return Data_Wrap_Struct(klass, 0, PyStructFree, self);
}


//Delete a PyStruct
static
void PyStructFree(PyStruct* self)
{
	if(Py_IsInitialized() && self->pObject)
	{
		//Make sure we decrement the object count on our wrapped
		//object before we free the ruby wrapper
		Py_XDECREF(self->pObject);
	}
	free(self);
}

static
VALUE rpRubify(VALUE rbPyObject) {
    VALUE rbObject;
    PyObject* pObject;
    PyStruct* pyStruct;
    
    Data_Get_Struct(rbPyObject, PyStruct, pyStruct);
    
    pObject = pyStruct->pObject;
    
    
    rbObject = ptorObject(pObject);
    
    return rbObject;
}


inline void Init_RubyPyObject() {
	cRubyPyObject = rb_define_class_under(mRubyPyApi,"PyObject", rb_cObject);
        rb_define_alloc_func(cRubyPyObject, PyStructAlloc);
        rb_define_method(cRubyPyObject, "rubify", &rpRubify, 0);
}