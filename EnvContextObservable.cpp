#include "stdafx.h"
#include "EnvContextObservable.h"


using namespace std;

EnvContextObservable::EnvContextObservable(EnvContext* context) {
	_envContext = context;
	_envContext->pMapLayer->m_pMap->InstallNotifyHandler(
		EnvContextObservable::OnHandlerCallback, (LONG_PTR)this);
	SetCurrentYear(_envContext->yearOfRun);
	SetActiveColumn(_envContext->pMapLayer->m_activeField);
}

int EnvContextObservable::GetCurrentYear() const {
	lock_guard<recursive_mutex> lock(_mutex);
	return _currentYear;
}

int EnvContextObservable::GetActiveColumn() const {
	lock_guard<recursive_mutex> lock(_mutex);
	return _activeColumn;
}

EnvContext* EnvContextObservable::GetEnvContext() const {
	lock_guard<recursive_mutex> lock(_mutex);
	return _envContext;
}

void EnvContextObservable::SetCurrentYear(int year) {
	lock_guard<recursive_mutex> lock(_mutex);
	_currentYear = year;
}

void EnvContextObservable::SetActiveColumn(int column) {
	lock_guard<recursive_mutex> lock(_mutex);
	_activeColumn = column;
}

void EnvContextObservable::SetEnvContext(EnvContext* context) {
	lock_guard<recursive_mutex> lock(_mutex);
	if (_envContext != context) {
		if (_envContext) {
			_envContext->pMapLayer->m_pMap->RemoveNotifyHandler(
				EnvContextObservable::OnHandlerCallback, (LONG_PTR)this);
		}
		_envContext = context;
		_envContext->pMapLayer->m_pMap->InstallNotifyHandler(
			EnvContextObservable::OnHandlerCallback, (LONG_PTR)this);
		SetCurrentYear(_envContext->yearOfRun);
		SetActiveColumn(_envContext->pMapLayer->m_activeField);
	}
}

int EnvContextObservable::OnHandlerCallback(
	Map* map, NOTIFY_TYPE what, int a0, LONG_PTR a1,LONG_PTR extra) 
{
	if (what == NT_ACTIVEATTRIBUTECHANGED) {
		EnvContextObservable* thisObject = (EnvContextObservable*)extra;
		thisObject->SetActiveColumn(thisObject->_envContext->pMapLayer->m_activeField);
		thisObject->NotifyObservers();
	}
	return 1;
}

int EnvContextObservable::GetRunTimeRange() const {
	lock_guard<recursive_mutex> lock(_mutex);
	if (!_envContext->pDeltaArray) {
		return 1;
	}

	return _envContext->endYear - _envContext->startYear;
}
