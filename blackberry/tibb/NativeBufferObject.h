/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef NATIVEBUFFEROBJECT_H_
#define NATIVEBUFFEROBJECT_H_

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <v8.h>
#pragma GCC diagnostic warning "-Wunused-parameter"

#include "NativeProxyObject.h"
#include <qbytearray>

using namespace v8;

enum NATIVE_BUFFER_PROP
{
    N_BUFFER_PROP_UNDEFINED
    , N_BUFFER_PROP_BYTEORDER
    , N_BUFFER_PROP_LENGTH
    , N_BUFFER_PROP_TYPE
    , N_BUFFER_PROP_VALUE

    /* This MUST be the last element */
    , N_PLATFORM_PROP_LAST
};

class TiObject;
class TiEventContainerFactory;

/*
 * NativeBufferObject
 *
 * NativeBufferObject methods
 */
class NativeBufferObject : public NativeProxyObject
{
public:
    static NativeBufferObject* createBuffer();
    int getObjectType() const;
    int setPropertyValue(size_t propertyNumber, TiObject* obj);
    int getPropertyValue(size_t propertyNumber, TiObject* obj);
    int getByteOrder(TiObject* obj);
    int getLength(TiObject* obj);
    int setLength(TiObject* obj);
    int getType(TiObject* obj);
    int getValue(TiObject* obj);
    int setValue(TiObject* obj);
    void setupEvents(TiEventContainerFactory* containerFactory);

    void clear();

protected:
    virtual ~NativeBufferObject();

private:
    NativeBufferObject() {};
    NativeBufferObject(const NativeBufferObject&);
    NativeBufferObject& operator=(const NativeBufferObject&);
    // Buffer containing raw data
    QByteArray internalData_;
};



#endif /* NATIVEBUFFEROBJECT_H_ */
