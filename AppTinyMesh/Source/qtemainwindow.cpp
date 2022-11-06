#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"
#include <QFileDialog>
#include <QElapsedTimer>

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

    rotation = 45;
    uiw->rotationDegrees->setValue(rotation);

    // Creation des connect
    CreateActions();

    // Initialisation de HeightField
    hf = HeightField();
    maxHeight = 50;
    uiw->heightMax->setText(QString::number(maxHeight));
    uiw->heightMaxSlider->setValue(maxHeight);
    widthSize = 5;
    uiw->widthSize->setText(QString::number(widthSize));
    uiw->widthSizeSlider->setValue(widthSize);
    slopeCoeff = 1;
    uiw->colorSlider->setValue(slopeCoeff);
    uiw->colorSlope->setText(QString::number(slopeCoeff));

    flattenX = 0;
    flattenY = 0;

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

    connect(uiw->rotationDegrees, SIGNAL(valueChanged(int)), this, SLOT(SetRotate(int)));
    connect(uiw->rotateButton, SIGNAL(clicked()), this, SLOT(Rotation()));
    connect(uiw->translateLeftBtn, SIGNAL(clicked()), this, SLOT(TranslationLeft()));
    connect(uiw->translateRightBtn, SIGNAL(clicked()), this, SLOT(TranslationRight()));
    connect(uiw->translateBehindBtn, SIGNAL(clicked()), this, SLOT(TranslationBehind()));
    connect(uiw->translateInFrontBtn, SIGNAL(clicked()), this, SLOT(TranslationInFront()));
    connect(uiw->translateTopBtn, SIGNAL(clicked()), this, SLOT(TranslationTop()));
    connect(uiw->translateBottomBtn, SIGNAL(clicked()), this, SLOT(TranslationBottom()));
    connect(uiw->scale1Btn, SIGNAL(clicked()), this, SLOT(Scale1()));
    connect(uiw->scale2Btn, SIGNAL(clicked()), this, SLOT(Scale2()));
    connect(uiw->scale3Btn, SIGNAL(clicked()), this, SLOT(Scale3()));
    connect(uiw->scale4Btn, SIGNAL(clicked()), this, SLOT(Scale4()));

    // HeightField buttons
    connect(uiw->fileBtn, SIGNAL(clicked()), this, SLOT(LoadFile()));
    connect(uiw->generateBtn, SIGNAL(clicked()), this, SLOT(GenerateHeightField()));
    connect(uiw->widthSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(SetWidthSize(int)));
    connect(uiw->widthSize, SIGNAL(textChanged(QString)), this, SLOT(SetWidthSize(QString)));
    connect(uiw->heightMaxSlider, SIGNAL(valueChanged(int)), this, SLOT(SetMaxHeight(int)));
    connect(uiw->heightMax, SIGNAL(textChanged(QString)), this, SLOT(SetMaxHeight(QString)));
    connect(uiw->colorSlider, SIGNAL(valueChanged(int)), this, SLOT(SetSlopeCoeff(int)));
    connect(uiw->colorSlope, SIGNAL(textChanged(QString)), this, SLOT(SetSlopeCoeff(QString)));

    connect(uiw->flattenXSlider, SIGNAL(valueChanged(int)), this, SLOT(SetFlattenX(int)));
    connect(uiw->flattenX, SIGNAL(textChanged(QString)), this, SLOT(SetFlattenX(QString)));
    connect(uiw->flattenYSlider, SIGNAL(valueChanged(int)), this, SLOT(SetFlattenY(int)));
    connect(uiw->flattenY, SIGNAL(textChanged(QString)), this, SLOT(SetFlattenY(QString)));
    connect(uiw->flattenRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(SetFlattenRadius(int)));
    connect(uiw->flattenRadius, SIGNAL(textChanged(QString)), this, SLOT(SetFlattenRadius(QString)));
    connect(uiw->flattenBtn, SIGNAL(clicked()), this, SLOT(Flatten()));

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
    QElapsedTimer timer;
    timer.start();
    meshWidget->ClearAll();
    meshWidget->AddMesh("Mesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));
    uiw->lineEdit_3->setText(QString::number(timer.elapsed()));

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
  heightFieldPlane = this->hf.generateMesh((double)this->maxHeight/50, (double)this->widthSize/500);
  meshColor = this->hf.generateMeshColor(this->heightFieldPlane, this->slopeCoeff);

  uiw->flattenXSlider->setMaximum(this->hf.getWidth()-1);
  uiw->flattenYSlider->setMaximum(this->hf.getLength()-1);
  UpdateGeometry();
}

void MainWindow::SetRotate(int degrees)
{
  this->rotation = degrees;
}

void MainWindow::Rotation()
{
  Matrix rotationMatrix = Matrix();
  if (uiw->rotationXRadio->isChecked())
    rotationMatrix = Matrix::rotationX(this->rotation);
  else if (uiw->rotationYRadio->isChecked())
    rotationMatrix = Matrix::rotationY(this->rotation);
  else
    rotationMatrix = Matrix::rotationZ(this->rotation);

  this->meshColor.Transform(rotationMatrix);
  UpdateGeometry();
}

void MainWindow::Scale1()
{
  this->meshColor.Transform(Matrix::scale(0.5));
  UpdateGeometry();
}

void MainWindow::Scale2()
{
  this->meshColor.Transform(Matrix::scale(0.75));
  UpdateGeometry();
}

void MainWindow::Scale3()
{
  this->meshColor.Transform(Matrix::scale(1.25));
  UpdateGeometry();
}

void MainWindow::Scale4()
{
  this->meshColor.Transform(Matrix::scale(1.5));
  UpdateGeometry();
}

void MainWindow::TranslationLeft()
{
  this->meshColor.Translate(Vector(-1, 0, 0));
  UpdateGeometry();
}

void MainWindow::TranslationRight()
{
  this->meshColor.Translate(Vector(1, 0, 0));
  UpdateGeometry();
}

void MainWindow::TranslationBehind()
{
  this->meshColor.Translate(Vector(0, 1, 0));
  UpdateGeometry();
}

void MainWindow::TranslationInFront()
{
  this->meshColor.Translate(Vector(0, -1, 0));
  UpdateGeometry();
}

void MainWindow::TranslationTop()
{
  this->meshColor.Translate(Vector(0, 0, 1));
  UpdateGeometry();
}

void MainWindow::TranslationBottom()
{
  this->meshColor.Translate(Vector(0, 0, -1));
  UpdateGeometry();
}

void MainWindow::SetWidthSize(int size)
{
  this->widthSize = size;
  uiw->widthSize->setText(QString::number(widthSize));
}

void MainWindow::SetWidthSize(QString size)
{
  this->widthSize = size.toInt();
  uiw->widthSizeSlider->setValue(widthSize);
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

void MainWindow::SetSlopeCoeff(int coeff)
{
  this->slopeCoeff = coeff;
  uiw->colorSlope->setText(QString::number(slopeCoeff));
}

void MainWindow::SetSlopeCoeff(QString coeff)
{
  this->slopeCoeff = coeff.toInt();
  uiw->colorSlider->setValue(slopeCoeff);
}

void MainWindow::SetFlattenX(int x)
{
  this->flattenX = x;
  uiw->flattenX->setText(QString::number(flattenX));
}

void MainWindow::SetFlattenX(QString x)
{
  this->flattenX = x.toInt();
  uiw->flattenXSlider->setValue(flattenX);
}

void MainWindow::SetFlattenY(int y)
{
  this->flattenY = y;
  uiw->flattenY->setText(QString::number(flattenY));
}

void MainWindow::SetFlattenY(QString y)
{
  this->flattenY = y.toInt();
  uiw->flattenYSlider->setValue(flattenY);
}

void MainWindow::SetFlattenRadius(int radius)
{
  this->flattenRadius = radius;
  uiw->flattenRadius->setText(QString::number(flattenRadius));
}

void MainWindow::SetFlattenRadius(QString radius)
{
  this->flattenRadius = radius.toInt();
  uiw->flattenRadiusSlider->setValue(flattenRadius);
}

void MainWindow::Flatten()
{
  this->hf.flatten(this->flattenX, this->flattenY, this->flattenRadius, this->hf[flattenX][flattenY], 1);
  GenerateHeightField();
  UpdateGeometry();
}
