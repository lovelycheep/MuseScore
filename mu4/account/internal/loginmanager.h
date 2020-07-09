//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2014 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __LOGINMANAGER_H__
#define __LOGINMANAGER_H__

#include "config.h"

namespace Ms {
class ApiRequest;

//---------------------------------------------------------
//   LoginManager
//---------------------------------------------------------

class LoginManager : public QObject
{
    Q_OBJECT

    enum class RequestType
    {
        LOGIN,
        LOGIN_REFRESH,
        GET_USER_INFO,
        GET_SCORE_INFO,
        UPLOAD_SCORE,
        GET_MEDIA_URL,
    };

    static constexpr int MAX_UPLOAD_TRY_COUNT = 5;
    static constexpr int MAX_REFRESH_LOGIN_RETRY_COUNT = 2;

    QNetworkAccessManager* _networkManager = nullptr;

    QAction* _uploadAudioMenuAction = nullptr;
    QString _accessToken;
    QString _refreshToken;
    QString _userName;
    QUrl _avatar;
    int _uid = -1;

    QString _updateScoreDataPath;

    QString _mediaUrl;
    QFile* _mp3File = nullptr;
    int _uploadTryCount = 0;

    QProgressDialog* _progressDialog = nullptr;

    void onReplyFinished(ApiRequest*, RequestType);
    void handleReply(QNetworkReply*, RequestType);
    static QString getErrorString(QNetworkReply*, const QJsonObject&);

    void onGetUserReply(QNetworkReply* reply, int code, const QJsonObject& replyContent);
    void onLoginReply(QNetworkReply* reply, int code, const QJsonObject& replyContent);
    void onLoginRefreshReply(QNetworkReply* reply, int code, const QJsonObject& replyContent);
    void onUploadReply(QNetworkReply* reply, int code, const QJsonObject& replyContent);
    void onGetScoreInfoReply(QNetworkReply* reply, int code, const QJsonObject& replyContent);
    void onGetMediaUrlReply(QNetworkReply* reply, int code, const QJsonObject& replyContent);

    ApiRequest* buildLoginRefreshRequest() const;

    bool load();
    bool save();
    void getUser();

#ifdef USE_WEBENGINE
    void loginInteractive();
#endif

signals:
    void loginError(const QString& error);
    void loginSuccess();
    void getUserError(const QString& error);
    void getUserSuccess();
    void getScoreError(const QString& error);
    void getScoreSuccess(const QString& title, const QString& description, bool priv, const QString& license,
                         const QString& tags, const QString& url);
    void uploadError(const QString& error);
    void uploadSuccess(const QString& url, const QString& nid, const QString& vid);
    void tryLoginSuccess();
    void mediaUploadSuccess();

private slots:
    void uploadMedia();
    void mediaUploadFinished();
    void mediaUploadProgress(qint64, qint64);

    void onTryLoginSuccess();
    void onTryLoginError(const QString&);

public slots:
    void tryLogin();

public:
    explicit LoginManager(QObject *parent = nullptr);

    LoginManager(QAction* uploadAudioMenuAction, QProgressDialog* progress, QObject* parent = 0);

    void login(QString login, QString password);
    bool logout();

    void upload(const QString& path, int nid, const QString& title);
    void updateScoreData(const QString& nid, bool newScore);
    void getScoreInfo(int nid);
    void getMediaUrl(const QString& nid, const QString& vid, const QString& format);

    const QString& userName() const { return _userName; }
    const QUrl& avatar() const { return _avatar; }
};
}

#endif