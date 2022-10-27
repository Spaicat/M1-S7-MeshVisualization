#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"
#include <QFileDialog>

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
    // Chargement de l'interface
    uiw->setupUi(this);

    // Chargement du GLWidget
    meshWidget = new MeshWidget;
    QGridLayout* GLlayout = new QGridLayout;
    GLlayout->addWidget(meshWidget, 0, 0);
    GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

    // Creation des connect
    CreateActions();

    // Initialisation vide de HeightField
    hf = HeightField();
    maxHeight = 50;
    uiw->heightMax->setText(QString::number(maxHeight));
    uiw->heightMaxSlider->setValue(maxHeight);
    heightPlaneSize = 5;
    uiw->heightSize->setText(QString::number(heightPlaneSize));
    uiw->heightSizeSlider->setValue(heightPlaneSize);
    isColorsChecked = false;

    meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
    delete meshWidget;
}

void MainWindow::CreateActions()
{
    // Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->diskMesh, SIGNAL(clicked()), this, SLOT(DiskMeshExample()));
    connect(uiw->cylinderMesh, SIGNAL(clicked()), this, SLOT(CylinderMeshExample()));
    connect(uiw->sphereMesh, SIGNAL(clicked()), this, SLOT(SphereMeshExample()));
    connect(uiw->torusMesh, SIGNAL(clicked()), this, SLOT(TorusMeshExample()));
    connect(uiw->capsuleMesh, SIGNAL(clicked()), this, SLOT(CapsuleMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

    // HeightField buttons
    connect(uiw->fileBtn, SIGNAL(clicked()), this, SLOT(LoadFile()));
    connect(uiw->generateBtn, SIGNAL(clicked()), this, SLOT(GenerateHeightField()));
    connect(uiw->withColors, SIGNAL(clicked(bool)), this, SLOT(SetColorsChecked(bool)));
    connect(uiw->heightSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(SetHeightPlaneSize(int)));
    connect(uiw->heightSize, SIGNAL(textChanged(QString)), this, SLOT(SetHeightPlaneSize(QString)));
    connect(uiw->heightMaxSlider, SIGNAL(valueChanged(int)), this, SLOT(SetMaxHeight(int)));
    connect(uiw->heightMax, SIGNAL(textChanged(QString)), this, SLOT(SetMaxHeight(QString)));

    // Widget edition
    connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
    connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
}

void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::BoxMeshExample()
{
    Mesh boxMesh = Mesh(Box(1.0));

    std::vector<Color> cols;
    cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

    meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::DiskMeshExample()
{
    Mesh diskMesh = Mesh(Disk(2), 32);

    // std::vector<Color> cols;
    // cols.resize(diskMesh.Vertexes());
    // for (int i = 0; i < cols.size(); i++)
    // 	cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

    meshColor = MeshColor(diskMesh);
    UpdateGeometry();
}

void MainWindow::CylinderMeshExample()
{
    Mesh cylinderMesh = Mesh(Cylinder(4, 2), 64);

    // std::vector<Color> cols;
    // cols.resize(diskMesh.Vertexes());*
    // for (int i = 0; i < cols.size(); i++)
    // 	cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

    meshColor = MeshColor(cylinderMesh);
    UpdateGeometry();
}

void MainWindow::SphereMeshExample()
{
    Mesh sphereMesh = Mesh(Sphere(3), 64);
    // std::vector<Color> cols;
    // cols.resize(diskMesh.Vertexes());*
    // for (int i = 0; i < cols.size(); i++)
    // 	cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

    meshColor = MeshColor(sphereMesh);
    UpdateGeometry();
}

void MainWindow::TorusMeshExample()
{
    Mesh torusMesh = Mesh(Torus(3, 2), 32, 32);

    // std::vector<Color> cols;
    // cols.resize(diskMesh.Vertexes());*
    // for (int i = 0; i < cols.size(); i++)
    // 	cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

    meshColor = MeshColor(torusMesh);
    UpdateGeometry();
}

void MainWindow::CapsuleMeshExample()
{
    Mesh capsuleMesh = Mesh(Capsule(4, 2), 7);

    // std::vector<Color> cols;
    // cols.resize(diskMesh.Vertexes());*
    // for (int i = 0; i < cols.size(); i++)
    // 	cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

    meshColor = MeshColor(capsuleMesh);
    UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
  AnalyticScalarField implicit;

  Mesh implicitMesh;
  implicit.Polygonize(31, implicitMesh, Box(2.0));

  std::vector<Color> cols;
  cols.resize(implicitMesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);

  meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
  UpdateGeometry();
}

void MainWindow::UpdateGeometry()
{
    meshWidget->ClearAll();
    meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

    UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
        meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
    else
        meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
    meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}

void MainWindow::LoadFile()
{
  QString filename = QFileDialog::getOpenFileName(
    this,
    "Open height field",
    QDir::currentPath(),
    "All files (*.*);;PNG (*.png);;GIF (*.gif);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;SVG (*.svg);;WBMP (*.wbmp);;WEBP (*.webp)"
  );

  // Set label to the file path and name
  uiw->fileInput->setText(filename);

  QImage image = QImage(filename);

  this->hf = HeightField(image);
}

void MainWindow::GenerateHeightField()
{
  if (this->isColorsChecked)
  {
    meshColor = this->hf.generateMeshColor((double)this->maxHeight/50, (double)this->heightPlaneSize/500);
  }
  else
  {
    Mesh heightField = this->hf.generateMesh((double)this->maxHeight/50, (double)this->heightPlaneSize/500);
    meshColor = MeshColor(heightField);
  }
  UpdateGeometry();
}

void MainWindow::SetColorsChecked(bool isChecked)
{
  this->isColorsChecked = isChecked;
}

void MainWindow::SetHeightPlaneSize(int size)
{
  this->heightPlaneSize = size;
  uiw->heightSize->setText(QString::number(heightPlaneSize));
}

void MainWindow::SetHeightPlaneSize(QString size)
{
  this->heightPlaneSize = size.toInt();
  uiw->heightSizeSlider->setValue(heightPlaneSize);
}

void MainWindow::SetMaxHeight(int max)
{
  this->maxHeight = max;
  uiw->heightMax->setText(QString::number(maxHeight));
}

void MainWindow::SetMaxHeight(QString max)
{
  this->maxHeight = max.toInt();
  uiw->heightMaxSlider->setValue(maxHeight);
}
