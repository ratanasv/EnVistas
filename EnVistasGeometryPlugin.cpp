#include "stdafx.h"
#include "EnVistasGeometryPlugin.h"
#include <stdexcept>
#include <EnvExtension.h>
#include <Maplayer.h>
#include <vector>

using std::vector;
using std::logic_error;
using std::runtime_error;
using std::map;
using std::shared_ptr;

// this is the most ridiculous build error...
wxEvtHandler::wxEvtHandler(const wxEvtHandler&) { }

static const VI_String METHOD_FOR_VISTAS_ERROR("This class is for interfacing with \
												ENVISION. Most method are not \
												implemented; they are for VISTAS.");

static const VI_String NOT_YET_IMPLEMENTED_ERROR("Not implemented yet. Talk to Vir\
												 about this.");

template <class T> void deleteArray(void* data) {
	delete[] (T*)data;
}

VI_String EnVistasGeometryPlugin::GetFactoryRegistryName() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_PluginType EnVistasGeometryPlugin::GetType() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_String EnVistasGeometryPlugin::GetName() {
	throw METHOD_FOR_VISTAS_ERROR;
}

void EnVistasGeometryPlugin::Set( const VI_Path& path ) {
	throw NOT_YET_IMPLEMENTED_ERROR;
}

VI_DataType EnVistasGeometryPlugin::GetDataType() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_String EnVistasGeometryPlugin::GetDataName() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_Path EnVistasGeometryPlugin::GetPath() {
	char pathBuffer[128];
	lstrcpy(pathBuffer, mapLayer->m_path);
	VI_Path result(pathBuffer);
	return result.GetDirectory();
}

long EnVistasGeometryPlugin::GetDiskSize() {
	throw METHOD_FOR_VISTAS_ERROR;
}

long EnVistasGeometryPlugin::GetMemorySize() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_SpatialInfo EnVistasGeometryPlugin::GetSpatialInfo() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_TemporalInfo EnVistasGeometryPlugin::GetTemporalInfo() {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_StringList EnVistasGeometryPlugin::GetAttributes() {
	const int numColumns = mapLayer->GetColCount();
	list<VI_String> attributes;
	for (int i=0; i<numColumns; i++) {
		attributes.push_back(VI_String(mapLayer->GetFieldLabel(i)));
	}
	return attributes;
}

VI_String EnVistasGeometryPlugin::GetAttributeLabel( const VI_String& attribute ) {
	return attribute;
}

VI_AttributeStats EnVistasGeometryPlugin::GetAttributeStats( const VI_String& attribute ) {
	throw METHOD_FOR_VISTAS_ERROR;
}

bool EnVistasGeometryPlugin::HasStats() {
	throw METHOD_FOR_VISTAS_ERROR;
}

void EnVistasGeometryPlugin::CalculateStatistics() {
	throw METHOD_FOR_VISTAS_ERROR;
}

bool EnVistasGeometryPlugin::ValuesAreTemporal() {
	throw METHOD_FOR_VISTAS_ERROR;
}

int EnVistasGeometryPlugin::GetNumShapes() {
	return mapLayer->m_pPolyArray->GetCount();
}

VI_ShapeArrayRef EnVistasGeometryPlugin::GetShapeArray() {
	auto polyArray = mapLayer->m_pPolyArray;
	unsigned numShapes = polyArray->GetCount();
	shared_ptr<vector<VI_Shape>> shapeArray(new vector<VI_Shape>(numShapes));
	for (unsigned i=0; i<numShapes; i++) {
		auto shape = polyArray->ElementAt(i);
		CDWordArray& parts = shape->m_vertexPartArray;
		auto numParts = parts.GetCount();
 		VertexArray& vertices = shape->m_vertexArray;
		auto numVerts = vertices.GetCount();

		shapeArray->at(i).ShapeHeader.numparts = numParts;
		shapeArray->at(i).ShapeHeader.numpoints = numVerts;
		shapeArray->at(i).ShapeHeader.xmax = shape->m_xMax;
		shapeArray->at(i).ShapeHeader.xmin = shape->m_xMin;
		shapeArray->at(i).ShapeHeader.ymin = shape->m_yMin;
		shapeArray->at(i).ShapeHeader.ymax = shape->m_yMax;
		float zmin, zmax;
		shape->GetBoundingZ(zmin, zmax);
		shapeArray->at(i).ShapeHeader.zmin = zmin;
		shapeArray->at(i).ShapeHeader.zmax = zmax;
		shapeArray->at(i).ShapeType = 15;
		for (unsigned j=0; j<numParts; j++) {
			shapeArray->at(i).Parts.push_back(parts.ElementAt(j));
		}
		shapeArray->at(i).Vertices.reset(new vector<struct point3d>(numVerts));
		for (unsigned j=0; j<numVerts; j++) {
			shapeArray->at(i).Vertices->at(j).x = vertices.ElementAt(j).x;
			shapeArray->at(i).Vertices->at(j).y = vertices.ElementAt(j).y;
			shapeArray->at(i).Vertices->at(j).z = vertices.ElementAt(j).z;
		}
	}
	VI_ShapeArrayRef shapeArrayRef(shapeArray);
	shapeArrayRef.SetShpMainHeader(GetShpMainHeader());
	return shapeArrayRef;
}

VI_Color EnVistasGeometryPlugin::ConvertToColor(const Bin& bin) const {
	const auto& colorRef = bin.m_color;
	return VI_Color((int)GetRValue(colorRef), (int)GetGValue(colorRef), 
		(int)GetBValue(colorRef));
}

map<VI_ImmutableAbstract, VI_Color> EnVistasGeometryPlugin::ObtainValueColorMap( 
	const VI_String& attribute ) 
{
	map<VI_ImmutableAbstract, VI_Color> result;
	auto mutableMapLayer = const_cast<MapLayer*>(mapLayer);
	const int numBin = mutableMapLayer->GetBinCount(USE_ACTIVE_COL);
	for (int i=0; i<numBin; i++) {
		auto bin = mapLayer->GetBin(USE_ACTIVE_COL, i);
		auto color = ConvertToColor(bin);
		result[VI_ImmutableAbstract(bin.m_minVal)] = color;
	}
	return result;
}

map<VI_ImmutableAbstract, VI_String> EnVistasGeometryPlugin::ObtainValueLabelMap( 
	const VI_String& attribute) 
{
	map<VI_ImmutableAbstract, VI_String> result;
	auto mutableMapLayer = const_cast<MapLayer*>(mapLayer);
	const int numBin = mutableMapLayer->GetBinCount(USE_ACTIVE_COL);
	for (int i=0; i<numBin; i++) {
		auto bin = mapLayer->GetBin(USE_ACTIVE_COL, i);
		result[VI_ImmutableAbstract(bin.m_minVal)] = VI_String(bin.m_label);
	}
	return result;
}

shared_ptr<vector<VI_ImmutableAbstract>> EnVistasGeometryPlugin::ObtainValues( 
	const VI_String& attribute) 
{
	shared_ptr<vector<VI_ImmutableAbstract>> result(new vector<VI_ImmutableAbstract>());
	auto mutableMapLayer = const_cast<MapLayer*>(mapLayer);
	const int numBin = mutableMapLayer->GetBinCount(USE_ACTIVE_COL);
	for (int i=0; i<numBin; i++) {
		auto bin = mapLayer->GetBin(USE_ACTIVE_COL, i);
		result->push_back(VI_ImmutableAbstract(bin.m_minVal));
	}
	return result;
}

VI_Abstract::AbstractType EnVistasGeometryPlugin::GetDataTypeFromXML( const VI_String& attribute ) {
	TYPE dataType = mapLayer->GetFieldType(USE_ACTIVE_COL);
	switch(dataType) {
	case TYPE_INT:
	case TYPE_LONG:
		return VI_Abstract::VALUE_TYPE_INT;
	case TYPE_FLOAT:
		return VI_Abstract::VALUE_TYPE_DOUBLE;
	case TYPE_CHAR:
		return VI_Abstract::VALUE_TYPE_STRING;
	default:
		throw runtime_error("Unsupported DataType");
		break;
	}
}

bool EnVistasGeometryPlugin::IsDataDiscrete( const VI_String& attribute ) {
	TYPE dataType = mapLayer->GetFieldType(USE_ACTIVE_COL);
	switch(dataType) {
	case TYPE_INT:
	case TYPE_LONG:
	case TYPE_CHAR:
		return true;
	case TYPE_FLOAT:
		return false;
	default:
		throw runtime_error("Unsupported DataType");
		break;
	}
}

VI_ShapeDataPlugin::MinMaxColorArray EnVistasGeometryPlugin::GetMinMaxColorArray( const VI_String& attribute ) {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_ShapeDataPlugin::ColorLabelArray EnVistasGeometryPlugin::ObtainColorLabelArray( const VI_String& attribute ) {
	throw METHOD_FOR_VISTAS_ERROR;
}

shared_ptr<vector<VI_ImmutableAbstract>> EnVistasGeometryPlugin::GetAttributeArray( 
	const VI_String& attribute ) 
{
	auto dataType = GetAttributeDataType(attribute);
	shared_ptr<vector<VI_ImmutableAbstract>> result(new vector<VI_ImmutableAbstract>());
	unsigned numShapes = GetNumShapes();
	switch(dataType) {
	case VI_Abstract::VALUE_TYPE_INT:
		for (int i=0; i<numShapes; i++) {
			int buffer;
			mapLayer->GetData(i, mapLayer->m_activeField, buffer);
			result->push_back(VI_ImmutableAbstract(buffer));
		}
		break;
	default:
		break;
	}
	return result;
}

VI_Abstract::AbstractType EnVistasGeometryPlugin::GetAttributeDataType( const VI_String& attribute ) {
	return GetDataTypeFromXML(attribute);
}

struct shpmainheader EnVistasGeometryPlugin::GetShpMainHeader() const {
	struct shpmainheader Returned;
	Returned.filecode = -1;
	Returned.filelength = -1;
	Returned.mmax = -1;
	Returned.mmin = -1;
	Returned.pad = -1;
	Returned.shapetype = 15;
	Returned.unused0 = -1;
	Returned.unused1 = -1;
	Returned.unused2 = -1;
	Returned.unused3 = -1;
	Returned.unused4 = -1;
	Returned.version = -1;
	float xmin, xmax, ymin, ymax, zmin, zmax;
	mapLayer->GetExtents(xmin, xmax, ymin, ymax);
	const_cast<MapLayer*>(mapLayer)->GetZExtents(zmin, zmax);
	Returned.xmin = xmin;
	Returned.ymin = ymin;
	Returned.zmin = zmin;
	Returned.xmax = xmax;
	Returned.ymax = ymax;
	Returned.zmax = zmax;
	return Returned;
}

