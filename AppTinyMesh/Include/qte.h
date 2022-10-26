#ifndef __Qte__
#define __Qte__

#include <QtWidgets/qmainwindow.h>
#include "realtime.h"
#include "meshcolor.h"
#include "heightfield.h"

QT_BEGIN_NAMESPACE
	namespace Ui { class Assets; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::Assets* uiw;           //!< Interface

  MeshWidget* meshWidget;   //!< Viewer
  MeshColor meshColor;		//!< Mesh.

  HeightField hf;
  int heightPlaneSize;
  int maxHeight;

public:
  MainWindow();
  ~MainWindow();
  void CreateActions();
  void UpdateGeometry();

public slots:
  void editingSceneLeft(const Ray&);
  void editingSceneRight(const Ray&);
  void BoxMeshExample();
  void DiskMeshExample();
  void CylinderMeshExample();
  void SphereMeshExample();
  void TorusMeshExample();
  void CapsuleMeshExample();
  void GenerateHeightField();
  void SphereImplicitExample();
  void ResetCamera();
  void UpdateMaterial();
  void LoadFile();
  void SetHeightPlaneSize(int);
  void SetHeightPlaneSize(QString);
  void SetMaxHeight(int);
  void SetMaxHeight(QString);
};

#endif
