#pragma once

#include <QMap>
#include <QDate>

class DataManager {
public:
	static DataManager* getInstance();
	void readData();

	bool isConcentrate(int year, int month, int day);
	bool isConcentrate(QDate date);
	void updateData(int year, int month, int day, int value);
	void updateData(QDate date, int value);

private:
	DataManager() {}
	~DataManager();

	// 年 月 日
	QMap<int, QMap<int, QMap<int, int>>> concentrateData_;
};