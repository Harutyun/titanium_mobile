/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef TIHTTPCLIENTOBJECT_H_
#define TIHTTPCLIENTOBJECT_H_

#include "TiProxy.h"

/*
 * TiHttpClientObject
 *
 * Implements HTTPClient interface
 *
 * Titanium.Network namespace
 */

class TiHttpClientObject : public TiProxy
{
public:
    static void addObjectToParent(TiObject* parent);
    static TiHttpClientObject* createHttpClient(NativeObjectFactory* objectFactory);
    virtual void setTiMappingProperties(const TiProperty* prop, int propertyCount);

protected:
    virtual ~TiHttpClientObject();
    virtual void onCreateStaticMembers();
    virtual void initializeTiObject(TiObject* parentContext);

private:
    static VALUE_MODIFY _valueModify(int propertyNumber, TiObject* value, void* context);
    static Handle<Value> _getValue(int propertyNumber, void* context);
    static Handle<Value> _open(void* userContext, TiObject* caller, const Arguments& args);
    static Handle<Value> _send(void* userContext, TiObject* caller, const Arguments& args);

    TiHttpClientObject();
    TiHttpClientObject(const TiHttpClientObject&);
    TiHttpClientObject& operator=(const TiHttpClientObject&);
};


#endif /* TIHTTPCLIENTOBJECT_H_ */
