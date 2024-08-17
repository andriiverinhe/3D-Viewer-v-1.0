#include "mainwindow.hpp"
void MainWindow::showMessageError(const QString &title,
                                  const QString &message) {
  QMessageBox::critical(nullptr, title, message);
}

bool MainWindow::checkCorrect() {
  bool code = false;
  if (init)
    if (glWidget->getCodeParser() == PARSER_OK) code = true;
  return code;
}

void MainWindow::setLineValidator(QLineEdit *line, const double &min,
                                  const double &max) {
  if (line != nullptr)
    line->setValidator(new QDoubleValidator(min, max, 7, this));
}

void MainWindow::initStep() {
  double min = 0;
  double max = std::numeric_limits<double>::max();
  setLineValidator(ui->moveStepX, min, max);
  setLineValidator(ui->moveStepY, min, max);
  setLineValidator(ui->moveStepZ, min, max);
  setLineValidator(ui->rotateStepX, min, max);
  setLineValidator(ui->rotateStepY, min, max);
  setLineValidator(ui->rotateStepZ, min, max);
  setLineValidator(ui->scaleStep, min, max);

  ui->moveStepX->setText("0.1");
  ui->moveStepY->setText("0.1");
  ui->moveStepZ->setText("0.1");
  ui->rotateStepX->setText("0.1");
  ui->rotateStepY->setText("0.1");
  ui->rotateStepZ->setText("0.1");
  ui->scaleStep->setText("0.1");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  glWidget = nullptr;
  ui->setupUi(this);
  initStep();
  init = 0;

  ui->stopBtn->setVisible(false);
  ui->fileNameLbl->setVisible(false);
  ui->vertexInfo->setVisible(false);
  ui->edgeInfo->setVisible(false);
}

MainWindow::~MainWindow() {
  if (glWidget != nullptr && glWidget->glData.gif_start == 1)
    QDir(glWidget->gifData_.directoryPathBMP).removeRecursively();

  if (init) delete glWidget;
  delete ui;
}

static void setMessageToLine(QAction *line, const double &size,
                             const QString &mes) {
  line->setText(mes + QString::number(size));
}

void MainWindow::setNewSettingsToIU() {
  if (checkCorrect()) {
    ui->menuSettings->setEnabled(true);
    ui->menuSave->setEnabled(true);
    ui->menuDisplay->setEnabled(true);

    if (glWidget->settings.isTypeParallel())
      this->on_actionParallel_triggered();
    if (glWidget->settings.isTypeCentral()) this->on_actionCentral_triggered();

    if (glWidget->settings.getDisplayMethodVAP() == ONLY_VERTEX)
      on_actionOnly_vertex_triggered();
    if (glWidget->settings.getDisplayMethodVAP() == ONLY_POLYGON)
      on_actionOnly_edge_triggered();
    if (glWidget->settings.getDisplayMethodVAP() == MIX)
      on_actionMix_triggered();

    if (glWidget->settings.isLineEdge()) on_actionSolidLine_Edge_triggered();
    if (glWidget->settings.isDotsEdge()) on_actionLineDots_Edge_triggered();

    if (glWidget->settings.getDisplayMethodV() == MISSING)
      on_actionNone_triggered();
    if (glWidget->settings.getDisplayMethodV() == CIRCLE)
      on_actionCircle_triggered();
    if (glWidget->settings.getDisplayMethodV() == SQUERE)
      on_actionSquere_triggered();

    setMessageToLine(ui->actionSize_Edge, glWidget->settings.getSizeEdge(),
                     "Size :");
    setMessageToLine(ui->actionSize_Vertex, glWidget->settings.getSizeVertex(),
                     "Size :");
  }
}

void MainWindow::initializeGlWidget() {
  init = 1;
  glWidget = new GlView(this);

  glWidget->glData.facets_count = 0;
  glWidget->glData.vertex_count = 0;
  glWidget->glData.gif_start = 0;
  glWidget->glData.polygons = NULL;
  glWidget->glData.vertexes.matrix = NULL;
  ;

  glWidget->setParent(ui->glBox);

  QVBoxLayout *layout = new QVBoxLayout(ui->glBox);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(glWidget);
  ui->glBox->setLayout(layout);

  glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::setInfo(QString fileName) {
  ui->fileNameLbl->setText(fileName);
  ui->fileNameLbl->setVisible(true);

  QString vertexInfo_ =
      "Vertex Info: " + QString::number(glWidget->glData.vertex_count);
  ui->vertexInfo->setText(vertexInfo_);
  ui->vertexInfo->setVisible(true);

  QString edgeInfo_ =
      "Edge Info: " + QString::number(glWidget->glData.facets_count);
  ui->edgeInfo->setText(edgeInfo_);
  ui->edgeInfo->setVisible(true);
}

void MainWindow::on_fileBtn_clicked() {
  if (init)
    if (glWidget->glData.gif_start == 1) on_stopBtn_clicked();
  QString fileName = QFileDialog::getOpenFileName(
      nullptr, "Open OBJ File", QDir::homePath(), "OBJ Files (*.obj)");

  if (fileName.isEmpty()) {
    qDebug() << "Selected file doesn't exist" << fileName;
  } else {
    if (!init)
      initializeGlWidget();
    else
      free_data(&glWidget->glData);

    glWidget->setNormalization(true);

    QByteArray byteArray = fileName.toUtf8();
    char *filename = byteArray.data();

    glWidget->setCodeParser(parser(&glWidget->glData, filename));

    if (glWidget->getCodeParser() == PARSER_FALSE) {
      QMessageBox::critical(nullptr, "Vertex", "Bad coordinate");
      ui->menuSettings->setEnabled(false);
      ui->menuSave->setEnabled(false);
    } else {
      setInfo(filename);
      glWidget->resizeGL(ST_SIZE_WIDTH, ST_SIZE_HEIGHT);
      setNewSettingsToIU();
      glWidget->updateGL();
    }
  }
}

static double getStep(const QLineEdit *step) {
  QString text = step->text();
  bool ok;
  double value = text.toDouble(&ok);
  if (!ok) value = 0.1;
  return value;
}

void MainWindow::scaleHelp(Scaling scale) {
  if (checkCorrect()) {
    double step = getStep(ui->scaleStep);
    int code = affineScalingOperation(&glWidget->glData, step, scale);
    if (code == -1)
      showMessageError("Operation failed", "The scale must be non-zero");
    else if (code == 0)
      showMessageError("Operation failed", "Scalig");
    else {
      glWidget->updateGL();
      if (glWidget->glData.gif_start && glWidget->gifData_.count == MAX_COUNT)
        on_stopBtn_clicked();
    }
  } else {
    showMessageError(
        "Initialization",
        "Incorrect initialization. Scaling operation cannot be performed");
  }
}

void MainWindow::on_scaleMinus_clicked() { scaleHelp(DECREASE); }
void MainWindow::on_scalePlus_clicked() { scaleHelp(INCREASE); }

void MainWindow::operationRAM(Rotation rotation_, const bool mode,
                              const QLineEdit *line) {
  if (checkCorrect()) {
    double coef = getStep(line);
    int code = ERROR;
    if (mode)
      code = affineRotationOperation(&glWidget->glData, coef, rotation_);
    else
      code = affineMovingOperation(&glWidget->glData, coef, rotation_);

    if (code) {
      glWidget->updateGL();

      if (glWidget->glData.gif_start &&
          glWidget->gifData_.count == (MAX_COUNT + 1))
        on_stopBtn_clicked();
    } else
      showMessageError("Operation failed", "Rotation or Moving");

  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. Rotation or Moving operation "
                     "cannot be performed");
  }
}

void MainWindow::on_rotateXplus_clicked() {
  operationRAM(XP, true, ui->rotateStepX);
}
void MainWindow::on_rotateXminus_clicked() {
  operationRAM(XM, true, ui->rotateStepX);
}
void MainWindow::on_rotateYplus_clicked() {
  operationRAM(YP, true, ui->rotateStepY);
}
void MainWindow::on_rotateYminus_clicked() {
  operationRAM(YM, true, ui->rotateStepY);
}
void MainWindow::on_rotateZminus_clicked() {
  operationRAM(ZP, true, ui->rotateStepZ);
}
void MainWindow::on_rotateZplus_clicked() {
  operationRAM(ZM, true, ui->rotateStepZ);
}

void MainWindow::on_moveXleft_clicked() {
  operationRAM(XM, false, ui->moveStepX);
}

void MainWindow::on_moveXright_clicked() {
  operationRAM(XP, false, ui->moveStepX);
}

void MainWindow::on_moveYdown_clicked() {
  operationRAM(YM, false, ui->moveStepY);
}

void MainWindow::on_moveYup_clicked() {
  operationRAM(YP, false, ui->moveStepY);
}

void MainWindow::on_moveZfar_clicked() {
  operationRAM(ZM, false, ui->moveStepZ);
}

void MainWindow::on_moveZnear_clicked() {
  operationRAM(ZP, false, ui->moveStepZ);
}

void MainWindow::on_PB_Normalize_clicked() {
  if (checkCorrect()) {
    glWidget->setNormalization(true);
    glWidget->updateGL();
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. Normalization operation cannot "
                     "be performed");
  }
}

void MainWindow::setColorHelp(QColor current, ColorSetter setterMethod) {
  if (checkCorrect()) {
    QColor color = QColorDialog::getColor(current, this, "Select Color");
    (glWidget->settings.*setterMethod)(color);
    glWidget->updateGL();
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to perform "
                     "the color setting operation");
  }
}

void MainWindow::on_actionBackground_color_triggered() {
  setColorHelp(glWidget->settings.getBackgroundColor(),
               &Setting::setBackGroundColor);
}

void MainWindow::on_actionColor_Edge_triggered() {
  setColorHelp(glWidget->settings.getColorEdge(), &Setting::setColorEdge);
}

void MainWindow::on_actionColor_Vertex_triggered() {
  setColorHelp(glWidget->settings.getColorVertex(), &Setting::setColorVertex);
}

void MainWindow::helpTriggeredType(bool codeCentral = false,
                                   bool codeParallel = true) {
  ui->actionCentral->setChecked(codeCentral);
  ui->actionParallel->setChecked(codeParallel);
  if (checkCorrect()) {
    glWidget->settings.setTypeProjection(codeParallel, codeCentral);
    on_PB_Normalize_clicked();
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to "
                     "rearrange the projection");
  }
}

void MainWindow::on_actionParallel_triggered() { helpTriggeredType(); }

void MainWindow::on_actionCentral_triggered() {
  helpTriggeredType(true, false);
}

double MainWindow::getSize(bool &ok) {
  return QInputDialog::getDouble(this, tr("Enter size"), tr("size:"), 0.1, 0,
                                 100, 5, &ok);
}

// mode = true - edge
// mode = false - vertex
void MainWindow::helpTrigeredSize(bool mode = true) {
  if (checkCorrect()) {
    bool ok = false;
    double size = getSize(ok);
    if (ok) {
      QString info = ("Size" + QString(": %1").arg(size));
      if (mode) {
        ui->actionSize_Edge->setText(info);
        glWidget->settings.setSizeEdge(size);
      }
      if (!mode) {
        ui->actionSize_Vertex->setText(info);
        glWidget->settings.setSizeVertex(size);
      }

      glWidget->updateGL();
    }
  } else {
    showMessageError(
        "Initialization",
        "Incorrect initialization. It is not possible to set the size");
  }
}

void MainWindow::on_actionSize_Edge_triggered() { helpTrigeredSize(); }

void MainWindow::on_actionSize_Vertex_triggered() { helpTrigeredSize(false); }

void MainWindow::helpTriggeredTypeLine(const bool solid = true,
                                       const bool dots = false) {
  if (checkCorrect()) {
    glWidget->settings.setTypeEdge(solid, dots);
    ui->actionSolidLine_Edge->setChecked(solid);
    ui->actionLineDots_Edge->setChecked(dots);
    glWidget->updateGL();
  } else {
    showMessageError(
        "Initialization",
        "Incorrect initialization. It is not possible to change the line type");
  }
}

void MainWindow::on_actionSolidLine_Edge_triggered() {
  helpTriggeredTypeLine();
}

void MainWindow::on_actionLineDots_Edge_triggered() {
  helpTriggeredTypeLine(false, true);
}

void MainWindow::helpTriggeredDispTypeV(const DisplayMethodPoint dispM,
                                        const bool none_, const bool circle_,
                                        const bool squere_) {
  if (checkCorrect()) {
    glWidget->settings.setSettingsDisplayMethodV(dispM);
    ui->actionNone->setChecked(none_);
    ui->actionCircle->setChecked(circle_);
    ui->actionSquere->setChecked(squere_);
    glWidget->updateGL();
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to change "
                     "the Display method(Vertex)");
  }
}

void MainWindow::on_actionNone_triggered() {
  helpTriggeredDispTypeV(MISSING, true, false, false);
}

void MainWindow::on_actionCircle_triggered() {
  helpTriggeredDispTypeV(CIRCLE, false, true, false);
}

void MainWindow::on_actionSquere_triggered() {
  helpTriggeredDispTypeV(SQUERE, false, false, true);
}

void MainWindow::helpTriggeredDispTypeVAP(const DisplayMethod dispM,
                                          const bool edge, const bool vertex,
                                          const bool mix) {
  if (checkCorrect()) {
    glWidget->settings.setSettingsDisplayMethodVAP(dispM);

    ui->actionOnly_edge->setChecked(edge);
    ui->actionOnly_vertex->setChecked(vertex);
    ui->actionMix->setChecked(mix);
    if (glWidget->settings.getDisplayMethodV() == MISSING && vertex)
      on_actionCircle_triggered();
    glWidget->updateGL();
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to change "
                     "the Display method");
  }
}

void MainWindow::on_actionOnly_edge_triggered() {
  helpTriggeredDispTypeVAP(ONLY_POLYGON, true, false, false);
}

void MainWindow::on_actionOnly_vertex_triggered() {
  helpTriggeredDispTypeVAP(ONLY_VERTEX, false, true, false);
}

void MainWindow::on_actionMix_triggered() {
  helpTriggeredDispTypeVAP(MIX, false, false, true);
}

void MainWindow::on_actionSave_settings_triggered() {
  if (checkCorrect()) {
    bool ok = true;
    glWidget->settings.saveSetting(ok);
    if (!ok) showMessageError("Save", "Unable to save!");
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to perform "
                     "the save settings operation");
  }
}

void MainWindow::on_actionImage_triggered() {
  if (checkCorrect()) {
    QString filter = "All files (*.*)";
    QString filePath = QFileDialog::getSaveFileName(this, "Save Image",
                                                    QDir::homePath(), filter);

    if (!filePath.isEmpty()) {
      QFileInfo fileInfo(filePath);
      QString fileSuffix = fileInfo.completeSuffix().toLower();

      if (fileSuffix.isEmpty() || (QString::compare(fileSuffix, "jpg") == 0) ||
          (QString::compare(fileSuffix, "bmp") == 0)) {
        if (fileSuffix.isEmpty()) {
          QMessageBox::information(nullptr, "Suffix",
                                   "There is no file type, so the default type "
                                   "<.jpg> will be selected.");
          filePath += ".jpg";
        }

        glWidget->saveImage(filePath);
      } else
        showMessageError(
            "Image type",
            "Cannot save invalid type, need either <jpg> or <bmp>");

    } else
      showMessageError("Path image", "Path is empty.");

  } else
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to perform "
                     "the save image operation");
}

void MainWindow::on_actionGIF_triggered() {
  if (checkCorrect()) {
    if (glWidget->glData.gif_start == 0) {
      QString filePath = QFileDialog::getSaveFileName(
          this, "Save GIF", QDir::homePath(), "All files (*.*)");
      if (!filePath.isEmpty()) {
        glWidget->glData.gif_start = 1;
        QFileInfo fileInfo(filePath);

        QString fileSuffix = fileInfo.completeSuffix().toLower();

        if (QString::compare(fileSuffix, "gif") != 0)
          glWidget->gifData_.directoryPath = filePath + ".gif";
        else
          glWidget->gifData_.directoryPath = filePath;

        glWidget->gifData_.directoryPathBMP = fileInfo.path() + "/gif_frames/";
        QDir().mkpath(glWidget->gifData_.directoryPathBMP);

        glWidget->gifData_.count = 1;
        glWidget->gifData_.index = 0;

        ui->stopBtn->setVisible(true);
      }
    } else
      showMessageError("Gif", "GIF recording has already started");
  } else {
    showMessageError("Initialization",
                     "Incorrect initialization. It is not possible to perform "
                     "the save gif operation");
  }
}

void MainWindow::on_stopBtn_clicked() {
  ui->stopBtn->setVisible(false);
  glWidget->glData.gif_start = 0;
  glWidget->createGifFromBmpFiles();
  glWidget->gifData_.count = 1;
  glWidget->gifData_.index = 0;
}
