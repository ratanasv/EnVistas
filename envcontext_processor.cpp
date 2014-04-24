#include "stdafx.h"
#include "envcontext_processor.h"
#include "EnVistasGeometryPlugin.h"
#include <Maplayer.h>
#include "SHP3D.h"
#include <cstring>
#include <thread>
#include "SHP3DVizPlugin.h"
#include "EnvContextObservable.h"

using std::string;
using std::thread;

static string GetCurrentPath() {
	TCHAR currentDir[MAX_PATH] = "";
	if (!::GetCurrentDirectory(sizeof(currentDir)-1, currentDir)) {
		throw runtime_error("cannot invoke ::GetCurrentDirectory");
	}

	return string(currentDir);
}

static shared_ptr<SHP3D> LoadSHP3D(const VI_Path& path) {
	return shared_ptr<SHP3D>(new SHP3D());
}

SHP3DProcessor::SHP3DProcessor(const EnvContext* context) : 
	_envContext(context)
{
	_vizPlugin = LoadSHP3D(VI_Path());
	_dataPlugin.reset(new EnVistasGeometryPlugin(_envContext));
	_vizPlugin->SetDataSynchronous(_dataPlugin.get());
	_activeColumn = _envContext->pMapLayer->m_activeField;
	_currentYear = _envContext->currentYear;
	EnvContextObservable::INSTANCE.AddObserver(this);
}


SHP3DProcessor::~SHP3DProcessor() {
	EnvContextObservable::INSTANCE.DeleteObserver(this);
}


void SHP3DProcessor::OnActiveColumnChanged() {
	_vizPlugin->UpdateSynchronous();
}

void SHP3DProcessor::OnCurrentYearChanged() {
	_vizPlugin->DeltaUpdateSynchronous();
}

void SHP3DProcessor::Update(const VI_Observable* const observable) {
	auto context = dynamic_cast<const EnvContextObservable*>(observable);
	if (!context) {
		return;
	}

	if (_envContext != context->GetEnvContext()) {
		_envContext = context->GetEnvContext();
		_dataPlugin->SetEnvContext(_envContext);
	}

	if (_activeColumn != context->GetActiveColumn()) {
		_activeColumn = context->GetActiveColumn();
		OnActiveColumnChanged();
	}

	if (_currentYear != context->GetCurrentYear()) {
		_currentYear = context->GetCurrentYear();
		OnCurrentYearChanged();
	}
}
