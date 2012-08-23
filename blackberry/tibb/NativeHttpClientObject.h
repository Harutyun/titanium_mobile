/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef NATIVEHTTPCLIENTOBJECT_H_
#define NATIVEHTTPCLIENTOBJECT_H_

#include "NativeProxyObject.h"
#include <Qt/qnetworkaccessmanager.h>
#include <string>
#include <qobject.h>
#include <Qt/qnetworkreply.h>
#include "TiEventContainer.h"

class TiObject;
class TiEventContainerFactory;
class QUrl;
class NativeHttpClientObject;
class ReplyHandler;

enum NATIVE_HTTP_CLIENT_PROP
{
    N_HTTP_CLIENT_PROP_UNDEFINED
    , N_HTTP_CLIENT_SECURE_SERT
    , N_HTTP_CLIENT_TIMEOUT
    , N_HTTP_CLIENT_ONLOAD
    , N_HTTP_CLIENT_ONERROR

    /* This MUST be the last element */
    , N_HTTP_CLIENT_PROP_LAST
};

/**
 * NativeHttpClientObject
 */

class NativeHttpClientObject : public NativeProxyObject
{
public:
    static NativeHttpClientObject* createHttpClient();
    int getObjectType() const;
    void setupEvents(TiEventContainerFactory* containerFactory);
    void open(std::string method, std::string url);
    void send(std::string data);
    int setPropertyValue(size_t propertyNumber, TiObject* obj, void* userContext);
    int getPropertyValue(size_t propertyNumber, TiObject* obj, void* userContext);

    //Properties
    int setValidateSecureSertificate(TiObject* obj, void* /*userContext*/);
    int getValidateSecureSertificate(TiObject* obj, void* /*userContext*/);
    int setTimeout(TiObject* obj, void* /*userContext*/);
    int getTimeout(TiObject* obj, void* /*userContext*/);
    int setOnLoadCallback(TiObject* obj, void* userContext);
    int getOnLoadCallback(TiObject* /*obj*/, void* /*userContext*/);
    int setOnErrorCallback(TiObject* obj, void* userContext);
    int getOnErrorCallback(TiObject* /*obj*/, void* /*userContext*/);

protected:
    virtual ~NativeHttpClientObject();

private:
    NativeHttpClientObject();
    NativeHttpClientObject(const NativeHttpClientObject&);
    NativeHttpClientObject& operator=(const NativeHttpClientObject&);

    //TODO: probably this instance should be one per application (needs to understand)
    QNetworkAccessManager* networkManager_;
    QNetworkAccessManager::Operation operation_;
    QUrl* url_;
    ReplyHandler* replyHandler_;
    bool validateSecureSert_;
    int timeout_;
    TiEventContainer* eventContainer_;
};

class ReplyHandler : public QObject
{
    Q_OBJECT

public:
    ReplyHandler(NativeHttpClientObject* owner, TiEventContainer* eventContainer)
        : owner_(owner)
        , eventContainer_(eventContainer)
    {
    }

public slots:
    void reply()
    {
        QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

        if (reply)
        {
            if ((reply->error()) == QNetworkReply::NoError)
            {
                //read data from reply
                QByteArray ar = reply->readAll();
                eventContainer_->fireEvent();
            }
            else
            {
                //get http status code
                int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            }
            reply->deleteLater();
        }
    }

private:
    NativeHttpClientObject* owner_;
    TiEventContainer* eventContainer_;
};


#endif /* NATIVEHTTPCLIENTOBJECT_H_ */
