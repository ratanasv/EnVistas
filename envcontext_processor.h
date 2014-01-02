#pragma once

#include <memory>
#include <EnvExtension.h>
#include <vistas/vistas.h>

using std::shared_ptr;

class VI_Scene;
class VI_ShapeDataPlugin;
class VI_VizPlugin3D;

class ENVContextProcessor {
public:
	virtual ~ENVContextProcessor() {};
};

class SHP3DProcessor : public ENVContextProcessor {
private:
	const EnvContext* _envContext;
	VI_Scene _scene;
	shared_ptr<VI_ShapeDataPlugin> _dataPlugin;
	shared_ptr<VI_VizPlugin3D> _vizPlugin;
	int _lastActiveField;
public:
	SHP3DProcessor(const EnvContext* context, VI_Scene& scene);
	void operator()();
};