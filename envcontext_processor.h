#pragma once

#include <memory>
#include <EnvExtension.h>
#include <vistas/vistas.h>
#include <MAP.h>
#include <vistas/Observer.h>

using std::shared_ptr;

class VI_ShapeDataPlugin;
class SHP3D;
class EnVistasGeometryPlugin;
class VI_Scene;

class SHP3DProcessor : public VI_Observer {
private:
	const EnvContext* _envContext;
	shared_ptr<SHP3D> _vizPlugin;
	shared_ptr<EnVistasGeometryPlugin> _dataPlugin;
	int _activeColumn;
	int _currentYear;
	SHP3DProcessor(const SHP3DProcessor& rhs);
public:
	SHP3DProcessor(const EnvContext* context);
	virtual ~SHP3DProcessor();
	void AddObjectToScene(VI_Scene& scene);
	//the only way new visualization gets rendered is via this method.
	virtual void Update(const VI_Observable* const observable);

private:
	void OnActiveColumnChanged();
	void OnCurrentYearChanged();

	friend class EnVistas;
};