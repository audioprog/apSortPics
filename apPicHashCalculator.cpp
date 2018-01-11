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
#include "apPicHashCalculator.h"

#include "apPicInfo.h"

#include <QBuffer>
#include <QCryptographicHash>
#include <QFile>
#include <QImage>
#include <QImageReader>

#include <QtConcurrentRun>

SapHash apPicHashCalculator::fileHashCreator( const QString& fileName )
{
	SapHash hash;

	QFile file(fileName);
	if (file.exists())
	{
		if (file.open(QFile::ReadOnly))
		{
			QByteArray ba = file.readAll();

			file.close();


			QByteArray md5 = QCryptographicHash::hash(ba, QCryptographicHash::Md5);

			hash.fileHash = md5.toHex();


			QBuffer buffer(&ba);

			QImage image = QImageReader(&buffer, fileName.section('.', -1).toLatin1()).read();

			{
				QByteArray imageBytes = QByteArray::fromRawData(reinterpret_cast<char*>(image.bits()), image.sizeInBytes());

				QByteArray imageMd5 = QCryptographicHash::hash(imageBytes, QCryptographicHash::Md5);

				hash.picHash = imageMd5.toHex();
			}
		}
	}

	return hash;
}

apPicHashCalculator::apPicHashCalculator(QSharedPointer<apPicInfo> picInfo)
	: picInfo(picInfo)
{
	QObject::connect(&watcher, &QFutureWatcherBase::finished, this, &apPicHashCalculator::threadFinished);

	// Start the computation.
	this->picHasher = QtConcurrent::run(&apPicHashCalculator::fileHashCreator, this->picInfo->fileName);
	watcher.setFuture(this->picHasher);
}

const QString&apPicHashCalculator::getFileName() const
{
	return this->picInfo->fileName;
}

void apPicHashCalculator::threadFinished()
{
	SapHash hash = this->picHasher.result();

	this->picInfo->fileHash = hash.fileHash;
	this->picInfo->picHash = hash.picHash;

	this->deleteLater();
}
