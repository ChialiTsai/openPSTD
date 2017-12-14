#include "window.h"

/**
 * Constructor.
 * Initializes the main window.
 * 
 * @param parent  A pointer to the parent widget
 */
Window::Window(QWidget* parent) : QMainWindow(parent), ui(new Ui::Window) {
    // Setup basic UI
    ui->setupUi(this);
    
    // Add a spacer to the main toolbar
    spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->mainToolBar->addWidget(spacer);
    
    // Create a new EventListener instance
    eventlistener = new EventListener();
    
    // Create a GraphicsView for the main frame
    sbZoom = new QSpinBox();
    view = new GraphicsView(this, eventlistener);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->horizontalLayout->addWidget(view);
    
    // Set initial model variables
    Model::getInstance()->showFPS = ui->actionFPS_counter->isChecked();
    
    // Add a label for the zoom level spinbox
    lZoom = new QLabel();
    lZoom->setText("Zoom level:");
    lZoom->setToolTip("In pixels / m");
    ui->mainToolBar->addWidget(lZoom);
    
    // Create a QSpinBox for the zoom level
    sbZoom->setMinimum(1);
    sbZoom->setMaximum(100);
    sbZoom->setValue(5);
    sbZoom->setToolTip("In pixels / m");
    Model::getInstance()->zoomlevel = 5;
    ui->mainToolBar->addWidget(sbZoom);
    connect(sbZoom, SIGNAL(valueChanged(int)), this, SLOT(slot_zoom(int)));
    
    QWidget* w = new QWidget();
    w->setFixedWidth(20);
    ui->mainToolBar->addWidget(w);
    
    // Add a label for the grid size spinbox
    lGridSize = new QLabel();
    lGridSize->setText("Grid size:");
    lGridSize->setToolTip("In m");
    ui->mainToolBar->addWidget(lGridSize);
    
    // Create a QSpinBox for the grid size
    Settings* settings = Settings::getInstance();
    sbGridSize = new QSpinBox();
    sbGridSize->setMinimum(settings->pstdGridSize);
    sbGridSize->setMaximum(10*settings->pstdGridSize);
    sbGridSize->setSingleStep(settings->pstdGridSize);
    sbGridSize->setValue(settings->pstdGridSize);
    sbGridSize->setToolTip("In m");
    ui->mainToolBar->addWidget(sbGridSize);
    connect(sbGridSize, SIGNAL(valueChanged(int)), this, SLOT(slot_gridsize(int)));
    
    // Set initial scene offset
    Model::getInstance()->offset = QPoint(0, 0);
    
    // Set action icons in settings menu
    ui->actionGrid_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->gridColor)));
    ui->actionBackground_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->backgroundColor)));
    ui->actionZoom_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->zoomRefColor)));
    ui->actionFPS_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->fpsColor)));
    ui->actionSource_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->sourceColor)));
    ui->actionReceiver_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->receiverColor)));
    
    // Center main window on screen
    QDesktopWidget* desktop = QApplication::desktop();
    QRect screen = desktop->availableGeometry(desktop->primaryScreen());
    int x = screen.x() + (screen.width() - 1024) / 2;
    int y = screen.y() + (screen.height() - 768) / 2;
    this->move(x, y);
    
    // Create a QActionGroup for the toolbar buttons
    qagMainToolbar = new QActionGroup(parent);
    qagMainToolbar->addAction(ui->actionSelect);
    qagMainToolbar->addAction(ui->actionMoveScene);
    qagMainToolbar->addAction(ui->actionAddDomain);
    qagMainToolbar->addAction(ui->actionAddSource);
    qagMainToolbar->addAction(ui->actionAddReceiver);
    qagMainToolbar->addAction(ui->actionMeasure);
    qagMainToolbar->setExclusive(true);
    
    // Connect actions in settings menu
    connect(ui->actionGrid_color, SIGNAL(triggered(bool)), this, SLOT(slot_gridcolor()));
    connect(ui->actionBackground_color, SIGNAL(triggered(bool)), this, SLOT(slot_bgcolor()));
    connect(ui->actionZoom_color, SIGNAL(triggered(bool)), this, SLOT(slot_zoomcolor()));
    connect(ui->actionFPS_color, SIGNAL(triggered(bool)), this, SLOT(slot_fpscolor()));
    connect(ui->actionSource_color, SIGNAL(triggered(bool)), this, SLOT(slot_sourcecolor()));
    connect(ui->actionReceiver_color, SIGNAL(triggered(bool)), this, SLOT(slot_receivercolor()));
    connect(ui->actionClamp_distance, SIGNAL(triggered(bool)), this, SLOT(slot_clampdist()));
    connect(ui->actionPSTD_grid_size, SIGNAL(triggered(bool)), this, SLOT(slot_pstdgridsize()));
    
    // Connect actions in scene menu
    connect(ui->actionClear_all, SIGNAL(triggered(bool)), this, SLOT(slot_clearscene()));
    connect(ui->actionDelete_selected, SIGNAL(triggered(bool)), this, SLOT(slot_deleteselected()));
    
    // Connect actions in view menu
    connect(ui->actionFPS_counter, SIGNAL(triggered(bool)), this, SLOT(slot_fpscounter()));
    connect(ui->actionGrid, SIGNAL(triggered(bool)), this, SLOT(slot_grid()));
    
    // Connect actions in operate menu
    connect(ui->actionSelect, SIGNAL(triggered(bool)), this, SLOT(slot_select()));
    connect(ui->actionMoveScene, SIGNAL(triggered(bool)), this, SLOT(slot_move()));
    connect(ui->actionAddDomain, SIGNAL(triggered(bool)), this, SLOT(slot_adddomain()));
    connect(ui->actionAddSource, SIGNAL(triggered(bool)), this, SLOT(slot_addsource()));
    connect(ui->actionAddReceiver, SIGNAL(triggered(bool)), this, SLOT(slot_addreceiver()));
    connect(ui->actionMeasure, SIGNAL(triggered(bool)), this, SLOT(slot_measure()));
    connect(ui->actionUndo, SIGNAL(triggered(bool)), this, SLOT(slot_undo()));
    connect(ui->actionRedo, SIGNAL(triggered(bool)), this, SLOT(slot_redo()));
    connect(ui->actionMoveToCenter, SIGNAL(triggered(bool)), this, SLOT(slot_movetocenter()));
    
    // Connect actions in the about menu
    connect(ui->actionChangelog, SIGNAL(triggered(bool)), this, SLOT(slot_changelog()));
}

/**
 * Destructor.
 */
Window::~Window() {
    // Delete class instance variables
    delete eventlistener;
    delete qagMainToolbar;
    delete lGridSize;
    delete lZoom;
    delete view;
    delete sbZoom;
    delete spacer;
    delete ui;
}

/**
 * PaintEvent method, called when the window needs to be repainted.
 * 
 * @param event  A reference to the QPaintEvent
 */
void Window::paintEvent(QPaintEvent* event) {
    QMainWindow::paintEvent(event);
    renderer->draw();
}

/**
 * Callback method for the grid color action in the settings menu.
 * Opens a color picker dialog and saves the new color.
 */
void Window::slot_gridcolor() {
    QColor color = QColorDialog::getColor(Settings::getInstance()->gridColor, this, "Choose a grid color");
    if (color.isValid()) settings->gridColor = color.rgb();
    ui->actionGrid_color->setIcon(QIcon(color2pixmap(settings->gridColor)));
}

/**
 * Callback method for the background color action in the settings menu.
 * Opens a color picker dialog and saves the new color.
 */
void Window::slot_bgcolor() {
    QColor color = QColorDialog::getColor(Settings::getInstance()->backgroundColor, this, "Choose a background color");
    if (color.isValid()) Settings::getInstance()->backgroundColor = color.rgb();
    ui->actionBackground_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->backgroundColor)));
}

/**
 * Callback method for the zoom color action in the settings menu.
 * Opens a color picker dialog and saves the new color.
 */
void Window::slot_zoomcolor() {
    QColor color = QColorDialog::getColor(Settings::getInstance()->zoomRefColor, this, "Choose a zoom color");
    if (color.isValid()) Settings::getInstance()->zoomRefColor = color.rgb();
    ui->actionZoom_color->setIcon(QIcon(color2pixmap(Settings::getInstance()->zoomRefColor)));
}

/**
 * Callback method for the fps color action in the settings menu.
 * Opens a color picker dialog and saves the new color.
 */
void Window::slot_fpscolor() {
    QColor color = QColorDialog::getColor(settings->fpsColor, this, "Choose an FPS color");
    if (color.isValid()) settings->fpsColor = color.rgb();
    ui->actionFPS_color->setIcon(QIcon(color2pixmap(settings->fpsColor)));
}

/**
 * Callback method for the source color action in the settings menu.
 * Opens a color picker dialog and saves the new color.
 */
void Window::slot_sourcecolor() {
    QColor color = QColorDialog::getColor(settings->sourceColor, this, "Choose a source color");
    if (color.isValid()) settings->sourceColor = color.rgb();
    ui->actionSource_color->setIcon(QIcon(color2pixmap(settings->sourceColor)));
}

/**
 * Callback method for the receiver color action in the settings menu.
 * Opens a color picker dialog and saves the new color.
 */
void Window::slot_receivercolor() {
    QColor color = QColorDialog::getColor(settings->receiverColor, this, "Choose a receiver color");
    if (color.isValid()) settings->receiverColor = color.rgb();
    ui->actionReceiver_color->setIcon(QIcon(color2pixmap(settings->receiverColor)));
}

/**
 * Callback method for the clamp distance action in the settings menu.
 * Opens an input dialog and saves the new clamp distance.
 */
void Window::slot_clampdist() {
    int clampdist = QInputDialog::getInt(
        this,
        "Choose a clamp distance",
        "Choose a clamp distance\nSet to zero to disable clamping",
        settings->clampDist
    );
    settings->clampDist = clampdist;
}

/**
 * Callback method for the pstd grid size action in the settings menu.
 * Opens an input dialog and saves the new pstd grid size.
 */
void Window::slot_pstdgridsize() {
    int pstdgridsize = QInputDialog::getInt(
        this,
        "Choose a PSTD grid size",
        "Choose a PSTD grid size",
        settings->pstdGridSize
    );
    settings->pstdGridSize = pstdgridsize;
    
    sbGridSize->setMinimum(Settings::getInstance()->pstdGridSize);
    sbGridSize->setMaximum(10*Settings::getInstance()->pstdGridSize);
    sbGridSize->setSingleStep(Settings::getInstance()->pstdGridSize);
    sbGridSize->setValue(Settings::getInstance()->pstdGridSize);
}

/**
 * Callback method for the clear scene action in the scene menu.
 * Removes all domains, sources, and receivers from the model.
 */
void Window::slot_clearscene() {
    // Delete all domains
    Model::getInstance()->domains.clear();
    
    // Delete all sources
    Model::getInstance()->sources.clear();
    
    // Delete all receivers
    Model::getInstance()->receivers.clear();
    
    // Clear the selected objects vector
    eventlistener->eventhandler->clearSelection();
}

/**
 * Callback method for the delete selected action in the scene menu.
 * Removes all selected objects from the model.
 */
void Window::slot_deleteselected() {
    // Delete all selected objects
    eventlistener->eventhandler->deleteSelected();
}

/**
 * Callback method for the FPS counter action in the view menu.
 * Toggles the display of the on-screen FPS counter.
 */
void Window::slot_fpscounter() {
    Model::getInstance()->showFPS = ui->actionFPS_counter->isChecked();
    ui->actionFPS_color->setEnabled(Model::getInstance()->showFPS);
}

/**
 * Callback method for the grid action in the view menu.
 * Toggles the display of the grid.
 */
void Window::slot_grid() {
    Model::getInstance()->showGrid = ui->actionGrid->isChecked();
}

/**
 * Callback method for the changelog action in the about menu.
 * Shows a popup containing the changelog of the current version.
 */
void Window::slot_changelog() {
    // Read the changelog file
    std::string changelog = "";
    std::ifstream infile("../changelog");
    std::string line;
    while (std::getline(infile, line)) {
        if (changelog != "") changelog += "\n";
        changelog += line;
    }
    
    // Show a message box with the changelog text
    QMessageBox box(
        "Changelog",
        QString(changelog.c_str()),
        QMessageBox::NoIcon,
        QMessageBox::Close,
        QMessageBox::NoButton,
        QMessageBox::NoButton
    );
    box.exec();
}