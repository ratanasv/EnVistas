#pragma once

#include <memory>
#include <EnvExtension.h>
#include <vistas/vistas.h>
#include <MAP.h>

using std::shared_ptr;

class VI_ShapeDataPlugin;
class SHP3D;
class EnVistasGeometryPlugin;
class VI_Scene;

class SHP3DProcessor {
private:
	const EnvContext* _envContext;
	shared_ptr<SHP3D> _vizPlugin;
	shared_ptr<EnVistasGeometryPlugin> _dataPlugin;
public:
	SHP3DProcessor(const EnvContext* context);
	static int OnHandlerCallback(Map*, NOTIFY_TYPE, int, LONG_PTR, LONG_PTR ); 
	void Update(const EnvContext* envContext);
	void AddObjectToScene(VI_Scene& scene);

	friend class EnVistas;
};