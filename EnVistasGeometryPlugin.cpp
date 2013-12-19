#include "stdafx.h"
#include "EnVistasGeometryPlugin.h"
#include <stdexcept>
#include <EnvExtension.h>
#include <Maplayer.h>
#include <vector>

using std::vector;
using std::logic_error;
using std::map;

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
	throw NOT_YET_IMPLEMENTED_ERROR;
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
	vector<VI_Shape> shapeArray(numShapes);
	for (unsigned i=0; i<numShapes; i++) {
		auto shape = polyArray->ElementAt(i);
		CDWordArray& parts = shape->m_vertexPartArray;
		auto numParts = parts.GetCount();
 		VertexArray& vertices = shape->m_vertexArray;
		auto numVerts = vertices.GetCount();

		shapeArray[i].ShapeHeader.numparts = numParts;
		shapeArray[i].ShapeHeader.numpoints = numVerts;
		shapeArray[i].ShapeHeader.xmax = shape->m_xMax;
		shapeArray[i].ShapeHeader.xmin = shape->m_xMin;
		shapeArray[i].ShapeHeader.ymin = shape->m_yMin;
		shapeArray[i].ShapeHeader.ymax = shape->m_yMax;
		shapeArray[i].ShapeHeader.zmin = -1.0;
		shapeArray[i].ShapeHeader.zmax = 1.0;
		shapeArray[i].ShapeType = 15;
		vector<int> partArray(numParts);
		for (unsigned j=0; j<numParts; j++) {
			partArray[j] = parts.ElementAt(j);
		}
		shapeArray[i].Parts = VI_DataRefBase<int>(partArray.data(), sizeof(int)*numParts, true);
		vector<struct point3d> vertArray(numVerts);
		for (unsigned j=0; j<numVerts; j++) {
			vertArray[j].x = vertices.ElementAt(j).x;
			vertArray[j].y = vertices.ElementAt(j).y;
			vertArray[j].z = vertices.ElementAt(j).z;
		}
		shapeArray[i].Vertices = VI_DataRefBase<struct point3d>(vertArray.data(), 
			sizeof(struct point3d)*numVerts, true);
	}
	return VI_DataRefBase<VI_Shape>(shapeArray.data(), sizeof(VI_Shape)*numShapes, true);
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

std::map<VI_ImmutableAbstract, VI_String> EnVistasGeometryPlugin::ObtainValueLabelMap( const VI_String& attribute ) {
	throw NOT_YET_IMPLEMENTED_ERROR;
}

std::shared_ptr<std::vector<VI_ImmutableAbstract> > EnVistasGeometryPlugin::ObtainValues( const VI_String& attribute ) {
	throw NOT_YET_IMPLEMENTED_ERROR;
}

VI_Abstract::AbstractType EnVistasGeometryPlugin::GetDataTypeFromXML( const VI_String& attribute ) {
	throw NOT_YET_IMPLEMENTED_ERROR;
}

bool EnVistasGeometryPlugin::IsDataDiscrete( const VI_String& attribute ) {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_ShapeDataPlugin::MinMaxColorArray EnVistasGeometryPlugin::GetMinMaxColorArray( const VI_String& attribute ) {
	throw METHOD_FOR_VISTAS_ERROR;
}

VI_ShapeDataPlugin::ColorLabelArray EnVistasGeometryPlugin::ObtainColorLabelArray( const VI_String& attribute ) {
	throw METHOD_FOR_VISTAS_ERROR;
}

shared_ptr<vector<VI_ImmutableAbstract>> EnVistasGeometryPlugin::GetAttributeArray( const VI_String& attribute ) {
	throw std::exception("The method or operation is not implemented.");
}

VI_Abstract::AbstractType EnVistasGeometryPlugin::GetAttributeDataType( const VI_String& attribute ) {
	throw std::exception("The method or operation is not implemented.");
}

