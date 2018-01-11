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
#include "apPicsInfoCache.h"

#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringBuilder>
#include <QVariant>

apPicsInfoCache::apPicsInfoCache()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

	QString dbFile = QDir::homePath() + "/.apSortPics/infos.db";

	QDir(QDir::homePath()).mkpath(dbFile);
	db.setDatabaseName(dbFile);

	if (!db.open()) {
		QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
			QObject::tr("Unable to establish a database connection.\n"
						"This example needs SQLite support. Please read "
						"the Qt SQL driver documentation for information how "
						"to build it.\n\n"
						"Click Cancel to exit."), QMessageBox::Cancel);
		return;
	}

	if ( ! db.tables().contains("fileHashes"))
	{
		QSqlQuery query;
		query.exec("create table fileHashes (md5 varchar(32), "
					   "fileName TEXT)");
	}
	if ( ! db.tables().contains("picHashes"))
	{
		QSqlQuery query;
		query.exec("create table picHashes (md5 varchar(32), "
					   "fileName TEXT)");
	}
}

void apPicsInfoCache::addFile(const QString& file, const QString& fileMd5, const QString& picMd5)
{
	if (this->fileHashFromFile(file) != fileMd5)
	{
		QSqlQuery query;

		query.exec(QLatin1String("INSERT INTO fileHashes (md5, fileName) VALUES ('") % fileMd5 % "','" % file % "')");
	}
	if (this->picHashFromFile(file) != picMd5)
	{
		QSqlQuery query;

		query.exec(QLatin1String("INSERT INTO picHashes (md5, fileName) VALUES ('") % picMd5 % "','" % file % "')");
	}
}

QString apPicsInfoCache::fileHashFromFile(const QString& fileName) const
{
	return this->hashFromFile("fileHashes", fileName);
}

QString apPicsInfoCache::picHashFromFile(const QString& fileName) const
{
	return this->hashFromFile("picHashes", fileName);
}

QStringList apPicsInfoCache::filesFromFileHash(const QString& hash) const
{
	return this->filesFromHash("fileHashes", hash);
}

QStringList apPicsInfoCache::filesFromPicHash(const QString& hash) const
{
	return this->filesFromHash("picHashes", hash);
}

QString apPicsInfoCache::hashFromFile(const QString& tableName, const QString& fileName) const
{
	QSqlQuery query;

	if ( ! query.exec(QLatin1String("SELECT fileName FROM ") % tableName % " WHERE fileName='" % fileName % "'"))
	{
		return QString();
	}

	if ( ! query.next())
	{
		return QString();
	}

	QString md5 = query.value(0).toString();

	return md5;
}

QStringList apPicsInfoCache::filesFromHash(const QString& tableName, const QString& hash) const
{
	QSqlQuery query;
	if ( ! query.exec("SELECT fileName FROM " % tableName % "WHERE hash='" + hash + "'"))
	{
		return QStringList();
	}

	QStringList fileNames;

	while (query.next())
	{
		QString fileName = query.value(0).toString();

		fileNames << fileName;
	}

	return fileNames;
}
