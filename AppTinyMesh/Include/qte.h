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
  int rotation;

  HeightField hf;
  Mesh heightFieldPlane;
  int widthSize;
  int maxHeight;
  int slopeCoeff;
  int flattenX;
  int flattenY;
  int flattenRadius;

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
  void MergedMeshExample();
  void GenerateHeightField();
  void SphereImplicitExample();
  void ResetCamera();
  void UpdateMaterial();
  void LoadFile();
  void SetRotate(int);
  void Rotation();
  void Scale1();
  void Scale2();
  void Scale3();
  void Scale4();
  void TranslationLeft();
  void TranslationRight();
  void TranslationBehind();
  void TranslationInFront();
  void TranslationTop();
  void TranslationBottom();
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
  void SetFlattenRadius(int);
  void SetFlattenRadius(QString);
  void Flatten();
};

#endif
