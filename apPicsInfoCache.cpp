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
		return false;
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

QStringList apPicsInfoCache::filesFromFileHash(const QString& hash) const
{
	QSqlQuery query;
	query.exec("SELECT fileName FROM fileHashes WHERE hash='" + hash + "'");

	QStringList fileNames;

	while (query.next())
	{
		QString fileName = query.value(0).toString();

		fileNames << fileName;
	}

	return fileNames;
}
