//! @file Application.cpp
//! @author Alexander Kuester (alexk95)
//! @date March 2025
// ###########################################################################################################################################################################################################################################################################################################################

// DebugService header
#include "Application.h"

// OpenTwin header
#include "OTCore/RuntimeTests.h"
#include "OTCore/ReturnMessage.h"
#include "OTCore/ThisComputerInfo.h"
#include "OTGui/TableCfg.h"
#include "OTServiceFoundation/UiComponent.h"
#include "OTServiceFoundation/ModelComponent.h"
#include "OTServiceFoundation/AbstractUiNotifier.h"
#include "OTServiceFoundation/AbstractModelNotifier.h"
#include "OTCommunication/Msg.h"
#include "OTCommunication/ActionTypes.h"
#include "ResultCollectionExtender.h"
#include "PlotBuilder.h"
#include "QuantityDescriptionCurve.h"
#include "OTCore/FolderNames.h"
#include "OTGui/PainterRainbowIterator.h"
#include "TemplateDefaultManager.h"
// std header
#include <thread>

#define OT_DEBUG_SERVICE_PAGE_NAME "Debug"

std::string Application::handleKill(ot::JsonDocument& _doc) {
	exit(0);
}

// ###########################################################################################################################################################################################################################################################################################################################

Application::Application() :
	ot::ApplicationBase(OT_INFO_SERVICE_TYPE_DebugService "Tester", OT_INFO_SERVICE_TYPE_DebugService "Tester", new ot::AbstractUiNotifier(), new ot::AbstractModelNotifier())
{
	
}

// ###########################################################################################################################################################################################################################################################################################################################

// ###########################################################################################################################################################################################################################################################################################################################

// ###########################################################################################################################################################################################################################################################################################################################

// Default methods

Application* g_instance{ nullptr };

Application * Application::instance(void) {
	if (g_instance == nullptr) { g_instance = new Application; }
	return g_instance;
}

void Application::deleteInstance(void) {
	if (g_instance) { delete g_instance; }
	g_instance = nullptr;
}

Application::~Application()
{

}

// ###########################################################################################################################################################################################################################################################################################################################

// Required functions

void Application::run(void) {
	
}

std::string Application::processAction(const std::string & _action, ot::JsonDocument & _doc)
{
	return ""; // Return empty string if the request does not expect a return
}

std::string Application::processMessage(ServiceBase * _sender, const std::string & _message, ot::JsonDocument & _doc)
{
	return ""; // Return empty string if the request does not expect a return
}

void Application::uiConnected(ot::components::UiComponent * _ui)
{
	
}

void Application::uiDisconnected(const ot::components::UiComponent * _ui)
{

}

void Application::modelConnected(ot::components::ModelComponent * _model)
{

}

void Application::modelDisconnected(const ot::components::ModelComponent * _model)
{

}

void Application::serviceConnected(ot::ServiceBase * _service)
{

}

void Application::serviceDisconnected(const ot::ServiceBase * _service)
{

}

void Application::preShutdown(void) {

}

void Application::shuttingDown(void)
{

}

bool Application::startAsRelayService(void) const
{
	return false;	// Do not want the service to start a relay service. Otherwise change to true
}

ot::PropertyGridCfg Application::createSettings(void) const {
	return ot::PropertyGridCfg();
}

void Application::settingsSynchronized(const ot::PropertyGridCfg& _dataset) {

}

bool Application::settingChanged(const ot::Property * _item) {
	return false;
}

// ###########################################################################################################################################################################################################################################################################################################################
