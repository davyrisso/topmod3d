CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib

include(../../CommonBuildFlags.pri)

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += .. ../vecmat ../dlflcore

HEADERS += \
	DLFLCast.hh  \
	DLFLConnect.hh  \
	DLFLConvexHull.hh  \
	DLFLCrust.hh  \
	DLFLDual.hh  \
	DLFLExtrude.hh  \
	DLFLMeshSmooth.hh  \
	DLFLMultiConnect.hh  \
	DLFLSculpting \
        DLFLSubdiv.hh \
    DLFLCubicBezierConnect.hh \
    DLFLCurvatureConnect.hh

SOURCES += \
	DLFLCast.cc  \
	DLFLConnect.cc  \
	DLFLConvexHull.cc  \
	DLFLCrust.cc  \
	DLFLDual.cc  \
	DLFLExtrude.cc  \
	DLFLMeshSmooth.cc  \
	DLFLMultiConnect.cc  \
	DLFLSculpting.cc \
        DLFLSubdiv.cc \
    DLFLCubicBezierConnect.cc \
    DLFLCurvatureConnect.cc


