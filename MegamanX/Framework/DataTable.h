#pragma once

class DataTable
{
public:
	enum class Types
	{
		String,
	};
	static const int TotalTypes = 1;

private:
	Types type;
	
public:
	DataTable(Types t) : type(t) {};
	virtual ~DataTable() = default;

	//복사 이동 생성자 막기

	virtual bool Load() = 0;
	virtual void Release() = 0;

};

