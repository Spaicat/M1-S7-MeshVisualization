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
  Mesh heightFieldPlane;
  int widthSize;
  int maxHeight;
  int slopeCoeff;
  int flattenX;
  int flattenY;

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
  void SetWidthSize(int);
  void SetWidthSize(QString);
  void SetMaxHeight(int);
  void SetMaxHeight(QString);
  void SetSlopeCoeff(int);
  void SetSlopeCoeff(QString);
  void SetFlattenX(int);
  void SetFlattenX(QString);
  void SetFlattenY(int);
  void SetFlattenY(QString);
  void Flatten();
};

#endif
