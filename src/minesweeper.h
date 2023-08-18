#ifndef SRC_MINESWEEPERGAME_H_
#define SRC_MINESWEEPERGAME_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <vector>

class MinesweeperGame {
 public:
  MinesweeperGame();

  static MinesweeperGame* instance;
  void NewGame();

  static void displayFuncWrapper();
  static void idleFuncWrapper();
  static void mouseFuncWrapper(int button, int state, int x, int y);

  void display();
  void idle();
  void mouse(int button, int state, int x, int y);

 private:
  struct Tcell {
    bool mine;
    bool flag;
    bool open;
    int mines_around;
  };

  bool IsCellInMap(int x, int y);
  void ScreenToOpenGL(int x_screen, int y_screen, float* x_opengl,
                      float* y_opengl);
  void OpenFields(int x, int y);
  void LeftMouseClick(int x, int y);
  void RightMouseClick(int x, int y);

  void ShowFlag();
  void ShowDigits(int digit);
  void ShowMine();
  void ShowField();
  void ShowFieldOpen();
  void ShowGame();

  static const int kMapW = 10;
  static const int kMapH = 10;
  static const int kCountMines = 15;
  Tcell map[kMapW][kMapH];

  int closed_cell_;
  bool failed_;
};

#endif  // SRC_MINESWEEPERGAME_H_