#include "minesweeper.h"

#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

MinesweeperGame* MinesweeperGame::instance = nullptr;

MinesweeperGame::MinesweeperGame()
    : closed_cell_(kMapW * kMapH), failed_(false) {
  srand(time(NULL));
  NewGame();
}

void MinesweeperGame::displayFuncWrapper() {
  if (instance) instance->display();
}

void MinesweeperGame::idleFuncWrapper() {
  if (instance) instance->idle();
}

void MinesweeperGame::mouseFuncWrapper(int button, int state, int x, int y) {
  if (instance) instance->mouse(button, state, x, y);
}

void MinesweeperGame::display() {
  glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (kCountMines == closed_cell_) {
    NewGame();
  }

  ShowGame();

  glutSwapBuffers();
}

void MinesweeperGame::idle() {
  glutPostRedisplay();
  usleep(1000);
}

void MinesweeperGame::mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    LeftMouseClick(x, y);
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    RightMouseClick(x, y);
  }
}

bool MinesweeperGame::IsCellInMap(int x, int y) {
  return (x >= 0 && y >= 0 && x < kMapW && y < kMapH);
}

void MinesweeperGame::ScreenToOpenGL(int x_screen, int y_screen,
                                     float* x_opengl, float* y_opengl) {
  *x_opengl = 2.0 * x_screen / glutGet(GLUT_WINDOW_WIDTH) - 1.0;
  *y_opengl = 1.0 - 2.0 * y_screen / glutGet(GLUT_WINDOW_HEIGHT);
}

void MinesweeperGame::OpenFields(int x, int y) {
  if (!IsCellInMap(x, y) || map[x][y].open) return;

  map[x][y].open = true;
  closed_cell_--;

  if (map[x][y].mines_around == 0) {
    for (int dx = -1; dx < 2; dx++) {
      for (int dy = -1; dy < 2; dy++) {
        OpenFields(x + dx, y + dy);
      }
    }
  }

  if (map[x][y].mine) {
    failed_ = true;
    for (int i = 0; i < kMapH; i++) {
      for (int j = 0; j < kMapW; j++) {
        map[j][i].open = true;
      }
    }
  }
}

void MinesweeperGame::LeftMouseClick(int x, int y) {
  if (failed_) {
    NewGame();
  } else {
    float x_opengl, y_opengl;
    ScreenToOpenGL(x, y, &x_opengl, &y_opengl);

    int cell_x = (int)((x_opengl + 1.0) * kMapW * 0.5);
    int cell_y = (int)((y_opengl + 1.0) * kMapH * 0.5);

    if (IsCellInMap(cell_x, cell_y) && !map[cell_x][cell_y].flag) {
      OpenFields(cell_x, cell_y);
    }
  }
}

void MinesweeperGame::RightMouseClick(int x, int y) {
  float x_opengl, y_opengl;
  ScreenToOpenGL(x, y, &x_opengl, &y_opengl);

  int cell_x = (int)((x_opengl + 1.0) * kMapW * 0.5);
  int cell_y = (int)((y_opengl + 1.0) * kMapH * 0.5);

  if (IsCellInMap(cell_x, cell_y)) {
    map[cell_x][cell_y].flag = !map[cell_x][cell_y].flag;
  }
}

void MinesweeperGame::NewGame() {
  closed_cell_ = kMapW * kMapH;
  failed_ = false;

  for (int i = 0; i < kMapW; i++) {
    for (int j = 0; j < kMapH; j++) {
      map[i][j].mine = false;
      map[i][j].flag = false;
      map[i][j].open = false;
      map[i][j].mines_around = 0;
    }
  }

  for (int i = 0; i < kCountMines; i++) {
    int x = rand() % kMapW;
    int y = rand() % kMapH;

    if (map[x][y].mine) {
      i--;
    } else {
      map[x][y].mine = true;

      for (int dx = -1; dx < 2; dx++) {
        for (int dy = -1; dy < 2; dy++) {
          if (IsCellInMap(x + dx, y + dy)) {
            map[x + dx][y + dy].mines_around += 1;
          }
        }
      }
    }
  }
}

void MinesweeperGame::ShowFlag() {
  std::vector<float> vertices = {0.25, 0.75, 0.85, 0.5, 0.25, 0.25};

  std::vector<float> line_vertices = {0.25, 0.75, 0.25, 0.0};

  glColor3f(1, 0, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices.data());
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
  glDisableClientState(GL_VERTEX_ARRAY);

  glLineWidth(5);
  glColor3f(0, 0, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, line_vertices.data());
  glDrawArrays(GL_LINES, 0, line_vertices.size() / 2);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void MinesweeperGame::ShowDigits(int digit) {
  auto Line = [](float x1, float y1, float x2, float y2) {
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
  };

  glLineWidth(3);
  glColor3f(1, 1, 0);
  glBegin(GL_LINES);

  if ((digit != 1) && (digit != 4)) Line(0.3, 0.85, 0.7, 0.85);
  if ((digit != 0) && (digit != 1) && (digit != 7)) Line(0.3, 0.5, 0.7, 0.5);
  if ((digit != 1) && (digit != 4) && (digit != 7)) Line(0.3, 0.15, 0.7, 0.15);

  if ((digit != 5) && (digit != 6)) Line(0.7, 0.5, 0.7, 0.85);
  if (digit != 2) Line(0.7, 0.5, 0.7, 0.15);

  if ((digit != 1) && (digit != 2) && (digit != 3) && (digit != 7))
    Line(0.3, 0.5, 0.3, 0.85);
  if ((digit == 0) || (digit == 2) || (digit == 6) || (digit == 8))
    Line(0.3, 0.5, 0.3, 0.15);

  glEnd();
}

void MinesweeperGame::ShowMine() {
  std::vector<float> vertices = {0.3, 0.3, 0.7, 0.3, 0.7, 0.7, 0.3, 0.7};

  glColor3f(0, 0, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices.data());
  glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void MinesweeperGame::ShowField() {
  glBegin(GL_TRIANGLE_STRIP);

  glColor3f(0.8, 0.8, 0.8);
  glVertex2f(0, 1);
  glColor3f(0.7, 0.7, 0.7);
  glVertex2f(1, 1);
  glVertex2f(0, 0);
  glColor3f(0.6, 0.6, 0.6);
  glVertex2f(1, 0);

  glEnd();

  glLineWidth(1);
  glColor3f(0.5, 0.5, 0.5);

  glBegin(GL_LINE_LOOP);

  glVertex2f(0, 0);
  glVertex2f(1, 0);
  glVertex2f(1, 1);
  glVertex2f(0, 1);

  glEnd();
}

void MinesweeperGame::ShowFieldOpen() {
  std::vector<float> vertices = {0, 1, 1, 1, 0, 0, 1, 0};

  glColor3f(0.3, 0.7, 0.3);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices.data());
  glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 2);
  glDisableClientState(GL_VERTEX_ARRAY);

  glLineWidth(1);
  glColor3f(0.5, 0.5, 0.5);

  glBegin(GL_LINE_LOOP);

  glVertex2f(0, 0);
  glVertex2f(1, 0);
  glVertex2f(1, 1);
  glVertex2f(0, 1);

  glEnd();
}

void MinesweeperGame::ShowGame() {
  glLoadIdentity();
  glScalef(2.0 / kMapW, 2.0 / kMapH, 1);
  glTranslatef(-kMapW * 0.5, -kMapH * 0.5, 0);

  for (int i = 0; i < kMapH; i++) {
    for (int j = 0; j < kMapW; j++) {
      glPushMatrix();
      glTranslatef(j, i, 0);

      if (map[j][i].open) {
        ShowFieldOpen();

        if (map[j][i].mine) {
          ShowMine();
        } else if (map[j][i].mines_around > 0) {
          ShowDigits(map[j][i].mines_around);
        }
      } else {
        ShowField();
        if (map[j][i].flag) {
          ShowFlag();
        }
      }

      glPopMatrix();
    }
  }
}
