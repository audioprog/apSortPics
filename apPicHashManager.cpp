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
#include "apPicHashManager.h"

#include "apPicHashCalculator.h"

#include <QApplication>
#include <QThread>

apPicHashManager* apPicHashManager::self = new apPicHashManager;

apPicHashManager::apPicHashManager()
{
	this->timer.setInterval(1);
	this->timer.setSingleShot(true);

	QObject::connect(&this->timer, &QTimer::timeout, [this]()
	{
		this->start();
	});
}

apPicHashManager::~apPicHashManager()
{
	this->breakQueue = true;

	this->queue.clear();

	while ( ! this->threads.isEmpty())
	{
		QApplication::processEvents();
	}
}

void apPicHashManager::addPicInfo(QSharedPointer<apPicInfo> info)
{
	if (self->breakQueue)
	{
		return;
	}

	self->queue << info;

	self->start();
}

void apPicHashManager::start()
{
	while ( ! this->queue.isEmpty() &&
		   this->threads.count() < QThread::idealThreadCount())
	{
		apPicHashCalculator* calculator = new apPicHashCalculator(this->queue.dequeue());
		QString fileName = calculator->getFileName();

		QObject::connect(calculator, &apPicHashCalculator::destroyed, &this->timer, [this, calculator, fileName](QObject*)
		{
			this->threads.removeAll(calculator);

			this->timer.start();

			this->notifier.signalFinished(fileName);
		});
		this->threads << calculator;
	}
}
