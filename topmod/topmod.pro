
QT += core opengl xml gui
include(../CommonBuildFlags.pri)

RC_FILE = topmod.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Downgrade some diagnostics about nonconformant code from errors to warnings.
#Thus, using -fpermissive will allow some nonconforming code to compile.
#We should update the code to get ride of this flag!
QMAKE_CXXFLAGS += -fpermissive


CONFIG -=  WITH_PYTHON WITH_SPACENAV WITH_VERSE
# Note: The line below prevents building on MacOSX (library not found: -lglu32).
#LIBS +=  -lglu32 -lopengl32

# to include the popup command line interface leave the following line uncommented
DEFINES *= QCOMPLETER

# DEFINES += TOPMOD_VERSION 
DEFINES -= GPU_OK

#TEMPLATE = app

#SUBDIRS = include

#Some header files are duplicated and some header files are not in any projects.
#It would be nice to clean them up.

DEPENDPATH += \
	   lang \
           ../include \
           ../include/Graphics \
           ../include/Light \
           ../include/vecmat \
           ../include/dlflcore \
           ../include/dlflaux \
           ../include/verse

INCLUDEPATH += \
            ../include \
            ../include/Graphics \
            ../include/Light \
            ../include/vecmat \
            # ../include/arcball \
            ../include/dlflcore \
            ../include/dlflaux \
            ../include/verse


# Input
HEADERS += \
        DLFLScriptEditor.hh \
	TopModPreferences.hh \
	TdxDeviceWrappers.hh \
	CommandCompleter.hh \
	DLFLLocator.hh \
	GLWidget.hh \
	TopMod.hh \
	MainWindow.hh \
	GeometryRenderer.hh \
	DLFLLighting.hh \	
	qcumber.hh \
	qshortcutdialog.hh \
	qshortcutmanager.hh \
	ui_shortcutdialog.h \
	ui_stylesheeteditor.h \
	editor.hh \
	PythonHighlighter.hh \
	BasicsMode.hh \
	ExtrusionsMode.hh \ 
	RemeshingMode.hh \
	ConicalMode.hh \ 
	HighgenusMode.hh \
	TexturingMode.hh \
	ExperimentalModes.hh \
	DLFLSelection.hh \
	Viewport.hh \
	TMPatchFace.hh \
	TMPatchObject.hh \
	TMPatch.hh \
	DLFLRenderer.hh \
	stylesheeteditor.hh \
        ../include/Base/BaseObject.hh \
        ../include/Base/Constants.hh \
        ../include/Base/Inlines.hh \
        ../include/Base/StreamIO.hh \
        # ../include/Graphics/Camera.hh \
        ../include/Graphics/Color.hh \
        ../include/Graphics/Grid.hh \
        ../include/Graphics/Texture.hh \
        ../include/Graphics/Transform.hh \
        ../include/Light/AmbientLight.hh \
        ../include/Light/Light.hh \
        ../include/Light/PointLight.hh \
        ../include/Light/SpotLight.hh \
	CgData.hh \
        # ../include/Camera2.hh \
        ../include/Camera3.hh \
    ../include/TexturedRenderer.hh \
    ../include/TexturedLitRenderer.hh \
    patchmode.hh \
    patchtodlfl.hh \
    BranchCovering_CornerCutting_Widget.hh

FORMS += shortcutdialog.ui stylesheeteditor.ui

SOURCES += \
        DLFLScriptEditor.cc \
	TopModPreferences.cc \
	TdxDeviceWrappers.cc \
	GLWidget.cc \
	TopMod.cc \
	main.cc \
	MainWindow.cc \
	MainWindowCallbacks.cc \
	MainWindowRemeshingCallbacks.cc \
	GeometryRenderer.cc \
	qshortcutdialog.cc \
	qshortcutmanager.cc \
	editor.cc \
	PythonHighlighter.cc \
	BasicsMode.cc	\
	ExtrusionsMode.cc \
	RemeshingMode.cc \ 
	ConicalMode.cc \ 
	HighgenusMode.cc \
	TexturingMode.cc \
	ExperimentalModes.cc \
	DLFLLighting.cc \
	DLFLRenderer.cc \
	DLFLSelection.cc \
	# DLFLSculpting.cc \
	DLFLUndo.cc \
	DLFLLocator.cc \
	TMPatchObject.cc \
	TMPatchFace.cc \
	stylesheeteditor.cc \
	CgData.cc \
        ../include/Camera3.cc \
	CommandCompleter.cc \
    patchmode.cc \
    patchtodlfl.cc \
    TMPatch.cc  \
    BranchCovering_CornerCutting_Widget.cc

RESOURCES += application.qrc

TRANSLATIONS += \
	lang/topmod_de.ts \
	lang/topmod_it.ts \
	lang/topmod_en.ts \
	lang/topmod_ca.ts \
	lang/topmod_es.ts \
	lang/topmod_fr.ts \
	lang/topmod_hi.ts \
	lang/topmod_tr.ts

CONFIG(WITH_VERSE){

HEADERS += \
        ../include/verse/TKE_verse.h \
        ../include/verse/TIF_verse.h \
        ../include/verse/MEM_guardedalloc.h \
        ../include/verse/DNA_mesh_types.h \
        ../include/verse/DNA_meshdata_types.h \
        ../include/verse/DNA_scriptlink_types.h \
        ../include/verse/DNA_object_types.h \
        ../include/verse/DNA_ID.h \
        ../include/verse/DNA_listBase.h \
        ../include/verse/mydevice.h \
        ../include/verse/TLI_dynamiclist.h \
        ../include/verse/TLI_util.h \
        ../include/verse/TLI_toplib.h \
        ../include/verse/TLI_arithb.h \
        ../include/verse/TKE_utildefines.h \
        ../include/verse/TIF_space.h \
        ../include/verse/TIF_editmesh.h \
        ../include/verse/TKE_depsgraph.h \
        ../include/verse/TKE_global.h \
        ../include/verse/TKE_mesh.h \
        ../include/verse/TLI_editVert.h \
        ../include/verse/TKE_object.h \
	VerseTopMod.hh 
SOURCES += \
        ../include/verse/verse_session.cc \
        ../include/verse/mallocn.cc \
        ../include/verse/verse_common.cc \
        ../include/verse/util.cc \
        ../include/verse/verse_geometry_node.cc \
        ../include/verse/verse_mesh.cc \
        ../include/verse/verse_node.cc \
        ../include/verse/verse_object.cc \
        ../include/verse/verse_object_node.cc \
	VerseTopMod.cc
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../include/dlflaux/release/ -ldlflaux
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../include/dlflaux/debug/ -ldlflaux
else:unix: LIBS += -L$$OUT_PWD/../include/dlflaux/ -ldlflaux

INCLUDEPATH += $$PWD/../include/dlflaux
DEPENDPATH += $$PWD/../include/dlflaux

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflaux/release/libdlflaux.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflaux/debug/libdlflaux.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflaux/release/dlflaux.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflaux/debug/dlflaux.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../include/dlflaux/libdlflaux.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../include/dlflcore/release/ -ldlflcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../include/dlflcore/debug/ -ldlflcore
else:unix: LIBS += -L$$OUT_PWD/../include/dlflcore/ -ldlflcore

INCLUDEPATH += $$PWD/../include/dlflcore
DEPENDPATH += $$PWD/../include/dlflcore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflcore/release/libdlflcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflcore/debug/libdlflcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflcore/release/dlflcore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/dlflcore/debug/dlflcore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../include/dlflcore/libdlflcore.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../include/vecmat/release/ -lvecmat
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../include/vecmat/debug/ -lvecmat
else:unix: LIBS += -L$$OUT_PWD/../include/vecmat/ -lvecmat

INCLUDEPATH += $$PWD/../include/vecmat
DEPENDPATH += $$PWD/../include/vecmat

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/vecmat/release/libvecmat.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/vecmat/debug/libvecmat.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/vecmat/release/vecmat.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../include/vecmat/debug/vecmat.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../include/vecmat/libvecmat.a
