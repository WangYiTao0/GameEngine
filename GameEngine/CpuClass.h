#pragma once

#include <Pdh.h>

#pragma comment(lib,"Pdh.lib")
class CpuClass
{
public:
	CpuClass();
	CpuClass(const CpuClass&);
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu = false;
	HQUERY m_queryHandle = NULL;
	HCOUNTER m_counterHandle = 0;
	unsigned long m_lastSampleTime = 0;
	long m_cpuUsage = 0;
};