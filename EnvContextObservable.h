#pragma once
#include <vistas/Observer.h>
#include <EnvExtension.h>
#include <MAP.h>

class EnvContextObservable : public VI_Observable {
private:
	EnvContextObservable();
	EnvContextObservable(const EnvContextObservable& rhs);
	
public:
	int GetCurrentYear() const;
	int GetActiveColumn() const;
	EnvContext* GetEnvContext() const;
	void SetCurrentYear(int year);
	void SetActiveColumn(int column);
	void SetEnvContext(EnvContext* context);
	static int OnHandlerCallback(Map*, NOTIFY_TYPE, int, LONG_PTR, LONG_PTR );

public:
	static EnvContextObservable INSTANCE;

private:
	int _currentYear;
	int _activeColumn;
	EnvContext* _envContext;
};
