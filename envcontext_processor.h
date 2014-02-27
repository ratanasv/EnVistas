#pragma once

#include <memory>
#include <EnvExtension.h>
#include <vistas/vistas.h>
#include <MAP.h>

using std::shared_ptr;

class VI_Scene;
class VI_ShapeDataPlugin;
class VI_VizPlugin3D;
class EnVistasGeometryPlugin;

class SHP3DProcessor {
private:
	const EnvContext* _envContext;
	VI_Scene _scene;
	shared_ptr<VI_VizPlugin3D> _vizPlugin;
	shared_ptr<EnVistasGeometryPlugin> _dataPlugin;
public:
	SHP3DProcessor(const EnvContext* context, VI_Scene& scene);
	void operator()();
	static int OnHandlerCallback(Map*, NOTIFY_TYPE, int, LONG_PTR, LONG_PTR ); 
	void Update(const EnvContext* envContext);
};