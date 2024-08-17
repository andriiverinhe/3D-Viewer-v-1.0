#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QColorDialog>
#include <QDoubleValidator>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QRadioButton>
#include <QVBoxLayout>

#include "../parser/parser.h"
#include "../transform/transform.h"
#include "glView.hpp"
#include "settings.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
 private slots:
  void on_fileBtn_clicked();

  void on_scaleMinus_clicked();

  void on_scalePlus_clicked();

  void on_rotateXplus_clicked();

  void on_rotateXminus_clicked();

  void on_rotateYplus_clicked();

  void on_rotateYminus_clicked();

  void on_rotateZminus_clicked();

  void on_rotateZplus_clicked();

  void on_moveXleft_clicked();

  void on_moveXright_clicked();

  void on_moveYdown_clicked();

  void on_moveYup_clicked();

  void on_moveZfar_clicked();

  void on_moveZnear_clicked();

  void on_PB_Normalize_clicked();

  void on_actionBackground_color_triggered();

  void on_actionParallel_triggered();

  void on_actionCentral_triggered();

  void on_actionSize_Edge_triggered();

  void on_actionColor_Edge_triggered();

  void on_actionSolidLine_Edge_triggered();

  void on_actionLineDots_Edge_triggered();

  void on_actionSize_Vertex_triggered();

  void on_actionColor_Vertex_triggered();

  void on_actionNone_triggered();

  void on_actionCircle_triggered();

  void on_actionSquere_triggered();

  void on_actionOnly_edge_triggered();

  void on_actionOnly_vertex_triggered();

  void on_actionMix_triggered();

  void on_actionSave_settings_triggered();

  void on_actionImage_triggered();

  void on_actionGIF_triggered();

  void on_stopBtn_clicked();

  void setInfo(QString fileName);

 private:
  void initStep();
  void setLineValidator(QLineEdit *line, const double &min, const double &max);
  void setNewSettingsToIU();
  void scaleHelp(Scaling scale);
  void helpTriggeredType(bool codeCentral, bool codeParallel);
  void helpTrigeredSize(bool mood);
  void helpTriggeredTypeLine(const bool solid, const bool dots);
  void helpTriggeredDispTypeV(const DisplayMethodPoint dispM, const bool none_,
                              const bool circle_, const bool squere_);
  void helpTriggeredDispTypeVAP(const DisplayMethod dispM, const bool edge,
                                const bool vertex, const bool mix);
  void operationRAM(Rotation rotation_, const bool mode, const QLineEdit *line);
  void initializeGlWidget();
  double getSize(bool &ok);
  bool checkCorrect();
  void showMessageError(const QString &title, const QString &message);

 private:
  int init;
  int prev_value_x;
  int prev_value_y;
  int prev_value_z;
  static constexpr int ST_SIZE_WIDTH = 800;
  static constexpr int ST_SIZE_HEIGHT = 700;
  Ui::MainWindow *ui;
  GlView *glWidget;
  using ColorSetter = void (Setting::*)(const QColor &);
  void setColorHelp(QColor current, ColorSetter setterMethod);
};

#endif  // MAINWINDOW_HPP
