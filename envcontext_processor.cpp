#include "stdafx.h"
#include "envcontext_processor.h"
#include "EnVistasGeometryPlugin.h"
#include <Maplayer.h>
#include "SHP3D.h"
#include <cstring>
#include <thread>
#include "SHP3DVizPlugin.h"

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
	context->pMapLayer->m_pMap->InstallNotifyHandler(SHP3DProcessor::OnHandlerCallback, 
		(LONG_PTR)this);
	_vizPlugin = LoadSHP3D(VI_Path());
	_dataPlugin.reset(new EnVistasGeometryPlugin(_envContext));
	_vizPlugin->SetDataSynchronous(_dataPlugin.get());
}

int SHP3DProcessor::OnHandlerCallback(Map* map, NOTIFY_TYPE what, int a0, LONG_PTR a1,
									  LONG_PTR extra) 
{
	if (what == NT_ACTIVEATTRIBUTECHANGED) {
		SHP3DProcessor* thisObject = (SHP3DProcessor*)extra;
		thisObject->Update(thisObject->_envContext);
	}
	return 1;
}

void SHP3DProcessor::Update(const EnvContext* envContext) {
	//envContext could change after we hit run.
	if (_envContext != envContext) {
		_envContext = envContext;
		//whoa, _dataPlugin might be in used concurrently, causing _vizPlugin
		//to produce untruthty results.
		_dataPlugin->SetEnvContext(envContext);
	}
	const int irrelevant = 0;
	_vizPlugin->UpdateDataSynchronous(_dataPlugin.get());
}
