#include "glView.hpp"

void GlView::setCodeParser(const int &code) { codeParser = code; }
int GlView::getCodeParser() { return codeParser; }

void GlView::showErrorReadSettings() {
  QMessageBox::critical(nullptr, "Setting error",
                        "Unable to read configuration file!");
  QMessageBox::information(nullptr, "Setting", "Set standart settings");
}

void GlView::setNormalization(bool code) { isNormalization = code; }

GlView::GlView(QWidget *parent) : QOpenGLWidget(parent) {
  setNormalization(true);
  if (!settings.readSetting()) {
    showErrorReadSettings();
    settings = std::move(Setting());
    if (!settings.isNormalSettings()) settings.exitProgramError();
  }
  setWindowTitle("3DVIEWER");
}

double GlView::getParallelValue(const INDEX_ASIX index) {
  double value = 0;
  if (index == XMIN || index == YMIN || index == ZMIN)
    value = glData.scale[index] - ST_EXTRA_SIZE;
  if (index == XMAX || index == YMAX || index == ZMAX)
    value = glData.scale[index] + ST_EXTRA_SIZE;
  return value;
}

double GlView::getValueForAsixS(INDEX_ASIX asix) {
  double resValue = 0;
  if (settings.isTypeCentral()) {
    if (asix == XMIN) resValue = ST_LEFT;
    if (asix == XMAX) resValue = ST_RIGHT;
    if (asix == YMIN) resValue = ST_DOWN;
    if (asix == YMAX) resValue = ST_UP;
    if (asix == ZMAX) resValue = nearVal;
    if (asix == ZMIN) {
      resValue = fabs(glData.scale[ZMAX] - glData.scale[ZMIN]);
      resValue = ((resValue <= ST_MIN_FAR) ? ST_MIN_FAR : resValue) -
                 (getZAxisShift());
    }
  } else
    resValue = getParallelValue(asix);
  return resValue;
}

double GlView::getLeft() { return getValueForAsixS(XMIN); }

double GlView::getRight() { return getValueForAsixS(XMAX); }

double GlView::getDown() { return getValueForAsixS(YMIN); }

double GlView::getUp() { return getValueForAsixS(YMAX); }

double GlView::getNear() { return getValueForAsixS(ZMAX); }

double GlView::getFar() { return getValueForAsixS(ZMIN); }

double GlView::getZAxisShift() {
  double zMin = glData.scale[ZMIN];
  double shift = 0;
  if (zMin < getNear())
    shift = -(fabs(getNear() - zMin) + ST_SHIFT);
  else
    shift = -ST_SHIFT;
  return shift;
}

void GlView::performNormalization() {
  if (isNormalization && (codeParser == PARSER_OK)) {
    setFigureToCenter(&glData);

    setNewScale(&glData);

    if (settings.isTypeCentral()) {
      while (glData.scale[XMAX] > ST_RIGHT || glData.scale[YMAX] > ST_UP) {
        scaling(&glData, ST_SCALE_MINUS);
        setNewScale(&glData);
      }
    }
    isNormalization = false;
  }
}

void GlView::paintType() {
  if (codeParser == PARSER_OK) {
    performNormalization();

    if (settings.isTypeParallel())
      glOrtho(getLeft(), getRight(), getDown(), getUp(), getNear(), getFar());

    if (settings.isTypeCentral()) {
      glFrustum(getLeft(), getRight(), getDown(), getUp(), getNear(), getFar());
      glTranslatef(0, 0, this->getZAxisShift());
    }
  }
}

void GlView::initializeGL() {}

void GlView::resizeGL(int w, int h) {
  if (codeParser == PARSER_OK) glViewport(0, 0, w, h);
}

void GlView::helpDrawing(const TYPE_DISP type, const GLfloat *color) {
  for (int i = 0; (i < (int)glData.facets_count) && codeParser == PARSER_OK;
       i++) {
    if (type == POLYGON)
      glBegin(GL_LINE_LOOP);
    else
      glBegin(GL_POINTS);

    glColor3fv(color);

    for (int j = 0; j < ST_SIZE_COORDINATE; j++) {
      int index = glData.polygons[i].pInds[j];
      glVertex3f(glData.vertexes.matrix[index][X],
                 glData.vertexes.matrix[index][Y],
                 glData.vertexes.matrix[index][Z]);
    }
    glEnd();
  }
}

bool GlView::ColorToGLfloatArray(const QVector<double> &colorv, GLfloat *colorf,
                                 const size_t &sizecf) {
  bool code = false;
  if (colorf && sizecf == ST_SIZE_COLOR_WITHOUT_ALPHA &&
      colorv.size() >= ST_SIZE_COLOR_WITHOUT_ALPHA) {
    colorf[R] = colorv[R];
    colorf[G] = colorv[G];
    colorf[B] = colorv[B];
    code = true;
  }
  return code;
}

void GlView::drawing_polygon(const GLfloat *color) {
  if (settings.isLineEdge()) glEnable(GL_DEPTH_TEST);
  if (settings.isDotsEdge()) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xF0F0);
  }

  glLineWidth(settings.getSizeEdge());

  helpDrawing(POLYGON, color);

  if (settings.isDotsEdge()) glDisable(GL_LINE_STIPPLE);
  if (settings.isLineEdge()) glDisable(GL_DEPTH_TEST);
}

void GlView::drawing_point(const GLfloat *color) {
  if (settings.getDisplayMethodV() == CIRCLE) glEnable(GL_POINT_SMOOTH);
  if (settings.getDisplayMethodV() == SQUERE) glDisable(GL_POINT_SMOOTH);

  glPointSize(settings.getSizeVertex());

  helpDrawing(VERTEX, color);

  if (settings.getDisplayMethodV() == CIRCLE) glDisable(GL_POINT_SMOOTH);
}

void GlView::drawing() {
  GLfloat color[ST_SIZE_COLOR_WITHOUT_ALPHA];

  if (settings.getDisplayMethodVAP() == MIX ||
      settings.getDisplayMethodVAP() == ONLY_POLYGON) {
    if (ColorToGLfloatArray(settings.getColorEdgeV(), color,
                            ST_SIZE_COLOR_WITHOUT_ALPHA))
      drawing_polygon(color);
  }
  if ((settings.getDisplayMethodVAP() == MIX ||
       settings.getDisplayMethodVAP() == ONLY_VERTEX) &&
      settings.getDisplayMethodV() != MISSING) {
    if (ColorToGLfloatArray(settings.getColorVertexV(), color,
                            ST_SIZE_COLOR_WITHOUT_ALPHA))
      drawing_point(color);
  }
}

void GlView::setBackgroubColorToDisplay(const bool isSettingColor = true) {
  QVector<double> backgroundColor;
  if (isSettingColor)
    backgroundColor = settings.getBackgroundColorV();
  else
    backgroundColor = {0, 0, 0, 0};

  glClearColor(backgroundColor[R], backgroundColor[G], backgroundColor[B],
               backgroundColor[A]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlView::paintGL() {
  if (codeParser == PARSER_OK) {
    setBackgroubColorToDisplay();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    paintType();
    drawing();

    glMatrixMode(GL_MODELVIEW);
  }
}

void GlView::updateGL() {
  update();
  if (glData.gif_start) saveBMP();
}

void GlView::saveImage(const QString &fileName) {
  if (fileName.isEmpty()) return;
  QImage image(ST_WIDTH_PHOTO, ST_HEIGHT_PHOTO, QImage::Format_RGB32);
  image.fill(Qt::white);

  int offsetX = (ST_WIDTH_PHOTO - width()) / 2;
  int offsetY = (ST_HEIGHT_PHOTO - height()) / 2;

  QPainter painter(&image);
  painter.translate(offsetX, offsetY);
  render(&painter);

  image.save(fileName);
}

void GlView::saveBMP() {
  if (gifData_.count <= MAX_COUNT) {
    QString filePath =
        gifData_.directoryPathBMP + QString::number(gifData_.index) + ".bmp";
    gifData_.index++;
    gifData_.count++;
    saveImage(filePath);
  }
}

QFileInfoList GlView::getFileNameList(const QDir &dir, const QString &filter,
                                      bool &ok) {
  QStringList filters;
  filters << filter;
  QFileInfoList files = dir.entryInfoList(filters);

  if (files.isEmpty())
    ok = false;
  else {
    std::sort(files.begin(), files.end(),
              [](const QFileInfo &file1, const QFileInfo &file2) {
                QString name1 = file1.baseName();
                QString name2 = file2.baseName();
                return name1.toInt() < name2.toInt();
              });
  }
  return files;
}

void GlView::createGifFromBmpFiles() {
  if (!gifData_.directoryPathBMP.isEmpty() &&
      !gifData_.directoryPath.isEmpty()) {
    QDir directoryBmp(gifData_.directoryPathBMP);
    bool ok = true;
    QFileInfoList bmpFiles = getFileNameList(directoryBmp, "*.bmp", ok);

    if (ok) {
      QStringList arguments;
      arguments << "-delay" << "4";

      foreach (QFileInfo f, bmpFiles)
        arguments << gifData_.directoryPathBMP + f.fileName();

      arguments << gifData_.directoryPath;

      QMessageBox::information(nullptr, "GIF", "Saving has started");
      QProcess process;
      process.start("convert", arguments);
      if (!process.waitForStarted()) {
        QMessageBox::critical(nullptr,
                              "Error starting process:", process.errorString());
      } else {
        if (!process.waitForFinished())
          QMessageBox::critical(
              nullptr, "Command execution error:", process.errorString());
        else
          QMessageBox::information(nullptr, "GIF file saved",
                                   gifData_.directoryPath);
      }

    } else
      QMessageBox::critical(nullptr, "GIF faile", "No bmp files.");

    directoryBmp.removeRecursively();
  } else
    QMessageBox::critical(nullptr, "Directory path",
                          "No path to installation directory");
}

void GlView::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void GlView::mouseMoveEvent(QMouseEvent *mo) {
  if (codeParser == PARSER_OK) {
    xRot = 0.001 / M_PI * (mo->pos().y() - mPos.y());
    affineRotationOperation(&glData, fabs(xRot), ((xRot < 0) ? XM : XP));

    yRot = 0.001 / M_PI * (mo->pos().x() - mPos.x());
    affineRotationOperation(&glData, fabs(yRot), ((yRot < 0) ? YM : YP));
    updateGL();
  }
}