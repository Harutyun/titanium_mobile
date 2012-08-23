/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#include "TiHttpClientObject.h"
#include "TiGenericFunctionObject.h"
#include "NativeHttpClientObject.h"
#include "TiPropertyMapObject.h"
#include "TiPropertySetFunctionObject.h"
#include "TiPropertyGetFunctionObject.h"
#include <string>

const static TiProperty g_tiProperties[] =
{
    {
        "validatesSecureCertificate", TI_PROP_PERMISSION_READ | TI_PROP_PERMISSION_WRITE, N_HTTP_CLIENT_SECURE_SERT
    },

    {
        "timeout", TI_PROP_PERMISSION_READ | TI_PROP_PERMISSION_WRITE, N_HTTP_CLIENT_TIMEOUT
    },

    {
        "onLoad", TI_PROP_PERMISSION_READ | TI_PROP_PERMISSION_WRITE, N_HTTP_CLIENT_ONLOAD
    },

    {
        "onError", TI_PROP_PERMISSION_READ | TI_PROP_PERMISSION_WRITE, N_HTTP_CLIENT_ONERROR
    },
};

TiHttpClientObject::TiHttpClientObject()
    : TiProxy("")
{
}

TiHttpClientObject::~TiHttpClientObject()
{
}

void TiHttpClientObject::addObjectToParent(TiObject* parent)
{
    TiHttpClientObject* obj = new TiHttpClientObject;
    parent->addMember(obj);
    obj->release();
}

TiHttpClientObject* TiHttpClientObject::createHttpClient(NativeObjectFactory* objectFactory)
{
    TiHttpClientObject* obj = new TiHttpClientObject();
    obj->setNativeObjectFactory(objectFactory);
    obj->initializeTiObject(NULL);
    return obj;
}

void TiHttpClientObject::initializeTiObject(TiObject* parentContext)
{
    if (!isInitialized())
    {
        TiProxy::initializeTiObject(parentContext);
        NativeObject* obj = getNativeObjectFactory()->createNativeObject(N_TYPE_HTTPCLIENT);
        setNativeObject(obj);
        obj->release();
    }
}

void TiHttpClientObject::onCreateStaticMembers()
{
    TiProxy::onCreateStaticMembers();
    setTiMappingProperties(g_tiProperties, sizeof(g_tiProperties) / sizeof(*g_tiProperties));
    TiGenericFunctionObject::addGenericFunctionToParent(this, "open", this, _open);
    TiGenericFunctionObject::addGenericFunctionToParent(this, "send", this, _send);
}

void TiHttpClientObject::setTiMappingProperties(const TiProperty* props, int propertyCount)
{
    string name;
    char c[2];
    c[1] = 0;
    for (int i = 0; i < propertyCount; i++)
    {
        TiObject* value = TiPropertyMapObject::addProperty(this, props[i].propertyName, props[i].nativePropertyNumber,
                          _valueModify, _getValue, this);
        // For all properties that have write permissions, add a setter method, e.g., myLabel.text=<my text>; myLabel.setText(<my text>);
        if (props[i].permissions & TI_PROP_PERMISSION_WRITE)
        {
            c[0] = toupper(props[i].propertyName[0]);
            name = "set";
            name += c;
            name += props[i].propertyName + 1;
            TiPropertySetFunctionObject::addPropertySetter(this, value, name.c_str());
        }
        // For all properties that have read permissions, add a getter method, e.g., var test=myLabel.text; var test=myLabel.getText();
        if (props[i].permissions & TI_PROP_PERMISSION_READ)
        {
            c[0] = toupper(props[i].propertyName[0]);
            name = "get";
            name += c;
            name += props[i].propertyName + 1;
            TiPropertyGetFunctionObject::addPropertyGetter(this, value, name.c_str());
        }
        value->release();
    }
}

VALUE_MODIFY TiHttpClientObject::_valueModify(int propertyNumber, TiObject* value, void* context)
{
    TiHttpClientObject* self = (TiHttpClientObject*) context;
    NativeHttpClientObject* object = (NativeHttpClientObject*)self->getNativeObject();
    if (object == NULL)
    {
        return VALUE_MODIFY_NOT_SUPPORTED;
    }
    VALUE_MODIFY modify = VALUE_MODIFY_ALLOW;
    switch (object->setPropertyValue(propertyNumber, value, self))
    {
    case NATIVE_ERROR_OK:
        modify = VALUE_MODIFY_ALLOW;
        break;
    case NATIVE_ERROR_NOTSUPPORTED:
        modify = VALUE_MODIFY_NOT_SUPPORTED;
        break;
    default:
        modify = VALUE_MODIFY_INVALID;
        break;
    }
    object->release();
    return modify;
}

Handle<Value> TiHttpClientObject::_getValue(int propertyNumber, void* context)
{
    TiHttpClientObject* self = (TiHttpClientObject*) context;
    NativeHttpClientObject* object = (NativeHttpClientObject*)self->getNativeObject();
    TiObject value;
    if (object != NULL)
    {
        object->getPropertyValue(propertyNumber, &value, context);
    }
    return value.getValue();
}

Handle<Value> TiHttpClientObject::_open(void* userContext, TiObject* /* caller */, const Arguments& args)
{
    HandleScope handleScope;
    TiHttpClientObject* obj = (TiHttpClientObject*) userContext;
    NativeHttpClientObject* nHttp = (NativeHttpClientObject*) obj->getNativeObject();
    if (args.Length() > 1)
    {
        Handle<String> arg1 = args[0]->ToString();
        Handle<String> arg2 = args[1]->ToString();
        std::string method = *String::Utf8Value(arg1);
        std::string url = *String::Utf8Value(arg2);
        nHttp->open(method, url);
    }
    return Undefined();
}

Handle<Value> TiHttpClientObject::_send(void* userContext, TiObject* /* caller */, const Arguments& args)
{
    HandleScope handleScope;
    TiHttpClientObject* obj = (TiHttpClientObject*) userContext;
    NativeHttpClientObject* nHttp = (NativeHttpClientObject*) obj->getNativeObject();
    std::string data;
    if (args.Length() == 1)
    {
        Handle<String> arg1 = args[0]->ToString();
        data = *String::Utf8Value(arg1);
    }
    nHttp->send(data);
    return Undefined();
}





