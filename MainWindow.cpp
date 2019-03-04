#include "MainWindow.h"

#include <QtCore/QTimer>
#include <QtCore/QFileInfo>

#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include <QtGui/QCloseEvent>

#include <windows.h>

#include "common/Fatal.h"

#include "settings/Settings.h"

#include "style/Style.h"

#include "graphics/Graphics.h"

#include "model/Model.h"
#include "model/ModelBuffers.h"

#include "views/ModelViewGrid.h"

#include "gui/BackgroundPanel.h"
#include "gui/OptionsPanel.h"
#include "gui/ToolPanel.h"
#include "gui/SettingsPanel.h"
#include "gui/ModeBox.h"
#include "gui/JointPanel.h"
#include "gui/WeightPanel.h"
#include "gui/AnimationPanel.h"
#include "gui/FramePanel.h"
#include "gui/GroupPanel.h"
#include "gui/PaintPanel.h"
#include "gui/AnchorWidget.h"
#include "gui/AxisLockWidget.h"

#include "tools/SelectTool.h"
#include "tools/MoveTool.h"
#include "tools/RotateTool.h"
#include "tools/ScaleTool.h"
#include "tools/ExtrudeTool.h"
#include "tools/PrimitiveTool.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "dialogs/ErrorDialog.h"
#include "dialogs/NotSavedDialog.h"
#include "dialogs/FileDialog.h"
#include "dialogs/RecentFileNotOpenedDialog.h"

#include "files/RecentFiles.h"

#include "options/OptionsDialog.h"

#include "operations/DeleteOperations.h"
#include "operations/SelectOperations.h"
#include "operations/FaceOperations.h"
#include "operations/VertexOperations.h"
#include "operations/HideOperations.h"
#include "operations/ClipboardOperations.h"
#include "operations/ModifyOperations.h"
#include "operations/CursorOperations.h"
#include "operations/AnimationOperations.h"
#include "operations/ConvertOperations.h"
#include "operations/ToolOperations.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      firstShow(true)
{
    settings = new Settings(this);
    Settings::loadFromXml("C:/Desk/Charm/settings.xml", settings);

    actions = new ActionList(this);

    graphics = new Graphics(this);
    if(!graphics->initialise())
    {
        Fatal("Unable to acquire graphics device");
    }

    if(!graphics->reset())
    {
        Fatal("Unable to reset graphics device");
    }

    float jointRadius = 0.5f;

    model = new Model(actions, this);
    modelBuffers = new ModelBuffers(graphics, model, jointRadius, this);

    connect(model, SIGNAL(modifiedStateChanged(bool)), SLOT(modifiedStateChanged(bool)));
    connect(model, SIGNAL(pathChanged(QString)), SLOT(pathChanged(QString)));
    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
    connect(model, SIGNAL(changed()), SLOT(changed()));
    connect(model, SIGNAL(selectionChanged()), SLOT(selectionChanged()));

    new DeleteOperations(actions, model, this);
    new SelectOperations(actions, model, this);
    new FaceOperations(actions, model, this);
    new VertexOperations(actions, model, this);
    new HideOperations(actions, model, this);
    new ClipboardOperations(actions, model, this);
    new ModifyOperations(actions, model, this);
    new CursorOperations(actions, model, this);
    new AnimationOperations(actions, model, this);
    new ConvertOperations(actions, model, graphics, this, this);
    new ToolOperations(actions, model, this);

    BackgroundPanel *backgroundPanel = new BackgroundPanel();

    setCentralWidget(backgroundPanel);

    grid = new ModelViewGrid(settings, actions, graphics, model, modelBuffers, this);
    connect(this, SIGNAL(render()), grid, SLOT(updateViews()));
    connect(this, SIGNAL(savingSettings()), grid, SLOT(savingSettings()));

    framePanel = new FramePanel(settings, actions, model, this);

    connect(this, SIGNAL(savingSettings()), framePanel, SLOT(savingSettings()));

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->setMargin(0);
    centralLayout->setSpacing(1);

    centralLayout->addWidget(grid);
    centralLayout->addWidget(framePanel);

    backgroundPanel->boxLayout()->addLayout(centralLayout);

    QVBoxLayout *optionsLayout = new QVBoxLayout();
    optionsLayout->setMargin(0);
    optionsLayout->setSpacing(1);

    OptionsPanel *optionsPanel = new OptionsPanel();

    optionsLayout->addWidget(optionsPanel);

    ToolPanel *toolPanel = new ToolPanel(this);

    settingsPanel = new SettingsPanel(model, actions);

    optionsLayout->addWidget(settingsPanel);

    backgroundPanel->boxLayout()->addLayout(optionsLayout);

    toolPanel->addTool(new SelectTool(settings, actions, model, grid, jointRadius, this));
    toolPanel->addTool(new MoveTool(settings, actions, model, grid, this));
    toolPanel->addTool(new RotateTool(settings, actions, model, grid, this));
    toolPanel->addTool(new ScaleTool(settings, actions, model, grid, this));
    toolPanel->addTool(new ExtrudeTool(settings, actions, model, grid, this));
    toolPanel->addTool(new PrimitiveTool(settings, actions, model, grid, this));

    foreach(Tool *tool, toolPanel->tools())
    {
        optionsPanel->addWidget(tool->optionsWidget());
        tool->optionsWidget()->setVisible(tool == toolPanel->tool());

        tool->enableRelatedActions(false);

        connect(this, SIGNAL(savingSettings()), tool, SLOT(savingSettings()));
    }

    connect(toolPanel, SIGNAL(toolChanged(Tool*,Tool*)), SLOT(toolChanged(Tool*,Tool*)));

    toolChanged(toolPanel->tool(), 0);

    jointPanel = new JointPanel(model);
    weightPanel = new WeightPanel(model);
    paintPanel = new PaintPanel(actions, model);

    animationPanel = new AnimationPanel(model);
    connect(animationPanel, SIGNAL(currentChanged(int)), framePanel, SLOT(currentAnimationChanged(int)));

    groupPanel = new GroupPanel(model, this);

    modeBox = new ModeBox(model, this);

    optionsPanel->addSeparator();

    optionsPanel->addWidget(groupPanel);
    separators[groupPanel] = optionsPanel->addSeparator();

    optionsPanel->addWidget(jointPanel);
    separators[jointPanel] = optionsPanel->addSeparator();

    optionsPanel->addWidget(weightPanel);
    separators[weightPanel] = optionsPanel->addSeparator();

    optionsPanel->addWidget(paintPanel);
    separators[paintPanel] = optionsPanel->addSeparator();

    optionsPanel->addWidget(animationPanel);
    separators[animationPanel] = optionsPanel->addSeparator();

    optionsPanel->addStretch();
    optionsPanel->addSeparator();

    optionsPanel->addWidget(modeBox);

    backgroundPanel->boxLayout()->addWidget(toolPanel);

    createFileMenu(menuBar()->addMenu("&File"));
    createEditMenu(menuBar()->addMenu("&Edit"));
    createModifyMenu(menuBar()->addMenu("&Modify"));
    createVerticesMenu(menuBar()->addMenu("&Vertices"));
    createFacesMenu(menuBar()->addMenu("&Faces"));
    createAnimationMenu(menuBar()->addMenu("&Animation"));
    createToolsMenu(menuBar()->addMenu("&Tools"));
    createHelpMenu(menuBar()->addMenu("&Help"));

    actions->loadFromSettings(settings->child("Actions"));

    model->change();
    model->changeSelection();
    model->setMode(Mode::Model);
    model->setAnchor(Anchor::Selection);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(renderTimeout()));

    timer->start(0);

    updateTitle();
    setWindowIcon(QIcon(":/resources/images/charmsmall.png"));
}

MainWindow::~MainWindow()
{
    delete grid;
}

void
MainWindow::showEvent(QShowEvent *event)
{
    if(firstShow)
    {
        QRect rect = settings->child("Window")->child("Position")->value().toRect();
        bool maximized = settings->child("Window")->child("Maximized")->value().toBool();

        int x = frameGeometry().width() - geometry().width();
        int y = frameGeometry().height() - geometry().height();

        move(rect.left(), rect.top());
        resize(rect.width() - x, rect.height() - y);

        if(maximized)
        {
            setWindowState(Qt::WindowMaximized);
        }

        firstShow = false;
    }
}

void
MainWindow::closeEvent(QCloseEvent *event)
{
    if(!canClose())
    {
        event->ignore();
        return;
    }

    WINDOWPLACEMENT p;
    GetWindowPlacement(reinterpret_cast<HWND>(winId()), &p);

    QRect rect(p.rcNormalPosition.left, p.rcNormalPosition.top, p.rcNormalPosition.right - p.rcNormalPosition.left, p.rcNormalPosition.bottom - p.rcNormalPosition.top);

    settings->child("Window")->clear();
    settings->child("Window")->add("Position", rect);
    settings->child("Window")->add("Maximized", p.showCmd == SW_MAXIMIZE);

    actions->saveToSettings(settings->child("Actions"));

    emit savingSettings();

    Settings::saveToXml("C:/Desk/Charm/settings.xml", settings);
    event->accept();
}

void
MainWindow::renderTimeout()
{
    if(graphics->isLost() || graphics->needsResetting())
    {
        if(graphics->isLost())
        {
            graphics->scheduleReset();
        }

        if(!graphics->isReadyToReset())
        {
            return;
        }

        if(!graphics->reset())
        {
            Fatal("Unable to reset graphics device");
        }
    }

    emit render();
}

void
MainWindow::toolChanged(Tool *tool, Tool *previous)
{
    if(previous)
    {
        previous->disconnectSlots();
        previous->focusLost();

        previous->optionsWidget()->setVisible(false);
    }

    if(tool)
    {
        tool->focusGained();
        tool->connectSlots();

        tool->optionsWidget()->setVisible(true);
        tool->optionsWidget()->setFocus();
    }
}

void
MainWindow::modifiedStateChanged(bool state)
{
    updateTitle();
}

void
MainWindow::pathChanged(const QString &path)
{
    updateTitle();
}

void
MainWindow::newModel()
{
    if(!canClose())
    {
        return;
    }

    model->clear();
    model->setPath(QString());
}

bool
MainWindow::open()
{
    QString path = FileDialog::getOpenFileName(this);

    if(path.isEmpty())
    {
        return false;
    }

    if(!canClose())
    {
        return false;
    }

    if(!model->loadFromFile(path))
    {
        fileFailedDialog("open", path);
        return false;
    }

    model->setPath(path);

    emit fileUsed(path);
    return true;
}

bool
MainWindow::save()
{
    if(model->path().isEmpty())
    {
        return saveAs();
    }

    if(!model->saveToFile(model->path()))
    {
        fileFailedDialog("create", model->path());
        return false;
    }

    model->setSavePoint();

    return true;
}

bool
MainWindow::saveAs()
{
    QString path = FileDialog::getSaveFileName(model->path(), FileDialog::standardFilter(), this);

    if(path.isEmpty())
    {
        return false;
    }

    if(!model->saveToFile(path))
    {
        fileFailedDialog("create", path);
        return false;
    }

    model->setPath(path);
    model->setSavePoint();

    emit fileUsed(path);
    return true;
}

void
MainWindow::fileRequested(const QString &path)
{
    if(path == model->path())
    {
        return;
    }

    if(!canClose())
    {
        return;
    }

    if(!model->loadFromFile(path))
    {
        if(RecentFileNotOpenedDialog::show(this, path) == QMessageBox::Yes)
        {
            emit fileFailed(path);
        }

        return;
    }

    model->setPath(path);

    emit fileUsed(path);
}

void
MainWindow::showOptions()
{
    OptionsDialog dialog(actions, this);
    dialog.exec();
}

void
MainWindow::modeChanged()
{
    animationPanel->setVisible(model->mode() == Mode::Animate);
    separators[animationPanel]->setVisible(model->mode() == Mode::Animate);

    framePanel->setVisible(model->mode() == Mode::Animate);

    paintPanel->setVisible(model->mode() != Mode::Animate);
    separators[paintPanel]->setVisible(model->mode() != Mode::Animate);
}

void
MainWindow::changed()
{
    bool visible = model->jointCount() != 0;

    jointPanel->setVisible(visible);
    separators[jointPanel]->setVisible(visible);

    visible = visible && model->selectedVertices().count();

    weightPanel->setVisible(visible);
    separators[weightPanel]->setVisible(visible);
}

void
MainWindow::selectionChanged()
{
    bool visible = model->jointCount() != 0 && model->selectedVertices().count();

    weightPanel->setVisible(visible);
    separators[weightPanel]->setVisible(visible);
}

bool
MainWindow::canClose()
{
    if(model->modified())
    {
        int result = NotSavedDialog::show(this);

        if(result == QMessageBox::Yes)
        {
            return save();
        }

        if(result == QMessageBox::Cancel)
        {
            return false;
        }
    }

    return true;
}

void
MainWindow::createFileMenu(QMenu *menu)
{
    Action *newAction = actions->add("File", "New");
    newAction->setText("&New");
    newAction->setStatusTip("Create a new model");

    connect(newAction, SIGNAL(triggered()), SLOT(newModel()));

    Action *openAction = actions->add("File", "Open");
    openAction->setText("&Open...");
    openAction->setShortcut(QKeySequence("Ctrl+O"));

    connect(openAction, SIGNAL(triggered()), SLOT(open()));

    Action *saveAction = actions->add("File", "Save");
    saveAction->setText("&Save");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));

    connect(saveAction, SIGNAL(triggered()), SLOT(save()));

    Action *saveAsAction = actions->add("File", "Save As");
    saveAsAction->setText("Save &As...");

    connect(saveAsAction, SIGNAL(triggered()), SLOT(saveAs()));

    Action *exitAction = actions->add("File", "Exit");
    exitAction->setText("E&xit");

    connect(exitAction, SIGNAL(triggered()), SLOT(close()));

    QMenu *recentFilesMenu = new QMenu("&Recent Files");

    RecentFiles *recentFiles = new RecentFiles(recentFilesMenu, settings->child("RecentFiles"), this);

    connect(this, SIGNAL(fileUsed(QString)), recentFiles, SLOT(fileUsed(QString)));
    connect(this, SIGNAL(fileFailed(QString)), recentFiles, SLOT(fileFailed(QString)));
    connect(recentFiles, SIGNAL(fileRequested(QString)), SLOT(fileRequested(QString)));

    menu->addAction(newAction);
    menu->addSeparator();
    menu->addAction(openAction);
    menu->addSeparator();
    menu->addAction(actions->find("File", "Import"));
    menu->addSeparator();
    menu->addAction(actions->find("File", "Export"));
    menu->addSeparator();
    menu->addAction(saveAction);
    menu->addAction(saveAsAction);
    menu->addSeparator();
    menu->addMenu(recentFilesMenu);
    menu->addSeparator();
    menu->addAction(exitAction);
}

void
MainWindow::createEditMenu(QMenu *menu)
{
    menu->addAction(actions->find("Model", "Undo"));
    menu->addAction(actions->find("Model", "Redo"));
    menu->addSeparator();
    menu->addAction(actions->find("Model", "Cut"));
    menu->addAction(actions->find("Model", "Copy"));
    menu->addAction(actions->find("Model", "Paste"));
    menu->addSeparator();
    menu->addAction(actions->find("Model", "Delete"));
    menu->addSeparator();

    QMenu *select = new QMenu("&Select");
    menu->addMenu(select);

    select->addAction(actions->find("Selection", "Select All"));
    select->addAction(actions->find("Selection", "Select None"));
    select->addAction(actions->find("Selection", "Invert Selection"));

    menu->addSeparator();

    QMenu *hide = new QMenu("&Hide");
    menu->addMenu(hide);

    hide->addAction(actions->find("Model", "Hide"));
    hide->addAction(actions->find("Model", "Unhide"));
    hide->addAction(actions->find("Model", "Unhide All"));

    menu->addSeparator();

    QMenu *cursor = new QMenu("&Cursor");
    menu->addMenu(cursor);

    cursor->addAction(actions->find("Cursor", "Selection Centre"));
    cursor->addAction(actions->find("Cursor", "Origin"));
    cursor->addSeparator();
    cursor->addAction(actions->find("Cursor", "Origin To Cursor"));
}

void
MainWindow::createModifyMenu(QMenu *menu)
{
    QMenu *flatten = menu->addMenu("&Flatten");
    flatten->addAction(actions->find("Modify", "Flatten X"));
    flatten->addAction(actions->find("Modify", "Flatten Y"));
    flatten->addAction(actions->find("Modify", "Flatten Z"));

    QMenu *flip = menu->addMenu("F&lip");
    flip->addAction(actions->find("Modify", "Flip X"));
    flip->addAction(actions->find("Modify", "Flip Y"));
    flip->addAction(actions->find("Modify", "Flip Z"));

    QMenu *mirror = menu->addMenu("&Mirror");
    mirror->addAction(actions->find("Modify", "Mirror X"));
    mirror->addAction(actions->find("Modify", "Mirror Y"));
    mirror->addAction(actions->find("Modify", "Mirror Z"));

    QMenu *zero = menu->addMenu("&Zero");
    zero->addAction(actions->find("Modify", "Zero X"));
    zero->addAction(actions->find("Modify", "Zero Y"));
    zero->addAction(actions->find("Modify", "Zero Z"));
}

void
MainWindow::createVerticesMenu(QMenu *menu)
{
    menu->addAction(actions->find("Vertices", "Snap"));
    menu->addAction(actions->find("Vertices", "Weld"));
    menu->addAction(actions->find("Vertices", "Split"));

    menu->addSeparator();

    menu->addAction(actions->find("Vertices", "Split Edge"));

    menu->addSeparator();

    menu->addAction(actions->find("Vertices", "Edit Weights"));
    menu->addAction(actions->find("Vertices", "Clear Weights"));

    menu->addSeparator();

    menu->addAction(actions->find("Vertices", "Select Unassigned"));
    menu->addAction(actions->find("Vertices", "Select Unclosed"));

    menu->addSeparator();

    menu->addAction(actions->find("Vertices", "Edit Position"));
    menu->addAction(actions->find("Vertices", "Snap to Grid"));

    menu->addSeparator();

    menu->addAction(actions->find("Vertices", "Fix Normals"));
    menu->addAction(actions->find("Vertices", "Unfix Normals"));
}

void
MainWindow::createFacesMenu(QMenu *menu)
{
    menu->addAction(actions->find("Faces", "Create"));
    menu->addAction(actions->find("Faces", "Reverse"));

    menu->addSeparator();

    menu->addAction(actions->find("Faces", "Select Vertices"));
    menu->addAction(actions->find("Faces", "Select Degenerates"));

    menu->addSeparator();

    menu->addAction(actions->find("Faces", "Clear Painting"));

    menu->addSeparator();

    menu->addAction(actions->find("Faces", "Group Split"));
}

void
MainWindow::createAnimationMenu(QMenu *menu)
{
    menu->addAction(actions->find("Animation", "Modify Event"));
    menu->addAction(actions->find("Animation", "Reset"));

    menu->addSeparator();

    menu->addAction(actions->find("Animation", "Swap Selected Transforms"));
    menu->addAction(actions->find("Animation", "Swap All Transforms"));
}

void
MainWindow::createToolsMenu(QMenu *menu)
{
    Action *optionsAction = actions->add("Tools", "Options");
    optionsAction->setText("&Options...");

    connect(optionsAction, SIGNAL(triggered()), SLOT(showOptions()));

    menu->addAction(actions->find("View", "Reset All"));

    menu->addSeparator();

    menu->addAction(optionsAction);
}

void
MainWindow::createHelpMenu(QMenu *menu)
{
    menu->addAction("About...");
}

void
MainWindow::fileFailedDialog(const QString &action, const QString &path)
{
    ErrorDialog::show(this, QString("Unable to %1 file <b>%2</b>.").arg(action).arg(path), "Please check the filename and path and try again.");
}

void
MainWindow::updateTitle()
{
    QString path = model->path().isEmpty() ? QString() : QString("%1 - ").arg(QFileInfo(model->path()).fileName());

    setWindowTitle(QString("%1Charm%2").arg(path).arg(model->modified() ? " (modified)" : ""));
}
