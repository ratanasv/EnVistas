#include "stdafx.h"
#include "EnVistasGeometryPlugin.h"
#include <stdexcept>

// this is the most ridiculous build error...
wxEvtHandler::wxEvtHandler(const wxEvtHandler&) {

}

using std::logic_error;

static const VI_String METHOD_FOR_VISTAS_ERROR("This class is for interfacing with \
												ENVISION. Most method are not \
												implemented; they are for VISTAS.");

static const VI_String NOT_YET_IMPLEMENTED_ERROR("Not implemented yet. Talk to Vir\
												 about this.");

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
	throw NOT_YET_IMPLEMENTED_ERROR;
}

VI_String EnVistasGeometryPlugin::GetAttributeLabel( const VI_String& attribute ) {
	throw NOT_YET_IMPLEMENTED_ERROR;
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
	throw NOT_YET_IMPLEMENTED_ERROR;
}

VI_ShapeArrayRef EnVistasGeometryPlugin::GetShapeArray() {
	throw NOT_YET_IMPLEMENTED_ERROR;
}

std::map<VI_ImmutableAbstract, VI_Color> EnVistasGeometryPlugin::ObtainValueColorMap( const VI_String& attribute ) {
	throw NOT_YET_IMPLEMENTED_ERROR;
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

