//
// Created by caracal on 11/1/23.
//

#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QTimer>
#include "LoginWidget.hpp"

std::string LoginWidget::getUsername() {
  return this->username;
}

std::string LoginWidget::getPassword() {
  return this->password;
}

LoginWidget::LoginWidget(QWidget *parent) {
  auto *layout = new QGridLayout(this);

  auto *title = new QLabel("Password-Safe", this);

  QFont font = title->font();
  font.setPointSize(20);
  title->setFont(font);
  title->setAlignment(Qt::AlignCenter);

  auto *usernameLabel = new QLabel("Username", this);
  auto *usernameInput = new QLineEdit(this);

  auto *passwordLabel = new QLabel("Password", this);
  auto *passwordInput = new QLineEdit(this);

  passwordInput->setEchoMode(QLineEdit::Password);

  auto *loginButton = new QPushButton("Login", this);
  auto *registerButton = new QPushButton("Register", this);

  auto *icl = new QLabel("", this);
  icl->setFixedHeight(20);

  layout->addWidget(title, 0, 0, 1, 2);
  layout->addWidget(usernameLabel, 1, 0);
  layout->addWidget(usernameInput, 1, 1);
  layout->addWidget(passwordLabel, 2, 0);
  layout->addWidget(passwordInput, 2, 1);
  layout->addWidget(loginButton, 3, 0, 1, 2);
  layout->addWidget(registerButton, 4, 0, 1, 2);
  layout->addWidget(icl, 5, 0, 1, 2);

  setLayout(layout);

  connect(usernameInput, &QLineEdit::returnPressed, this, [=, this]() {
    this->password = passwordInput->text().toStdString();
    this->username = usernameInput->text().toStdString();

    emit loginButtonClicked();
  });

  connect(passwordInput, &QLineEdit::returnPressed, this, [=, this]() {
    this->password = passwordInput->text().toStdString();
    this->username = usernameInput->text().toStdString();

    emit loginButtonClicked();
  });

  connect(loginButton, &QPushButton::clicked, this, [=, this]() {
    this->password = passwordInput->text().toStdString();
    this->username = usernameInput->text().toStdString();

    emit loginButtonClicked();
  });

  connect(registerButton, &QPushButton::clicked, this, [=, this]() {
    this->password = passwordInput->text().toStdString();
    this->username = usernameInput->text().toStdString();

    emit registerButtonClicked();
  });
}

void LoginWidget::notifyText(const std::string &message, const std::string &color) {
  auto *ol = findChild<QLabel *>("icl");
  if (ol) {
    ol->deleteLater();
  }

  auto *icl = new QLabel(message.c_str(), this);
  icl->setObjectName("icl");

  QFont font = icl->font();
  font.setPointSize(10);
  icl->setFont(font);

  icl->setFixedHeight(20);
  icl->setAlignment(Qt::AlignCenter);

  std::string colorString = "QLabel { color : " + color + "; }";
  icl->setStyleSheet(colorString.c_str());

  auto *layout = dynamic_cast<QGridLayout *>(this->layout());
  layout->addWidget(icl, 5, 0, 1, 2);

  auto *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [icl, timer]() {
    icl->deleteLater();
    timer->deleteLater();
  });
  timer->start(5000);
}


LoginWidget::~LoginWidget() = default;