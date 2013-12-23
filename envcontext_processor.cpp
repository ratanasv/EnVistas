#include "stdafx.h"
#include "envcontext_processor.h"
#include "EnVistasGeometryPlugin.h"
#include "SHP3D.h"
#include <cstring>

using std::string;

static const string shp3dRegisterName("vip_core_shpviewer");
static const string shp3dPath("E:/Vault/envision_source/x64/Debug/shp3d.dll");

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

SHP3DProcessor::SHP3DProcessor(const EnvContext* context) : _envContext(context) {
	_dataPlugin.reset(new EnVistasGeometryPlugin(context->pMapLayer));
	VI_Path path(shp3dPath);
	assert(path.Exists());
	_vizPlugin = LoadSHP3DDLL(path);
	const int irrelevant = 0;
	_vizPlugin->SetData(_dataPlugin.get(), irrelevant);
}


bool SHP3DProcessor::DoesNeedUpdate(const EnvContext* context) {
	if (_envContext == context) {
		return false;
	} else {
		return true;
	}
}

void SHP3DProcessor::UpdateScene(VI_Scene& scene) {
	scene.RemoveAllObjects();
	_vizPlugin->SetScene(scene);
}

