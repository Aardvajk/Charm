#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QMap>

#include <QtWidgets/QMainWindow>

class Settings;
class Graphics;
class Model;
class ModelBuffers;
class ModelViewGrid;
class Tool;
class ActionList;
class QMenu;
class AnimationPanel;
class FramePanel;
class JointPanel;
class WeightPanel;
class GroupPanel;
class PaintPanel;
class ModeBox;
class SettingsPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void
    render();

    void
    fileUsed(const QString &path);

    void
    fileFailed(const QString &path);

    void
    savingSettings();

protected:
    virtual
    void
    showEvent(QShowEvent *event);

    virtual
    void
    closeEvent(QCloseEvent *event);

private slots:
    void
    renderTimeout();

    void
    toolChanged(Tool *tool, Tool *previous);

    void
    modifiedStateChanged(bool state);

    void
    pathChanged(const QString &path);

    void
    newModel();

    bool
    open();

    bool
    save();

    bool
    saveAs();

    void
    fileRequested(const QString &path);

    void
    showOptions();

    void
    modeChanged();

    void
    changed();

    void
    selectionChanged();

private:
    bool
    canClose();

    void
    createFileMenu(QMenu *menu);

    void
    createEditMenu(QMenu *menu);

    void
    createModifyMenu(QMenu *menu);

    void
    createVerticesMenu(QMenu *menu);

    void
    createFacesMenu(QMenu *menu);

    void
    createAnimationMenu(QMenu *menu);

    void
    createToolsMenu(QMenu *menu);

    void
    createHelpMenu(QMenu *menu);

    void
    fileFailedDialog(const QString &action, const QString &path);

    void
    updateTitle();

    Settings *settings;

    Graphics *graphics;

    Model *model;
    ModelBuffers *modelBuffers;
    ModelViewGrid *grid;

    ActionList *actions;

    JointPanel *jointPanel;
    WeightPanel *weightPanel;
    AnimationPanel *animationPanel;
    FramePanel *framePanel;
    GroupPanel *groupPanel;
    PaintPanel *paintPanel;
    ModeBox *modeBox;
    SettingsPanel *settingsPanel;

    QMap<QWidget*, QWidget*> separators;

    bool firstShow;
};

#endif // MAINWINDOW_H
