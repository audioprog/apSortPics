/*
MIT License

Copyright (c) 2016 audioprog

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef APPICHASHMANAGER_H
#define APPICHASHMANAGER_H

#include "apPicInfo.h"

#include <QQueue>
#include <QSharedPointer>
#include <QTimer>

class apPicHashCalculator;

class apPicHashManagerNotifier : public QObject
{
	Q_OBJECT

signals:
	void signalFinished( QString );
};

class apPicHashManager
{
public:
	apPicHashManager();

	~apPicHashManager();

	static void addPicInfo( QSharedPointer<apPicInfo> info );

	apPicHashManagerNotifier* getNotifier();

private:
	void start();

private:
	static apPicHashManager* self;

private:
	QQueue<QSharedPointer<apPicInfo>> queue;

	QList<apPicHashCalculator*> threads;

	QTimer timer;

	bool breakQueue = false;

	apPicHashManagerNotifier notifier;
};

#endif // APPICHASHMANAGER_H
