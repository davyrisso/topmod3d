CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib

include(../../CommonBuildFlags.pri)

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += .. ../vecmat

HEADERS += \
	DLFLCommon.hh \
	DLFLCore.hh \
	DLFLCoreExt.hh \
	DLFLEdge.hh \
	DLFLFace.hh \
	DLFLFaceVertex.hh \
	DLFLMaterial.hh \
	DLFLObject.hh \
	DLFLVertex.hh

SOURCES += \
	DLFLCommon.cc \
	DLFLCore.cc \
	DLFLCoreExt.cc \
	DLFLEdge.cc \
	DLFLFace.cc \
	DLFLFaceVertex.cc \
	DLFLFile.cc \
        DLFLFileAlt.cc \
	DLFLObject.cc \
	DLFLVertex.cc

