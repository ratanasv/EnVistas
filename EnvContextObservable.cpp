#include "stdafx.h"
#include "EnvContextObservable.h"

EnvContextObservable EnvContextObservable::INSTANCE;

EnvContextObservable::EnvContextObservable() {

}

int EnvContextObservable::GetCurrentYear() const {
	return _currentYear;
}

int EnvContextObservable::GetActiveColumn() const {
	return _activeColumn;
}

EnvContext* EnvContextObservable::GetEnvContext() const {
	return _envContext;
}

void EnvContextObservable::SetCurrentYear(int year) {
	_currentYear = year;
}

void EnvContextObservable::SetActiveColumn(int column) {
	_activeColumn = column;
}

void EnvContextObservable::SetEnvContext(EnvContext* context) {
	_envContext->pMapLayer->m_pMap->RemoveNotifyHandler(
		EnvContextObservable::OnHandlerCallback, (LONG_PTR)this);
	_envContext = context;
	_envContext->pMapLayer->m_pMap->InstallNotifyHandler(
		EnvContextObservable::OnHandlerCallback, (LONG_PTR)this);
}

int EnvContextObservable::OnHandlerCallback(
	Map* map, NOTIFY_TYPE what, int a0, LONG_PTR a1,LONG_PTR extra) 
{
	if (what == NT_ACTIVEATTRIBUTECHANGED) {
		EnvContextObservable* thisObject = (EnvContextObservable*)extra;
		thisObject->SetActiveColumn(thisObject->_envContext->pMapLayer->m_activeField);
	}
	return 1;
}
