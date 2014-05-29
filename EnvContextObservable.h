#pragma once
#include <vistas/Observer.h>
#include <EnvExtension.h>
#include <MAP.h>
#include <mutex>

class EnvContextObservable : public VI_Observable {
public:
	EnvContextObservable(EnvContext* context);
private:
	EnvContextObservable(const EnvContextObservable& rhs);
	
public:
	int GetCurrentYear() const;
	int GetActiveColumn() const;
	EnvContext* GetEnvContext() const;
	void SetCurrentYear(int year);
	void SetActiveColumn(int column);
	void SetEnvContext(EnvContext* context);
	int GetRunTimeRange() const;
	static int OnHandlerCallback(Map*, NOTIFY_TYPE, int, LONG_PTR, LONG_PTR );


private:
	int _currentYear;
	int _activeColumn;
	EnvContext* _envContext;
	mutable std::recursive_mutex _mutex;
};
