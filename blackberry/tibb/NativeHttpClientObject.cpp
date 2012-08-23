/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#include "NativeHttpClientObject.h"

#include "NativeControlObject.h"
#include "TiObject.h"
#include <QUrl.h>
#include <QtNetwork/qnetworkrequest.h>
#include "TiV8Event.h"
#include "TiHttpClientObject.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <v8.h>
#pragma GCC diagnostic warning "-Wunused-parameter"

using namespace v8;

#define PROP_SETGET_FUNCTION(NAME)      prop_##NAME

#define PROP_SETGET(NAME)               static int prop_##NAME(NativeHttpClientObject* httpClient, TiObject* obj, void * userContext) \
    {\
        return httpClient->NAME(obj, userContext);\
    }

#define GET_ARRAY_SIZE(ARRAY)           (int)(sizeof(ARRAY)/sizeof(*(ARRAY)))

typedef int (*NATIVE_PROPSETGET_CALLBACK)(NativeHttpClientObject*, TiObject*, void*);

struct NATIVE_PROPSETGET_SETTING
{
    NATIVE_HTTP_CLIENT_PROP propNumber;
    NATIVE_PROPSETGET_CALLBACK setter;
    NATIVE_PROPSETGET_CALLBACK getter;
};

class SetGetHTTPClientProperties
{
public:
    SetGetHTTPClientProperties(const NATIVE_PROPSETGET_SETTING* map, int mapEntries)
    {
        setters_ = new NATIVE_PROPSETGET_CALLBACK[N_HTTP_CLIENT_PROP_LAST];
        memset(setters_, 0, sizeof(NATIVE_PROPSETGET_CALLBACK) * N_HTTP_CLIENT_PROP_LAST);
        getters_ = new NATIVE_PROPSETGET_CALLBACK[N_HTTP_CLIENT_PROP_LAST];
        memset(getters_, 0, sizeof(NATIVE_PROPSETGET_CALLBACK) * N_HTTP_CLIENT_PROP_LAST);
        for (int i = 0; i < mapEntries; i++)
        {
            setters_[map[i].propNumber] = map[i].setter;
            getters_[map[i].propNumber] = map[i].getter;
        }
    }
    ~SetGetHTTPClientProperties()
    {
        if (setters_ != NULL)
        {
            delete[] setters_;
            setters_ = NULL;
        }
        if (getters_ != NULL)
        {
            delete[] getters_;
            getters_ = NULL;
        }
    }
    NATIVE_PROPSETGET_CALLBACK GetSetterCallback(size_t prop)
    {
        if (prop >= (std::size_t)N_HTTP_CLIENT_PROP_LAST)
        {
            return NULL;
        }
        return setters_[prop];
    }
    NATIVE_PROPSETGET_CALLBACK GetGetterCallback(size_t prop)
    {
        if (prop >= (std::size_t)N_HTTP_CLIENT_PROP_LAST)
        {
            return NULL;
        }
        return getters_[prop];
    }
private:
    SetGetHTTPClientProperties(const SetGetHTTPClientProperties&);
    SetGetHTTPClientProperties& operator=(const SetGetHTTPClientProperties&);
    NATIVE_PROPSETGET_CALLBACK* setters_;
    NATIVE_PROPSETGET_CALLBACK* getters_;
};

PROP_SETGET(setValidateSecureSertificate)
int NativeHttpClientObject::setValidateSecureSertificate(TiObject* obj, void* /*userContext*/)
{
    int error = NativeControlObject::getBoolean(obj, &validateSecureSert_);
    if (!N_SUCCEEDED(error))
    {
        return error;
    }
    return NATIVE_ERROR_OK;
}

PROP_SETGET(getValidateSecureSertificate)
int NativeHttpClientObject::getValidateSecureSertificate(TiObject* obj, void* /*userContext*/)
{
    obj->setValue(Boolean::New(validateSecureSert_));
    return NATIVE_ERROR_OK;
}

PROP_SETGET(setTimeout)
int NativeHttpClientObject::setTimeout(TiObject* obj, void* /*userContext*/)
{
    int error = NativeControlObject::getInteger(obj, &timeout_);
    if (!N_SUCCEEDED(error))
    {
        return error;
    }
    return NATIVE_ERROR_OK;
}

PROP_SETGET(getTimeout)
int NativeHttpClientObject::getTimeout(TiObject* obj, void* /*userContext*/)
{
    obj->setValue(Number::New(timeout_));
    return NATIVE_ERROR_OK;
}


PROP_SETGET(setOnLoadCallback)
int NativeHttpClientObject::setOnLoadCallback(TiObject* obj, void* userContext)
{
    Handle<Value> value = obj->getValue();
    if (value->IsFunction())
    {
        TiHttpClientObject* context = (TiHttpClientObject*)userContext;
        Handle<Object> source = Handle<Object>::Cast(context->getValue());
        TiV8Event* connectEvent = TiV8Event::createEvent("onLoad", Handle<Function>::Cast(value), source);
        setEventHandler("onLoad", connectEvent);
        connectEvent->release();
        return NATIVE_ERROR_OK;
    }
    return NATIVE_ERROR_INVALID_ARG;
}

PROP_SETGET(getOnLoadCallback)
int NativeHttpClientObject::getOnLoadCallback(TiObject* /*obj*/, void* /*userContext*/)
{
    // TODO: Implement this function
    return NATIVE_ERROR_NOTSUPPORTED;
}

PROP_SETGET(setOnErrorCallback)
int NativeHttpClientObject::setOnErrorCallback(TiObject* obj, void* userContext)
{
    Handle<Value> value = obj->getValue();
    if (value->IsFunction())
    {
        TiHttpClientObject* context = (TiHttpClientObject*)userContext;
        Handle<Object> source = Handle<Object>::Cast(context->getValue());
        TiV8Event* connectEvent = TiV8Event::createEvent("onError", Handle<Function>::Cast(value), source);
        setEventHandler("onError", connectEvent);
        connectEvent->release();
        return NATIVE_ERROR_OK;
    }
    return NATIVE_ERROR_INVALID_ARG;
}

PROP_SETGET(getOnErrorCallback)
int NativeHttpClientObject::getOnErrorCallback(TiObject* /*obj*/, void* /*userContext*/)
{
    // TODO: Implement this function
    return NATIVE_ERROR_NOTSUPPORTED;
}

const static NATIVE_PROPSETGET_SETTING g_HttpClientPropSetGet[] =
{
    {N_HTTP_CLIENT_SECURE_SERT, PROP_SETGET_FUNCTION(setValidateSecureSertificate), PROP_SETGET_FUNCTION(getValidateSecureSertificate)},
    {N_HTTP_CLIENT_TIMEOUT, PROP_SETGET_FUNCTION(setTimeout), PROP_SETGET_FUNCTION(getTimeout)},
    {N_HTTP_CLIENT_ONLOAD, PROP_SETGET_FUNCTION(setOnLoadCallback), PROP_SETGET_FUNCTION(getOnLoadCallback)},
    {N_HTTP_CLIENT_ONERROR, PROP_SETGET_FUNCTION(setOnErrorCallback), PROP_SETGET_FUNCTION(getOnErrorCallback)}
};

static SetGetHTTPClientProperties g_HttpClientProps(g_HttpClientPropSetGet, GET_ARRAY_SIZE(g_HttpClientPropSetGet));

int NativeHttpClientObject::setPropertyValue(size_t propertyNumber, TiObject* obj, void* userContext)
{
    NATIVE_PROPSETGET_CALLBACK cb = g_HttpClientProps.GetSetterCallback(propertyNumber);
    if (cb == NULL)
    {
        return NATIVE_ERROR_NOTSUPPORTED;
    }
    return (cb)(this, obj, userContext);
}

int NativeHttpClientObject::getPropertyValue(size_t propertyNumber, TiObject* obj, void* userContext)
{
    NATIVE_PROPSETGET_CALLBACK cb = g_HttpClientProps.GetGetterCallback(propertyNumber);
    if (cb == NULL)
    {
        return NATIVE_ERROR_NOTSUPPORTED;
    }
    return (cb)(this, obj, userContext);
}


NativeHttpClientObject::NativeHttpClientObject() :
    networkManager_(new QNetworkAccessManager()),
    operation_(QNetworkAccessManager::UnknownOperation),
    url_(NULL),
    replyHandler_(NULL),
    validateSecureSert_(false),
    timeout_(-1),
    eventContainer_(NULL)
{

}

NativeHttpClientObject::~NativeHttpClientObject()
{
    delete url_;
    delete replyHandler_;
    delete eventContainer_;
}

int NativeHttpClientObject::getObjectType() const
{
    return N_TYPE_HTTPCLIENT;
}

NativeHttpClientObject* NativeHttpClientObject::createHttpClient()
{
    return new NativeHttpClientObject;
}

void NativeHttpClientObject::setupEvents(TiEventContainerFactory* containerFactory)
{
    NativeProxyObject::setupEvents(containerFactory);
    if (eventContainer_ == NULL)
    {
        eventContainer_ = containerFactory->createEventContainer();
    }
    replyHandler_ = new ReplyHandler(this, eventContainer_);
    events_.insert("onLoad", EventPairSmartPtr(eventContainer_, replyHandler_));
    events_.insert("onError", EventPairSmartPtr(eventContainer_, replyHandler_));
}

void NativeHttpClientObject::open(std::string method, std::string url)
{
    if (method == "GET")
    {
        operation_ = QNetworkAccessManager::GetOperation;
    }
    else if (method == "POST")
    {
        operation_ = QNetworkAccessManager::PostOperation;
    }
    else
    {
        //TODO
    }
    url_ = new QUrl(QString(url.c_str()));
}

void NativeHttpClientObject::send(std::string data)
{
    Q_ASSERT(networkManager_);

    if (url_ == NULL)
    {
        //httpClient.send was called before calling httpClient.open
        return;
    }

    QNetworkRequest request(*url_);
    QNetworkReply* reply;
    switch (operation_)
    {
    case QNetworkAccessManager::GetOperation:
        reply = networkManager_->get(request);
        break;
    case QNetworkAccessManager::PostOperation:
        reply = networkManager_->post(request, QByteArray(data.c_str()));
        break;
    default:
        return;
    }
    QObject::connect(reply, SIGNAL(finished()), replyHandler_, SLOT(reply()));
}
