// Add custom code header

#pragma once

// OpenTwin header
#include "OTCore/Logger.h"
#include "OTCore/OTClassHelper.h"

// Qt header
#include <QtCore/qobject.h>

// Forward declaractions
class MainWindow;

// #########################################################################################################################################

class TestCode : public QObject {
	Q_OBJECT
	OT_DECL_NOCOPY(TestCode)
public:

	//! \brief Constructor.
	//! \warning Only variable initialization is allowed.
	TestCode();

	// #########################################################################################################################################

	//! \brief Slot will be called when the main window is initialized and is ready to initialize the test code.
	//! \param _logView The log view may be used as a parent when adding custom widget views.
	void initialize(MainWindow* _window);

private:

	MainWindow* m_mainWindow; //! \brief Main application window.

	// #########################################################################################################################################

	// v--- Add custom methods and members below ---v



};

// Custom methods
