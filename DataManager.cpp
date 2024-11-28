#include "DataManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>

const QString FILE_PATH = "data/data.json";
const int CONCENTRATE_TIME = 60;

DataManager* DataManager::getInstance()
{
	static DataManager manager;

	return &manager;
}

void DataManager::readData()
{
	QFile file{ FILE_PATH };
	if (file.open(QIODevice::ReadOnly)) {
		QByteArray fileContent = file.readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent);
		if (!jsonDoc.isNull()) {
			QJsonObject obj = jsonDoc.object();
			for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
				int year = it.key().toInt();
				QJsonObject monthObject = it->toObject();

				QMap<int, QMap<int, int>> monthData;
				for (auto monthIt = monthObject.constBegin(); monthIt != monthObject.constEnd(); ++monthIt) {
					int month = monthIt.key().toInt();
					QJsonObject dayObject = monthIt->toObject();

					QMap<int, int> dayData;
					for (auto dayIt = dayObject.constBegin(); dayIt != dayObject.constEnd(); ++dayIt) {
						int day = dayIt.key().toInt();
						int hours = dayIt.value().toInt();
						dayData.insert(day, hours);
					}

					monthData.insert(month, dayData);
				}

				concentrateData_.insert(year, monthData);
			}
		}

		file.close();
	}
}

bool DataManager::isConcentrate(int year, int month, int day)
{
	if (!concentrateData_.contains(year)) {
		return false;
	}

	if (!concentrateData_[year].contains(month)) {
		return false;
	}

	if (!concentrateData_[year][month].contains(day)) {
		return false;
	}

	return concentrateData_[year][month][day] > CONCENTRATE_TIME;
}

bool DataManager::isConcentrate(QDate date)
{
	return isConcentrate(date.year(), date.month(), date.day());
}

void DataManager::updateData(int year, int month, int day, int value)
{
	if (!concentrateData_.contains(year) || !concentrateData_[year].contains(month) || !concentrateData_[year][month].contains(day)) {
		concentrateData_[year][month][day] = 0;
	}

	concentrateData_[year][month][day] += value;
}

void DataManager::updateData(QDate date, int value)
{
	updateData(date.year(), date.month(), date.day(), value);
}

DataManager::~DataManager()
{
	QJsonObject obj;
	for (auto yearIt = concentrateData_.constBegin(); yearIt != concentrateData_.constEnd(); ++yearIt) {
		int year = yearIt.key();
		QJsonObject yearObj;

		for (auto monthIt = yearIt->constBegin(); monthIt != yearIt->constEnd(); ++monthIt) {
			int month = monthIt.key();
			QJsonObject monthObj;

			for (auto dayIt = monthIt->constBegin(); dayIt != monthIt->constEnd(); ++dayIt) {
				int day = dayIt.key();
				float hours = dayIt.value();
				monthObj.insert(QString::number(day), hours);
			}
			yearObj.insert(QString::number(month), monthObj);
		}
		obj.insert(QString::number(year), yearObj);
	}

	QJsonDocument jsonDoc;
	jsonDoc.setObject(obj);
	QFile file{ FILE_PATH };
	if (file.open(QIODevice::WriteOnly)) {
		file.write(jsonDoc.toJson());

		file.close();
	}
}
