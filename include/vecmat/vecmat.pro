#message (Compiler = $$QMAKE_CXX)
# QMAKE_CXX=
CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib

include(../../CommonBuildFlags.pri)

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	Matrix3x3.hh \
	Matrix4x4.hh \
	Quaternion.hh \
	Vector.hh \
	Vector2d.hh \
	Vector3d.hh \
	Vector4d.hh

SOURCES += \
	Matrix3x3.cc \
	Matrix4x4.cc \
	Vector.cc \
	Vector2d.cc \
	Vector3d.cc \
	Vector4d.cc


