/***************************************************************************
                          skyguideslistview.h  -  K Desktop Planetarium
                             -------------------
    begin                : 2014/04/07
    copyright            : (C) 2014 by Gioacchino Mazzurco
    email                : gmazzurco89@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QDebug>
#include <QString>
#include <QStringList>

#include <kstandarddirs.h>

#include "skyguideslistmodel.h"
#include "skyguidedata.h"
#include "../skyobjects/skyobject.h"


SkyGuidesListModel::SkyGuidesListModel(const SkyObject * obj) :
	QList<QObject*>(),
	actualSkyObject(NULL)
{
	updateSkyObject(obj);
}

SkyGuidesListModel::SkyGuidesListModel(const SkyGuidesListModel & skglm) :
	QList<QObject*>(skglm),
	actualSkyObject(NULL)
{
	updateSkyObject(skglm.actualSkyObject);
};

SkyGuidesListModel::SkyGuidesListModel() :
	QList<QObject*>(),
	actualSkyObject(NULL)
{};

void SkyGuidesListModel::updateSkyObject(const SkyObject * obj)
{
	if ( obj && obj->hasName() )
	{
		if ( actualSkyObject && (!( (*actualSkyObject) == (*obj) )) )
			return;

		KStandardDirs ksd;
		QStringList guidesPaths = ksd.findAllResources("data", "kstars/skyguides/guides/*/index.html");

		qDebug() << "SkyGuidesListModel::SkyGuidesListModel( " << obj->name() << " ) found " << guidesPaths.count() << " guides." ;

		foreach( QString path, guidesPaths )
		{
			path.chop(10);
			this->append(new SkyGuideData("Jupiter", path));
			qDebug() << path;
		}

		delete actualSkyObject;
		actualSkyObject = obj->clone();
	}
}

SkyGuidesListModel::~SkyGuidesListModel()
{
	delete actualSkyObject;
}

