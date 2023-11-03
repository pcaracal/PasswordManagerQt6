//
// Created by caracal on 11/1/23.
//

#include <QApplication>
#include <QLabel>
#include <iostream>
#include "AppMainWindow.hpp"
#include "StringUtil.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  auto *window = new AppMainWindow();
  window->show();

  return QApplication::exec();
}