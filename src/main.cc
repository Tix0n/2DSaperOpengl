#include "minesweeper.h"


int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Minesweeper Game");

  MinesweeperGame game;
  MinesweeperGame::instance = &game;

  game.NewGame();

  glutDisplayFunc(MinesweeperGame::displayFuncWrapper);
  glutIdleFunc(MinesweeperGame::idleFuncWrapper);
  glutMouseFunc(MinesweeperGame::mouseFuncWrapper);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glutMainLoop();
  return 0;
}
