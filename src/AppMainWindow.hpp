//
// Created by caracal on 11/1/23.
//

#ifndef PASSWORDMANAGERQT6_APPMAINWINDOW_HPP
#define PASSWORDMANAGERQT6_APPMAINWINDOW_HPP


#include <QMainWindow>

class AppMainWindow : public QMainWindow {
Q_OBJECT

public:
  explicit AppMainWindow(QWidget *parent = nullptr);

  ~AppMainWindow() override;

  std::string getUsername();

  std::string getPassword();

private:
  std::string username;
  std::string password;

  void loginScreen();

  void passwordScreen();
};


#endif //PASSWORDMANAGERQT6_APPMAINWINDOW_HPP
