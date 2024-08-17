#pragma once

#include <QColorDialog>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QVector>

QT_BEGIN_NAMESPACE

namespace Ui {
class Setting;
}

QT_END_NAMESPACE

#define ST_COLOR_EDGE Qt::black
#define ST_COLOR_VERTEX Qt::red
#define ST_COLOR_BACKGROUND Qt::white

enum DisplayMethodPoint { MISSING = 0, CIRCLE = 1, SQUERE = 2 };
enum DisplayMethod { ONLY_VERTEX = 0, ONLY_POLYGON = 1, MIX = 2 };

constexpr double ST_MIN_SIZE = 0.0000001;
constexpr double ST_SIZE = 0.1;

constexpr int MIN_METHODS = 0;
constexpr int MAX_METHODS_P = 3;
constexpr int MAX_METHODS = 3;

constexpr int ST_METHOD_POINT = (int)MISSING;
constexpr int ST_METHOD = (int)MIX;

constexpr bool ST_LINE = true;
constexpr bool ST_DOTS = false;

constexpr bool ST_PARALLEL_TYPE = true;
constexpr bool ST_CENTRAL_TYPE = false;

class Setting {
 private:
  static constexpr int ST_SIZE_COLOR = 4;

  struct Edge {
    double size;
    QColor color;
    bool line;
    bool dots;
    Edge()
        : size(ST_SIZE), color(ST_COLOR_EDGE), line(ST_LINE), dots(ST_DOTS) {}
    Edge(double size_, QColor color_)
        : size(size_), color(color_), line(ST_LINE), dots(ST_DOTS) {}
  };

  struct Vertex {
    QColor color;
    double size;
    Vertex() : color(ST_COLOR_VERTEX), size(ST_SIZE) {}
    Vertex(const double size_, const QColor color_)
        : color(color_), size(size_) {}
  };

  bool typeProjectionParallel = ST_PARALLEL_TYPE;
  bool typeProjectionCentral = ST_CENTRAL_TYPE;

  Edge edge_;
  DisplayMethodPoint dispMethod_ = (DisplayMethodPoint)ST_METHOD_POINT;
  DisplayMethod dispVAP = (DisplayMethod)ST_METHOD;
  Vertex vertex_;

  QColor BackgroundColor = ST_COLOR_BACKGROUND;

 public:
  Setting() = default;
  ~Setting(){};

  void setSettingsEdge(const double &size, const QColor &color,
                       const bool &line, const bool &dots);
  void setSizeEdge(const double &size);
  void setColorEdge(const QColor &color);
  void setTypeEdge(const bool &line, const bool &dots);

  void setSettingsVertex(const double &size, const QColor &color);
  void setSizeVertex(const double &size);
  void setColorVertex(const QColor &color);

  void setSettingsDisplayMethodV(const DisplayMethodPoint &method);
  void setSettingsDisplayMethodVAP(const DisplayMethod &method);

  void setTypeProjection(const bool &codeTypeParallel,
                         const bool &codeTypeCentral);

  void setBackGroundColor(const QColor &color);

  double getSizeEdge();
  QColor getColorEdge();
  QVector<double> getColorEdgeV();
  bool isLineEdge();
  bool isDotsEdge();

  double getSizeVertex();
  QColor getColorVertex();
  QVector<double> getColorVertexV();

  DisplayMethodPoint getDisplayMethodV();
  DisplayMethod getDisplayMethodVAP();

  int getDisplayMethodI();

  QColor getBackgroundColor();
  QVector<double> getBackgroundColorV();

  bool isTypeCentral();
  bool isTypeParallel();

  bool isNormalSettings() const;

  void exitProgramError() const;

  void saveSetting(bool &ok);
  bool readSetting();

 private:
  QVector<double> getColorVector(const QColor &SColor);
  bool isNormalTypeProjection() const;
  bool isNormalSize(const double &size) const;
  bool isNormalColor(const QColor &color) const;
  bool isNormalTypeEdge() const;
  bool isNormalMethodV() const;
  bool isNormalMethodVAP() const;

  void setSize(double &src, const double &size);
  void setColor(QColor &src, const QColor &color, const QColor color_error);
};