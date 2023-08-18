#include "patchtodlfl.hh"
#include "TMPatch.hh"
#include <QDebug>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QtAlgorithms>

BiCubicBezierPatchToDLFLData::BiCubicBezierPatchToDLFLData() {
  this->vertexCount = 0;
  this->uv_size =
      (1 << TMPatch::PatchNestingFactor) * TMPatch::PatchGridDimension;
  this->UV_values.reserve(this->uv_size + 1);
  double t = 1.0 / (uv_size);
  for (int i = 0; i <= this->uv_size; ++i) {
    double u = t * i;
    this->UV_values.push_back(FourUValues(u));
  }
}

void BiCubicBezierPatchToDLFLData::processBezierPatch(const TMPatch &patch) {
  if (patch.childs != NULL) {
    processBezierPatch(patch.childs[0]);
    processBezierPatch(patch.childs[1]);
    processBezierPatch(patch.childs[2]);
    processBezierPatch(patch.childs[3]);
    return;
  }

  int increament = 1 << patch.level;
  Point3dArrayArray grid;
  int gridSize = (this->uv_size) / increament + 1;
  grid.reserve(gridSize);

  int i_index = 0;
  while (i_index <= this->uv_size) {
    arrayOfPoints pnts;
    pnts.reserve(gridSize);
    int j_index = 0;
    while (j_index <= this->uv_size) {
      Point3d pnt =
          patch.point_at(this->UV_values[i_index], this->UV_values[j_index]);
      pnts.push_back(pnt);
      if (!point_to_index.contains(pnt)) {
        vertexCount++;
        pnt.id = vertexCount;
        point_to_index.insert(pnt, vertexCount);
      }
      j_index += increament;
    }
    grid.push_back(pnts);
    i_index += increament;
  }

  for (int i = 0; i < gridSize - 1; ++i) {
    for (int j = 0; j < gridSize - 1; ++j) {
      QuadFace *newFace = new QuadFace(point_to_index.value(grid[i][j]),
                                       point_to_index.value(grid[i + 1][j]),
                                       point_to_index.value(grid[i + 1][j + 1]),
                                       point_to_index.value(grid[i][j + 1]));
      this->faces.push_back(newFace);
    }
  }
  grid.clear();
}

struct patch_data BiCubicBezierPatchToDLFLData::get_patch_data() {
  QList<Point3d> sortedPoints = this->point_to_index.keys();
  point3d_comparer pnt_comparer;
  qSort(sortedPoints.begin(), sortedPoints.end(), pnt_comparer);
  struct patch_data data;
  data.vertex_count = sortedPoints.size();
  data.vertices = new Vector3d[sortedPoints.size()];
  QList<Point3d>::iterator first, last;
  first = sortedPoints.begin();
  last = sortedPoints.end();
  int v_counter = 0;
  while (first != last) {
    data.vertices[v_counter].set((*first)[0], (*first)[1], (*first)[2]);
    first++;
    v_counter++;
  }
  data.face_count = faces.size();
  data.face_indices = new uint[4 * faces.size()];
  for (int i = 0; i < this->faces.size(); ++i) {
    data.face_indices[i * 4] = (*(faces[i]))[0];
    data.face_indices[i * 4 + 1] = (*(faces[i]))[1];
    data.face_indices[i * 4 + 2] = (*(faces[i]))[2];
    data.face_indices[i * 4 + 3] = (*(faces[i]))[3];
    if (data.face_indices[i * 4] == 0 || data.face_indices[i * 4 + 1] == 0 ||
        data.face_indices[i * 4 + 2] == 0 ||
        data.face_indices[i * 4 + 3] == 0) {
      qDebug() << "Data has zero for face indices";
    }
  }
  return data;
}

// this function was developed for debug purpose only. I thought it might be
// useful and I did not remove it
void BiCubicBezierPatchToDLFLData::toFile(QString address) {
  QFile file(address);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    return;
  }
  QList<Point3d> sortedPoints = this->point_to_index.keys();
  point3d_comparer pnt_comparer;
  qSort(sortedPoints.begin(), sortedPoints.end(), pnt_comparer);

  QTextStream out(&file);
  out << "# Patch data to OBJ"
      << "\n";
  // write the vertices
  QList<Point3d>::iterator first, last;
  first = sortedPoints.begin();
  last = sortedPoints.end();
  while (first != last) {
    out << "v"
        << " " << (*first)[0] << " " << (*first)[1] << " " << (*first)[2]
        << "\n";
    first++;
  }
  out << "# Faces OBJ"
      << "\n";
  // write the faces
  for (int i = 0; i < this->faces.size(); ++i) {
    out << "f"
        << " " << (*(faces[i]))[0] << " " << (*(faces[i]))[1] << " "
        << (*(faces[i]))[2] << " " << (*(faces[i]))[3] << "\n";
  }

  file.flush();
  file.close();
}

void BiCubicBezierPatchToDLFLData::release_memory(void) {
  for (int i = 0; i < this->faces.size(); ++i) {
    delete faces[i];
    faces[i] = NULL;
  }
  faces.clear();
  this->point_to_index.clear();
}
