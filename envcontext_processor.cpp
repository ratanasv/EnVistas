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

static shared_ptr<VI_VizPlugin3D> LoadSHP3DDLL(const VI_Path& path) {
	if (!gPluginMgr->LoadPlugin(path)) {
		return shared_ptr<VI_VizPlugin3D>();
	}
	auto plugin = gPluginMgr->GetPluginInstance(shp3dRegisterName);
	auto shp3dPlugin = dynamic_cast<SHP3D*>(plugin);
	if (!shp3dPlugin) {
		return shared_ptr<VI_VizPlugin3D>();
	}
	return shared_ptr<VI_VizPlugin3D>(shp3dPlugin);
}

SHP3DProcessor::SHP3DProcessor(const EnvContext* context, VI_Scene& scene) : 
	_envContext(context), _scene(scene), _lastActiveField(-1)
{

	_dataPlugin.reset(new EnVistasGeometryPlugin(context->pMapLayer));
	VI_Path path(shp3dPath);
	assert(path.Exists());
	_vizPlugin = LoadSHP3DDLL(path);
	thread task(*this);
	task.detach();
}

void SHP3DProcessor::operator()() {
	while (true) {
		if (_envContext->pMapLayer->m_activeField != _lastActiveField) {
			_lastActiveField = _envContext->pMapLayer->m_activeField;
			const int irrelevant = 0;
			_vizPlugin->SetData(_dataPlugin.get(), irrelevant);
			_scene.RemoveAllObjects();
			_vizPlugin->SetScene(_scene);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
