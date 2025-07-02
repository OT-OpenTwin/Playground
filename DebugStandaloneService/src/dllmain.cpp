//! @file dllmain.cpp
//! @author Alexander Kuester (alexk95)
//! @date March 2025
// ###########################################################################################################################################################################################################################################################################################################################

// DebugService header
#include "Application.h"

// OpenTwin header
#include "OTCore/String.h"
#include "OTCore/ThisService.h"
#include "OTCommunication/ActionDispatcher.h"
#include "OTCommunication/ServiceLogNotifier.h"

// std header
#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" {

	_declspec(dllexport) const char *performAction(const char * _json, const char * _senderIP) {
		return ot::String::getCStringCopy(ot::ActionDispatcher::instance().dispatchWrapper(_json, _senderIP, ot::EXECUTE));
	};

	_declspec(dllexport) const char *queueAction(const char * _json, const char * _senderIP) {
		return ot::String::getCStringCopy(ot::ActionDispatcher::instance().dispatchWrapper(_json, _senderIP, ot::QUEUE));
	};

	_declspec(dllexport) const char *getServiceURL(void) {
		return ot::String::getCStringCopy(Application::instance()->getServiceURL());
	}

	_declspec(dllexport) void deallocateData(const char * _data) {
		// *****************
		// This code will deallocate the memory of the return values from the perform- and queueAction calls
		if (_data != nullptr) {
			delete[] _data;
		}
		// *****************
	};

	// This function is called once upon startup of this service
	_declspec(dllexport) int init(const char * _siteID, const char * _ownIP, const char * _sessionServiceIP, const char * _sessionID) {
		using namespace ot;

		// Perform custom initialization without service foundation

		Application* app = Application::instance();
		app->setServiceURL(_ownIP);

#if defined(_DEBUG)
		ServiceLogNotifier::initialize(app->getServiceName(), "", true);
#else
		ServiceLogNotifier::initialize(app->getServiceName(), "", false);
#endif
		ThisService& ts = ThisService::instance();
		ts.setServiceName(app->getServiceName());
		ts.setServiceType(app->getServiceType());
		ts.setServiceURL(app->getServiceURL());

		return 0;
	};
}