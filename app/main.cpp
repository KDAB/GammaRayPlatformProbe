/*
    Copyright (C) 2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
    Author: Volker Krause <volker.krause@kdab.com>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <QDebug>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QtAndroid>

#include <gammaray/core/server.h>

Q_DECL_EXPORT int main(int argc, char **argv)
{
    QCoreApplication::setApplicationName(QStringLiteral("GammaRay Android Platform Probe"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDAB"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kdab.com"));

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv); // QApplication, so we get the QStyle tool
    QQmlApplicationEngine engine;
    engine.load(QStringLiteral("qrc:/main.qml"));

    // give the GammaRay probe time to initialize networking
    QTimer::singleShot(10, &app, [&engine]() {
        const auto server = qobject_cast<GammaRay::Server*>(GammaRay::Endpoint::instance());
        engine.rootContext()->setContextProperty("_serverAddress", server->externalAddress().toString());
        QObject::connect(server, &GammaRay::Server::externalAddressChanged, &engine, [server, &engine]() {
            engine.rootContext()->setContextProperty("_serverAddress", server->externalAddress().toString());
        });
    });

    QtAndroid::hideSplashScreen();
    return app.exec();
}