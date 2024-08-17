#pragma once

#include <QColorDialog>
#include <QFileInfoList>
#include <QImageWriter>
#include <QInputDialog>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QPoint>
#include <QProcess>
#include <QVector>

#include "../parser/parser.h"
#include "../transform/transform.h"
#include "settings.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class GlView;
}

QT_END_NAMESPACE

constexpr int MAX_COUNT = 120;
struct GifData {
  QString directoryPath;
  QString directoryPathBMP;
  int count;
  int index;
};

class GlView : public QOpenGLWidget {
 private:
  static constexpr double ST_SHIFT = 5.0;
  static constexpr int ST_LEFT = -1;
  static constexpr int ST_RIGHT = 1;
  static constexpr int ST_UP = 1;
  static constexpr int ST_DOWN = -1;
  static constexpr int ST_MIN_FAR = 20;
  static constexpr int ST_EXTRA_SIZE = 10;
  static constexpr double ST_SCALE_MINUS = 0.95;

  static constexpr int ST_SIZE_COLOR_WITHOUT_ALPHA = 3;
  static constexpr int ST_SIZE_COLOR = 4;

  static constexpr int ST_WIDTH_PHOTO = 640;
  static constexpr int ST_HEIGHT_PHOTO = 480;

  enum INDEX_ASIX {
    XMIN = 0,
    XMAX = 1,
    YMIN = 2,
    YMAX = 3,
    ZMIN = 4,
    ZMAX = 5
  };

  enum INDEX_COLOR { R = 0, G = 1, B = 2, A = 3 };

  enum TYPE_DISP { VERTEX, POLYGON };

 private:
  bool isNormalization;
  int codeParser = PARSER_FALSE;

  double nearVal = 2;

  double xRot, yRot, zRot;
  QPoint mPos;

 public:
  Setting settings;
  Data glData;
  GifData gifData_;

  // Constructor
  GlView(QWidget *parent = nullptr);

  // Destructor
  ~GlView() { free_data(&glData); };

  // Method
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void setNormalization(bool code);
  void updateGL();
  void saveImage(const QString &fileName);
  void createGifFromBmpFiles();
  void saveBMP();
  // void removeGifSource();
  void setCodeParser(const int &code);
  int getCodeParser();
  void setBackgroubColorToDisplay(const bool isSettingColor);

 private:
  double getValueForAsixS(INDEX_ASIX asix);
  double getLeft();
  double getRight();
  double getDown();
  double getUp();
  double getNear();
  double getFar();
  double getZAxisShift();
  void performNormalization();
  void showErrorReadSettings();

  void paintType();
  void drawing();
  void drawing_polygon(const GLfloat *color);
  void drawing_point(const GLfloat *color);
  void helpDrawing(const TYPE_DISP type, const GLfloat *color);

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  QFileInfoList getFileNameList(const QDir &dir, const QString &filter,
                                bool &ok);
  double getParallelValue(const INDEX_ASIX index);
  bool ColorToGLfloatArray(const QVector<double> &colorv, GLfloat *colorf,
                           const size_t &sizecf);
};
