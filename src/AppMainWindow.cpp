//
// Created by caracal on 11/1/23.
//
#include <nlohmann/json.hpp>
#include <QLabel>
#include <iostream>
#include "AppMainWindow.hpp"
#include "LoginWidget.hpp"
#include "StringUtil.hpp"
#include "FileUtil.hpp"
#include "PasswordWidget.hpp"

using json = nlohmann::json;

std::string DEVELOPMENT_DATA_PATH = "data.json";

AppMainWindow::AppMainWindow(QWidget *parent) {
  setWindowTitle("Gato");
  setFixedSize(800, 300);

  loginScreen();
}

AppMainWindow::~AppMainWindow() = default;

void AppMainWindow::passwordScreen() {
  auto *passwordScreen = new PasswordWidget(this, this->username, this->password, DEVELOPMENT_DATA_PATH);
  this->setCentralWidget(passwordScreen);
}

void AppMainWindow::loginScreen() {
  auto *login = new LoginWidget(this);
  this->setCentralWidget(login);

  connect(login, &LoginWidget::loginButtonClicked, this, [=, this]() {
    this->password = login->getPassword();
    this->username = login->getUsername();

    std::string tmp = FileUtil::read_file(DEVELOPMENT_DATA_PATH);
    if (tmp.empty() || tmp == "-1") {
      login->notifyText("Data file empty or doesn't exist, can't login", "red");
      return;
    }

    json data = FileUtil::read_json(DEVELOPMENT_DATA_PATH);

    if (data == nullptr) {
      login->notifyText("Data file not found", "red");
      return;
    }

    std::string username_salt = data["username_salt"];
    std::string password_salt = data["password_salt"];

    if (data["username"] == StringUtil::sha3_512(this->username + username_salt) &&
        data["password"] == StringUtil::sha3_512(this->password + password_salt)) {
      login->notifyText("Login success", "green");
      login->deleteLater();
      passwordScreen();
    } else {
      login->notifyText("Invalid login credentials", "red");
    }
  });

  connect(login, &LoginWidget::registerButtonClicked, this, [=, this]() {
    this->password = login->getPassword();
    this->username = login->getUsername();

    if (this->password.empty() || this->username.empty()) {
      login->notifyText("Username or password can't be empty", "red");
      return;
    }

    std::string tmp = FileUtil::read_file(DEVELOPMENT_DATA_PATH);
    if (!tmp.empty() && tmp != "-1") {
      login->notifyText("Data file not empty, can't register", "red");
      return;
    } else {
      login->notifyText("Register success", "green");
    }

    std::string username_salt = StringUtil::generate_salt();
    std::string password_salt = StringUtil::generate_salt();

    json data;
    data["username"] = StringUtil::sha3_512(this->username + username_salt);
    data["password"] = StringUtil::sha3_512(this->password + password_salt);
    data["username_salt"] = username_salt;
    data["password_salt"] = password_salt;

    FileUtil::write_json(DEVELOPMENT_DATA_PATH, data);

    login->deleteLater();
    passwordScreen();
  });
}


std::string AppMainWindow::getUsername() {
  return this->username;
}

std::string AppMainWindow::getPassword() {
  return this->password;
}