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

static const string shp3dRegisterName("vip_core_shpviewer");
#ifdef _DEBUG
static const string shp3dPath("E:/Vault/envision_source/x64/Debug/shp3d.dll");
#else
static const string shp3dPath("E:/Vault/envision_source/x64/Release/shp3d.dll");
#endif

static shared_ptr<SHP3D> LoadSHP3DDLL(const VI_Path& path) {
	if (!gPluginMgr->LoadPlugin(path)) {
		return shared_ptr<SHP3D>();
	}
	auto plugin = gPluginMgr->GetPluginInstance(shp3dRegisterName);
	auto shp3dPlugin = dynamic_cast<SHP3D*>(plugin);
	if (!shp3dPlugin) {
		return shared_ptr<SHP3D>();
	}
	return shared_ptr<SHP3D>(shp3dPlugin);
}

SHP3DProcessor::SHP3DProcessor(const EnvContext* context) : 
	_envContext(context)
{
	context->pMapLayer->m_pMap->InstallNotifyHandler(SHP3DProcessor::OnHandlerCallback, 
		(LONG_PTR)this);
	VI_Path path(shp3dPath);
	assert(path.Exists());
	_vizPlugin = LoadSHP3DDLL(path);
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
