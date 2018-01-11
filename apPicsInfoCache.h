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
#ifndef APPICSINFOCACHE_H
#define APPICSINFOCACHE_H

#include <QString>

class apPicsInfoCache
{
public:
	apPicsInfoCache();

	void addFile( const QString& file, const QString& fileMd5, const QString& pixMd5 );

	QString fileHashFromFile( const QString& ) const;

	QString picHashFromFile( const QString& ) const;

	QStringList filesFromFileHash( const QString& ) const;

	QStringList filesFromPicHash( const QString& ) const;

private:
	QString hashFromFile( const QString& tableName, const QString& fileName ) const;

	QStringList filesFromHash( const QString& tableName, const QString& hash ) const;
};

#endif // APPICSINFOCACHE_H
