#include "settings.h"

void Setting::setSettingsEdge(const double &size, const QColor &color,
                              const bool &line, const bool &dots) {
  setTypeEdge(line, dots);
  setSizeEdge(size);
  setColorEdge(color);
}

void Setting::setSizeEdge(const double &size) { setSize(edge_.size, size); }

void Setting::setColorEdge(const QColor &color) {
  setColor(edge_.color, color, ST_COLOR_EDGE);
}

void Setting::setTypeEdge(const bool &line, const bool &dots) {
  edge_.line = line;
  edge_.dots = dots;
  if (!isNormalTypeEdge()) {
    edge_.line = ST_LINE;
    edge_.dots = ST_DOTS;
  }
}

void Setting::setSettingsVertex(const double &size, const QColor &color) {
  setSizeVertex(size);
  setColorVertex(color);
}

void Setting::setSizeVertex(const double &size) { setSize(vertex_.size, size); }

void Setting::setColorVertex(const QColor &color) {
  setColor(vertex_.color, color, ST_COLOR_VERTEX);
}

void Setting::setSettingsDisplayMethodV(const DisplayMethodPoint &method) {
  dispMethod_ = method;
  if (!isNormalMethodV()) dispMethod_ = (DisplayMethodPoint)ST_METHOD_POINT;
}

void Setting::setSettingsDisplayMethodVAP(const DisplayMethod &method) {
  dispVAP = method;
  if (!isNormalMethodVAP()) dispVAP = (DisplayMethod)ST_METHOD;
}

void Setting::setTypeProjection(const bool &codeTypeParallel,
                                const bool &codeTypeCentral) {
  typeProjectionCentral = codeTypeCentral;
  typeProjectionParallel = codeTypeParallel;

  if (!isNormalTypeProjection()) {
    typeProjectionCentral = ST_CENTRAL_TYPE;
    typeProjectionParallel = ST_PARALLEL_TYPE;
  }
}

void Setting::setBackGroundColor(const QColor &color) {
  setColor(BackgroundColor, color, ST_COLOR_BACKGROUND);
}

double Setting::getSizeEdge() { return edge_.size; }

QColor Setting::getColorEdge() { return edge_.color; }

QVector<double> Setting::getColorEdgeV() { return getColorVector(edge_.color); }

bool Setting::isLineEdge() { return edge_.line; }

bool Setting::isDotsEdge() { return edge_.dots; }

double Setting::getSizeVertex() { return vertex_.size; }

QColor Setting::getColorVertex() { return vertex_.color; }

QVector<double> Setting::getColorVertexV() {
  return getColorVector(vertex_.color);
}

DisplayMethodPoint Setting::getDisplayMethodV() { return dispMethod_; }

DisplayMethod Setting::getDisplayMethodVAP() { return dispVAP; }

int Setting::getDisplayMethodI() { return (int)dispMethod_; }

QColor Setting::getBackgroundColor() { return BackgroundColor; }

QVector<double> Setting::getBackgroundColorV() {
  return getColorVector(BackgroundColor);
}

bool Setting::isTypeCentral() { return typeProjectionCentral; }

bool Setting::isTypeParallel() { return typeProjectionParallel; }

bool Setting::isNormalSettings() const {
  bool code = true;
  code = code && isNormalTypeProjection();

  code = code && isNormalSize(edge_.size);
  code = code && isNormalColor(edge_.color);
  code = code && isNormalTypeEdge();

  code = code && isNormalMethodV();
  code = code && isNormalMethodVAP();

  code = code && isNormalSize(vertex_.size);
  code = code && isNormalColor(vertex_.color);

  code = code && isNormalColor(BackgroundColor);

  return code;
};

void Setting::exitProgramError() const {
  if (!this->isNormalSettings()) {
    QMessageBox::critical(nullptr, "Setting error",
                          "The settings were not configured correctly!");
    exit(EXIT_FAILURE);
  }
}

void Setting::saveSetting(bool &ok) {
  if (!isNormalSettings()) {
    ok = false;
    return;
  }

  QSettings settings_("SAVE", "Settings");
  settings_.beginGroup("Edge");
  settings_.setValue("Size", edge_.size);
  settings_.setValue("Color", edge_.color.name(QColor::HexRgb));
  settings_.setValue("Solid_line", edge_.line);
  settings_.setValue("Dotted_line", edge_.dots);
  settings_.endGroup();

  settings_.beginGroup("Vertex");
  settings_.setValue("Size", vertex_.size);
  settings_.setValue("Color", vertex_.color.name(QColor::HexRgb));
  settings_.endGroup();

  settings_.beginGroup("Type_Projection");
  settings_.setValue("Central", typeProjectionCentral);
  settings_.setValue("Parallel", typeProjectionParallel);
  settings_.endGroup();

  settings_.beginGroup("Display_Method_Point");
  settings_.setValue("Method", dispMethod_);
  settings_.endGroup();

  settings_.beginGroup("Display_Method");
  settings_.setValue("Method", dispVAP);
  settings_.endGroup();

  settings_.beginGroup("Background");
  settings_.setValue("Color", BackgroundColor.name(QColor::HexRgb));
  settings_.endGroup();
}

bool Setting::readSetting() {
  QSettings settings_("SAVE", "Settings");

  // Edge
  edge_.size = settings_.value("Edge/Size", ST_SIZE).toDouble();
  edge_.color = QColor(
      settings_
          .value("Edge/Color", ((QColor)ST_COLOR_EDGE).name(QColor::HexRgb))
          .toString());
  edge_.line = settings_.value("Edge/Solid_line", ST_LINE).toBool();
  edge_.dots = settings_.value("Edge/Dotted_line", ST_DOTS).toBool();

  // Vertex
  vertex_.size = settings_.value("Vertex/Size", ST_SIZE).toDouble();
  vertex_.color = QColor(
      settings_
          .value("Vertex/Color", ((QColor)ST_COLOR_VERTEX).name(QColor::HexRgb))
          .toString());

  // Display Method Point
  dispMethod_ = (DisplayMethodPoint)settings_
                    .value("Display_Method_Point/Method", ST_METHOD_POINT)
                    .toInt();

  // Display Method
  dispVAP = (DisplayMethod)settings_.value("Display_Method/Method", ST_METHOD)
                .toInt();

  // Background
  BackgroundColor =
      QColor(settings_
                 .value("Background/Color",
                        ((QColor)ST_COLOR_BACKGROUND).name(QColor::HexRgb))
                 .toString());

  // Type Projection
  typeProjectionCentral =
      settings_.value("Type_Projection/Central", ST_CENTRAL_TYPE).toBool();
  typeProjectionParallel =
      settings_.value("Type_Projection/Parallel", ST_PARALLEL_TYPE).toBool();

  return isNormalSettings();
}

void Setting::setSize(double &src, const double &size) {
  src = size;
  if (!isNormalSize(size)) src = ST_SIZE;
}

void Setting::setColor(QColor &src, const QColor &color,
                       const QColor color_error) {
  if (isNormalColor(color))
    src = color;
  else {
    if (!isNormalColor(src)) src = color_error;
  }
}

QVector<double> Setting::getColorVector(const QColor &SColor) {
  if (!isNormalColor(SColor)) return QVector<double>(ST_SIZE_COLOR, 0);

  QVector<double> Vcolor;
  Vcolor.push_back(SColor.redF());
  Vcolor.push_back(SColor.greenF());
  Vcolor.push_back(SColor.blueF());
  Vcolor.push_back(SColor.alphaF());

  return Vcolor;
}

bool Setting::isNormalTypeProjection() const {
  return (typeProjectionCentral != typeProjectionParallel);
}

bool Setting::isNormalSize(const double &size) const {
  return (size > ST_MIN_SIZE);
}

bool Setting::isNormalColor(const QColor &color) const {
  return (color.isValid());
}

bool Setting::isNormalTypeEdge() const { return (edge_.line != edge_.dots); }

bool Setting::isNormalMethodV() const {
  return (dispMethod_ >= MIN_METHODS && dispMethod_ <= MAX_METHODS_P);
}

bool Setting::isNormalMethodVAP() const {
  return (dispMethod_ >= MIN_METHODS && dispMethod_ <= MAX_METHODS);
}