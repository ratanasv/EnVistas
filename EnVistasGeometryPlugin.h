#pragma once

#include <vistas/vistas.h>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

class MapLayer;
class Bin;
class EnvContext;
class DELTA;
class EnvContextObservable;
////////////////////////////////////
// shape provider for Vistas engine
////////////////////////////////////
class EnVistasGeometryPlugin : public VI_ShapeDeltaDataPlugin, public VI_Observer {
public:
	EnVistasGeometryPlugin(std::shared_ptr<EnvContextObservable>& observable);
	virtual ~EnVistasGeometryPlugin();

	/* VI_PluginBase Methods. */
	virtual VI_String		GetFactoryRegistryName();
	virtual	VI_PluginType	GetType();
	virtual VI_String		GetName();

	/* VI_DataPlugin Methods. */
	virtual void				Set(const VI_Path& path);
	virtual VI_DataType			GetDataType();
	virtual VI_String			GetDataName();
	virtual VI_Path				GetPath();
	virtual long				GetDiskSize();
	virtual long				GetMemorySize();
	virtual VI_SpatialInfo		GetSpatialInfo();
	virtual VI_TemporalInfo		GetTemporalInfo();
	virtual VI_StringList		GetAttributes();
	virtual VI_String			GetAttributeLabel(const VI_String& attribute);
	virtual VI_AttributeStats	GetAttributeStats(const VI_String& attribute);
	virtual bool				HasStats();
	virtual void				CalculateStatistics();

	/* VI_ShapeArrayDataPlugin Methods. */
	virtual bool		ValuesAreTemporal();
	virtual int			GetNumShapes();

	

	virtual VI_ShapeArray	GetShapeArray();

	virtual std::map<VI_ImmutableAbstract, VI_Color>
		ObtainValueColorMap(const VI_String& attribute);
	virtual std::map<VI_ImmutableAbstract, VI_String>
		ObtainValueLabelMap(const VI_String& attribute);
	virtual std::shared_ptr<std::vector<VI_ImmutableAbstract> >
		ObtainValues(const VI_String& attribute);
	virtual VI_Abstract::AbstractType 
		GetDataTypeFromXML(const VI_String& attribute);

	

	virtual bool IsDataDiscrete(const VI_String& attribute);
	virtual MinMaxColorArray  GetMinMaxColorArray(
		const VI_String& attribute);
	virtual ColorLabelArray ObtainColorLabelArray(const VI_String& attribute);

	virtual shared_ptr<vector<VI_ImmutableAbstract>> GetAttributeArray( const VI_String& attribute );

	virtual VI_Abstract::AbstractType GetAttributeDataType( const VI_String& attribute );

	virtual std::shared_ptr<const std::vector<VI_ShapeDelta>> GetDeltaArray() const;

	virtual void Update(const VI_Observable* const observable);

private:
	VI_Color ConvertToColor(const Bin& bin) const;
	struct shpmainheader GetShpMainHeader() const;
	struct shpmainheader GetShapeExtents() const;
	VI_Abstract::AbstractType GetDataTypeColumn(int column) const;
	int GetPolyArraySize();
	bool CheckTypeVDataAndMapLayer(const DELTA& delta) const;
	bool IsActiveColumn(const DELTA& delta) const;
	bool IsCurrentYear(const DELTA& delta) const;

private:
	std::shared_ptr<EnvContextObservable> _observable;
	mutable boost::shared_mutex _readWriteMutex;
	const EnvContext* _envContext;
	int _currentYear;
	VI_ShapeArray _shapeArray;
};