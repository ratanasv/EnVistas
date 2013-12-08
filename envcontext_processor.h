#pragma once

#include <memory>
#include <EnvExtension.h>

using std::shared_ptr;

class VI_Scene;
class VI_ShapeDataPlugin;
class VI_VizPlugin3D;

class ENVContextProcessor {
public:
	virtual ~ENVContextProcessor() {};
	virtual bool DoesNeedUpdate(const EnvContext* context) = 0;
	virtual void UpdateScene(VI_Scene& scene) = 0;
};

class SHP3DProcessor : public ENVContextProcessor {
private:
	const EnvContext* _envContext;
	shared_ptr<VI_ShapeDataPlugin> _dataPlugin;
	shared_ptr<VI_VizPlugin3D> _vizPlugin;
public:
	explicit SHP3DProcessor(const EnvContext* context);
	virtual bool DoesNeedUpdate(const EnvContext* context);
	virtual void UpdateScene( VI_Scene& scene );
};