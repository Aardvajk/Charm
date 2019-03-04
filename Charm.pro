QT += core gui widgets xml
TARGET = Ark
TEMPLATE = app

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -fexceptions -frtti -fno-strict-aliasing \
                  -DQT_NO_CAST_TO_ASCII \
                  -fcheck-new -DNOMINMAX -std=gnu++0x -m64 \
                  -include "QtCore/QDebug"

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

QMAKE_LFLAGS += -fexceptions -frtti -fno-builtin -fno-strict-aliasing \
                -static-libgcc -static -Wl,-enable-auto-import \
                -Wl,-enable-runtime-pseudo-reloc -Wl,--as-needed \
                -Wl,-enable-stdcall-fixup -Wl,-allow-multiple-definition -Wl,-enable-auto-import -m64

INCLUDEPATH += "C:/mingw64/x86_64-w64-mingw32.shared/include" \
               "C:/mingw64/x86_64-w64-mingw32.shared/qt5/include"

LIBS += "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3d9.lib" \
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx9.lib" \
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/dxerr.lib" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libuser32.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libgdi32.a" \
        "C:/mingw64/x86_64-w64-mingw32.shared/lib/libpsapi.a"

SOURCES += main.cpp \
        MainWindow.cpp \
    graphics/BasicVertex.cpp \
    graphics/GraphicsDevice.cpp \
    graphics/GraphicsWidget.cpp \
    graphics/PointVertex.cpp \
    graphics/Texture.cpp \
    graphics/VertexBuffer.cpp \
    graphics/VertexDeclaration.cpp \
    graphics/VertexShader.cpp \
    graphics/Graphics.cpp \
    common/Fatal.cpp \
    style/Style.cpp \
    model/Model.cpp \
    model/ModelData.cpp \
    buffers/Buffer.cpp \
    buffers/PreviewBuffer.cpp \
    maths/Vec3.cpp \
    model/ModelBuffers.cpp \
    views/ModelView.cpp \
    maths/Maths.cpp \
    maths/Matrix.cpp \
    buffers/WireFrameBuffer.cpp \
    model/caches/FaceReferenceCache.cpp \
    debug/Assert.cpp \
    model/caches/SpatialCache.cpp \
    common/PositionKey.cpp \
    buffers/PointBuffer.cpp \
    views/ModelWidget.cpp \
    gui/ToolBar.cpp \
    views/ModelViewGrid.cpp \
    gui/BackgroundPanel.cpp \
    gui/ToolButton.cpp \
    tools/Tool.cpp \
    tools/ModelViewTool.cpp \
    tools/BaseModelViewTool.cpp \
    common/Rotation.cpp \
    common/VectorAnimation.cpp \
    common/Projection.cpp \
    common/Shade.cpp \
    tools/SelectTool.cpp \
    tools/visuals/BoxMarquee.cpp \
    command/Command.cpp \
    command/SelectCommand.cpp \
    actions/Action.cpp \
    actions/ActionList.cpp \
    command/DeleteCommand.cpp \
    undo/UndoList.cpp \
    dialogs/ErrorDialog.cpp \
    dialogs/NotSavedDialog.cpp \
    dialogs/FileDialog.cpp \
    files/RecentFiles.cpp \
    dialogs/RecentFileNotOpenedDialog.cpp \
    settings/Settings.cpp \
    options/OptionsDialog.cpp \
    options/KeyboardOptions.cpp \
    gui/KeySequenceWidget.cpp \
    gui/LineEdit.cpp \
    gui/OptionsPanel.cpp \
    gui/PanelLineEdit.cpp \
    gui/AxisWidget.cpp \
    gui/PanelButton.cpp \
    gui/ToggleBox.cpp \
    common/VariantSet.cpp \
    tools/MoveTool.cpp \
    model/caches/SelectionCache.cpp \
    tools/RotateTool.cpp \
    tools/ScaleTool.cpp \
    tools/ExtrudeTool.cpp \
    command/ExtrudeCommand.cpp \
    gui/RadioBox.cpp \
    gui/SettingsPanel.cpp \
    gui/AxisLockWidget.cpp \
    operations/DeleteOperations.cpp \
    common/ModelElement.cpp \
    operations/SelectOperations.cpp \
    tools/PrimitiveTool.cpp \
    command/CreateCommand.cpp \
    operations/FaceOperations.cpp \
    command/ModifyCommand.cpp \
    operations/VertexOperations.cpp \
    command/CompositeCommand.cpp \
    operations/HideOperations.cpp \
    command/HideCommand.cpp \
    gui/PanelComboBox.cpp \
    operations/ClipboardOperations.cpp \
    gui/PanelListBox.cpp \
    common/Axis.cpp \
    gui/PopUpWindow.cpp \
    views/ViewSettingsWidget.cpp \
    gui/PanelSeparator.cpp \
    tools/primitives/Primitive.cpp \
    tools/primitives/VertexPrimitive.cpp \
    buffers/JointBuffer.cpp \
    tools/primitives/JointPrimitive.cpp \
    model/caches/JointRelationCache.cpp \
    operations/ModifyOperations.cpp \
    views/ModelViewSettings.cpp \
    gui/PanelGroup.cpp \
    gui/ToolPanel.cpp \
    gui/ToolPanelButton.cpp \
    maths/Quaternion.cpp \
    common/StrictDoubleValidator.cpp \
    tools/visuals/LineHandle.cpp \
    command/TransformCommand.cpp \
    gui/RowWidget.cpp \
    common/Mode.cpp \
    gui/ModeBox.cpp \
    command/RotateCommand.cpp \
    gui/AnimationPanel.cpp \
    model/Animation.cpp \
    dialogs/AnimationDialog.cpp \
    command/AnimationCommand.cpp \
    gui/FramePanel.cpp \
    gui/FrameSlider.cpp \
    model/KeyFrame.cpp \
    command/KeyFrameCommand.cpp \
    model/Joint.cpp \
    command/MoveCommand.cpp \
    common/Color.cpp \
    gui/JointPanel.cpp \
    gui/BasePanelListBox.cpp \
    gui/PanelMultiListBox.cpp \
    dialogs/JointDialog.cpp \
    dialogs/WeightDialog.cpp \
    model/caches/JointMatrixCache.cpp \
    tools/primitives/SpherePrimitive.cpp \
    properties/Property.cpp \
    properties/PropertyPanel.cpp \
    properties/PropertyLineEdit.cpp \
    common/StrictIntValidator.cpp \
    command/MoveCursorCommand.cpp \
    common/Anchor.cpp \
    gui/AnchorWidget.cpp \
    tools/primitives/CylinderPrimitive.cpp \
    tools/primitives/CuboidPrimitive.cpp \
    operations/CursorOperations.cpp \
    properties/PropertyCheckBox.cpp \
    operations/AnimationOperations.cpp \
    converter/Converter.cpp \
    converter/XFileConverter.cpp \
    gui/WeightPanel.cpp \
    gui/WeightCombo.cpp \
    model/WeightInfo.cpp \
    common/TransposedName.cpp \
    model/Group.cpp \
    gui/GroupPanel.cpp \
    command/GroupCommand.cpp \
    dialogs/GroupDialog.cpp \
    common/IdIssuer.cpp \
    model/caches/GroupCache.cpp \
    operations/ConvertOperations.cpp \
    converter/RawBufferConverter.cpp \
    common/DataStream.cpp \
    operations/ToolOperations.cpp \
    command/AnimationEventCommand.cpp \
    dialogs/AnimationEventDialog.cpp \
    dialogs/ExportDialog.cpp \
    converter/StaticBufferConverter.cpp \
    tools/primitives/SupportPrimitive.cpp \
    support/SupportProvider.cpp \
    support/SupportShape.cpp \
    gui/Separator.cpp \
    converter/tiny_obj_loader.cc \
    converter/ObjFileConverter.cpp \
    command/ReorderJointCommand.cpp \
    gui/PaintPanel.cpp \
    gui/PaintListBox.cpp \
    command/PaletteCommand.cpp \
    common/Paint.cpp \
    converter/TileConverter.cpp \
    dialogs/PositionDialog.cpp \
    converter/LevelConverter.cpp

HEADERS  += MainWindow.h \
    graphics/BasicVertex.h \
    graphics/GraphicsDevice.h \
    graphics/GraphicsWidget.h \
    graphics/PointVertex.h \
    graphics/Texture.h \
    graphics/VertexBuffer.h \
    graphics/VertexDeclaration.h \
    graphics/VertexShader.h \
    maths/Vec3.h \
    graphics/Graphics.h \
    common/Fatal.h \
    style/Style.h \
    model/Model.h \
    model/Vertex.h \
    model/Face.h \
    model/ModelData.h \
    buffers/Buffer.h \
    buffers/PreviewBuffer.h \
    model/ModelBuffers.h \
    views/ModelView.h \
    maths/Maths.h \
    maths/Matrix.h \
    buffers/WireFrameBuffer.h \
    model/caches/FaceReferenceCache.h \
    common/IndexPairKey.h \
    debug/Assert.h \
    debug/DebugSettings.h \
    debug/FunctionTimer.h \
    model/caches/SpatialCache.h \
    common/PositionKey.h \
    buffers/PointBuffer.h \
    views/ModelWidget.h \
    gui/ToolBar.h \
    views/ModelViewGrid.h \
    gui/BackgroundPanel.h \
    gui/ToolButton.h \
    common/VariantAnimation.h \
    tools/Tool.h \
    tools/ModelViewTool.h \
    tools/BaseModelViewTool.h \
    common/Projection.h \
    common/Rotation.h \
    common/VectorAnimation.h \
    common/Shade.h \
    tools/SelectTool.h \
    tools/visuals/BoxMarquee.h \
    command/Command.h \
    command/SelectCommand.h \
    actions/Action.h \
    actions/ActionList.h \
    command/DeleteCommand.h \
    undo/UndoList.h \
    dialogs/ErrorDialog.h \
    dialogs/NotSavedDialog.h \
    dialogs/FileDialog.h \
    files/RecentFiles.h \
    dialogs/RecentFileNotOpenedDialog.h \
    settings/Settings.h \
    options/OptionsDialog.h \
    options/KeyboardOptions.h \
    gui/KeySequenceWidget.h \
    gui/LineEdit.h \
    gui/OptionsPanel.h \
    gui/PanelLineEdit.h \
    gui/AxisWidget.h \
    gui/PanelButton.h \
    gui/ToggleBox.h \
    common/ModelElement.h \
    common/VariantSet.h \
    tools/MoveTool.h \
    model/caches/SelectionCache.h \
    tools/RotateTool.h \
    tools/ScaleTool.h \
    tools/ExtrudeTool.h \
    command/ExtrudeCommand.h \
    model/Edge.h \
    gui/RadioBox.h \
    gui/SettingsPanel.h \
    gui/AxisLockWidget.h \
    operations/DeleteOperations.h \
    operations/SelectOperations.h \
    tools/PrimitiveTool.h \
    command/CreateCommand.h \
    operations/FaceOperations.h \
    command/ModifyCommand.h \
    common/StoredItem.h \
    operations/VertexOperations.h \
    command/CompositeCommand.h \
    operations/HideOperations.h \
    command/HideCommand.h \
    gui/PanelComboBox.h \
    operations/ClipboardOperations.h \
    gui/PanelListBox.h \
    common/Axis.h \
    gui/PopUpWindow.h \
    views/ViewSettingsWidget.h \
    gui/PanelSeparator.h \
    tools/primitives/Primitive.h \
    tools/primitives/VertexPrimitive.h \
    buffers/JointBuffer.h \
    model/Joint.h \
    tools/primitives/JointPrimitive.h \
    model/caches/JointRelationCache.h \
    operations/ModifyOperations.h \
    views/ModelViewSettings.h \
    gui/PanelGroup.h \
    gui/ToolPanel.h \
    gui/ToolPanelButton.h \
    maths/Quaternion.h \
    common/StrictDoubleValidator.h \
    tools/visuals/LineHandle.h \
    command/TransformCommand.h \
    gui/RowWidget.h \
    common/Mode.h \
    gui/ModeBox.h \
    command/RotateCommand.h \
    gui/AnimationPanel.h \
    model/Animation.h \
    dialogs/AnimationDialog.h \
    command/AnimationCommand.h \
    gui/FramePanel.h \
    gui/FrameSlider.h \
    model/KeyFrame.h \
    command/KeyFrameCommand.h \
    command/MoveCommand.h \
    common/Color.h \
    gui/JointPanel.h \
    gui/BasePanelListBox.h \
    gui/PanelMultiListBox.h \
    dialogs/JointDialog.h \
    dialogs/WeightDialog.h \
    model/caches/JointMatrixCache.h \
    tools/primitives/SpherePrimitive.h \
    properties/Property.h \
    properties/PropertyPanel.h \
    properties/PropertyLineEdit.h \
    common/StrictIntValidator.h \
    command/MoveCursorCommand.h \
    common/Anchor.h \
    gui/AnchorWidget.h \
    tools/primitives/CylinderPrimitive.h \
    tools/primitives/CuboidPrimitive.h \
    operations/CursorOperations.h \
    properties/PropertyCheckBox.h \
    operations/AnimationOperations.h \
    converter/Converter.h \
    converter/XFileConverter.h \
    gui/WeightPanel.h \
    gui/WeightCombo.h \
    model/WeightInfo.h \
    common/TransposedName.h \
    model/Group.h \
    gui/GroupPanel.h \
    command/GroupCommand.h \
    dialogs/GroupDialog.h \
    common/IdIssuer.h \
    model/caches/GroupCache.h \
    operations/ConvertOperations.h \
    converter/RawBufferConverter.h \
    common/DataStream.h \
    operations/ToolOperations.h \
    command/AnimationEventCommand.h \
    dialogs/AnimationEventDialog.h \
    model/AnimationEvent.h \
    dialogs/ExportDialog.h \
    converter/StaticBufferConverter.h \
    tools/primitives/SupportPrimitive.h \
    support/SupportProvider.h \
    support/SupportShape.h \
    gui/Separator.h \
    converter/tiny_obj_loader.h \
    converter/ObjFileConverter.h \
    command/ReorderJointCommand.h \
    gui/PaintPanel.h \
    gui/PaintListBox.h \
    command/PaletteCommand.h \
    common/Paint.h \
    converter/TileConverter.h \
    dialogs/PositionDialog.h \
    common/Optional.h \
    converter/LevelConverter.h

OTHER_FILES +=

RC_FILE += Charm.rc

RESOURCES += \
    Resources.qrc
