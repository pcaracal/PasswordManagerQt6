//
// Created by caracal on 11/2/23.
//

#include <nlohmann/json.hpp>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "PasswordWidget.hpp"
#include "FileUtil.hpp"
#include "StringUtil.hpp"

using json = nlohmann::json;

PasswordWidget::PasswordWidget(QWidget *parent, const std::string &username, const std::string &password,
                               const std::string &dataFilePath) {
  this->username = username;
  this->password = password;
  this->dataFilePath = dataFilePath;
  auto *layout = new QGridLayout(this);

  auto *addItemButton = new QPushButton("Add Item", this);
  layout->addWidget(addItemButton, 1, 0, 1, 2);
  addItemButton->setObjectName("addItemButton");

  connect(addItemButton, &QPushButton::clicked, this, [=, this]() {
    createNewItem();
  });

  createList();
}

PasswordWidget::~PasswordWidget() = default;

void PasswordWidget::createList() {
  auto *layout = dynamic_cast<QGridLayout *>(this->layout());

  if (layout->itemAtPosition(0, 0) != nullptr) {
    delete layout->itemAtPosition(0, 0)->widget();
  }

  auto *list = new QListWidget(this);
  list->setObjectName("list");

  json j = FileUtil::read_json(this->dataFilePath);
  for (auto &item: j["data"]) {
    std::string name = StringUtil::aes_decrypt(item["name"], this->password);
    name = name.substr(0, name.size() - 128);

    list->addItem(QString::fromStdString(name));
  }

  connect(list, &QListWidget::itemClicked, this, [=, this](QListWidgetItem *item) {
    editItem(list->row(item));
  });

  layout->addWidget(list, 0, 0, 1, 2);
  setLayout(layout);
}

void PasswordWidget::editItem(const int &index) {
  auto *layout = dynamic_cast<QGridLayout *>(this->layout());
  findChild<QPushButton *>("addItemButton")->hide();

  for (int i = 0; i < layout->count(); ++i) {
    if (layout->itemAt(i)->widget()->objectName() != "list") {
      if (layout->itemAt(i)->widget()->objectName() != "addItemButton") {
        layout->itemAt(i)->widget()->deleteLater();
      }
    }
  }

  std::string oName, oUsername, oPassword;
  json j = FileUtil::read_json(this->dataFilePath);
  oName = StringUtil::aes_decrypt(j["data"][index]["name"], this->password);
  oName = oName.substr(0, oName.size() - 128);

  oUsername = StringUtil::aes_decrypt(j["data"][index]["username"], this->password);
  oUsername = oUsername.substr(0, oUsername.size() - 128);

  oPassword = StringUtil::aes_decrypt(j["data"][index]["password"], this->password);
  oPassword = oPassword.substr(0, oPassword.size() - 128);


  auto *nameLabel = new QLabel("Name", this);
  auto *nameInput = new QLineEdit(this);
  nameInput->setText(QString::fromStdString(oName));

  auto *usernameLabel = new QLabel("Username", this);
  auto *usernameInput = new QLineEdit(this);
  usernameInput->setText(QString::fromStdString(oUsername));

  auto *passwordLabel = new QLabel("Password", this);
  auto *passwordInput = new QLineEdit(this);
  passwordInput->setText(QString::fromStdString(oPassword));

  auto *saveButton = new QPushButton("Save", this);
  auto *cancelButton = new QPushButton("Cancel", this);
  auto *deleteButton = new QPushButton("Delete", this);

  layout->addWidget(nameLabel, 2, 0, 1, 1);
  layout->addWidget(nameInput, 2, 1, 1, 1);
  layout->addWidget(usernameLabel, 3, 0, 1, 1);
  layout->addWidget(usernameInput, 3, 1, 1, 1);
  layout->addWidget(passwordLabel, 4, 0, 1, 1);
  layout->addWidget(passwordInput, 4, 1, 1, 1);
  layout->addWidget(saveButton, 5, 1, 1, 1);
  layout->addWidget(cancelButton, 5, 0, 1, 1);
  layout->addWidget(deleteButton, 6, 0, 1, 2);

  connect(saveButton, &QPushButton::clicked, this, [=, this]() {
    save_edit(index);
  });
  connect(cancelButton, &QPushButton::clicked, this, [=, this]() {
    deleteButton->deleteLater();
    deleteInput();
  });
  connect(deleteButton, &QPushButton::clicked, this, [=, this]() {
    json j = FileUtil::read_json(this->dataFilePath);
    j["data"].erase(j["data"].begin() + index);
    FileUtil::write_json(this->dataFilePath, j);

    createList();
    deleteButton->deleteLater();
    deleteInput();
  });

  connect(nameInput, &QLineEdit::returnPressed, this, [=, this]() {
    save_edit(index);
  });
  connect(usernameInput, &QLineEdit::returnPressed, this, [=, this]() {
    save_edit(index);
  });
  connect(passwordInput, &QLineEdit::returnPressed, this, [=, this]() {
    save_edit(index);
  });
}

void PasswordWidget::save_edit(const int &index) {
  auto *layout = dynamic_cast<QGridLayout *>(this->layout());

  auto *nameInput = dynamic_cast<QLineEdit *>(layout->itemAtPosition(2, 1)->widget());
  auto *usernameInput = dynamic_cast<QLineEdit *>(layout->itemAtPosition(3, 1)->widget());
  auto *passwordInput = dynamic_cast<QLineEdit *>(layout->itemAtPosition(4, 1)->widget());
  auto *deleteButton = dynamic_cast<QPushButton *>(layout->itemAtPosition(6, 0)->widget());

  std::string nName = nameInput->text().toStdString();
  std::string nUsername = usernameInput->text().toStdString();
  std::string nPassword = passwordInput->text().toStdString();

  std::string unixMillis = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count());

  json j = FileUtil::read_json(this->dataFilePath);
  j["data"][index]["name"] = StringUtil::aes_encrypt(
      nName + StringUtil::sha3_512(StringUtil::generate_salt()),
      this->password);
  j["data"][index]["username"] = StringUtil::aes_encrypt(
      nUsername + StringUtil::sha3_512(StringUtil::generate_salt()),
      this->password);
  j["data"][index]["password"] = StringUtil::aes_encrypt(
      nPassword + StringUtil::sha3_512(StringUtil::generate_salt()),
      this->password);
  FileUtil::write_json(this->dataFilePath, j);

  createList();
  deleteButton->deleteLater();
  deleteInput();
}

void PasswordWidget::createNewItem() {
  auto *layout = dynamic_cast<QGridLayout *>(this->layout());
  findChild<QPushButton *>("addItemButton")->hide();

  auto *nameLabel = new QLabel("Name", this);
  auto *nameInput = new QLineEdit(this);

  auto *usernameLabel = new QLabel("Username", this);
  auto *usernameInput = new QLineEdit(this);

  auto *passwordLabel = new QLabel("Password", this);
  auto *passwordInput = new QLineEdit(this);

  auto *saveButton = new QPushButton("Save", this);
  auto *cancelButton = new QPushButton("Cancel", this);

  layout->addWidget(nameLabel, 2, 0, 1, 1);
  layout->addWidget(nameInput, 2, 1, 1, 1);
  layout->addWidget(usernameLabel, 3, 0, 1, 1);
  layout->addWidget(usernameInput, 3, 1, 1, 1);
  layout->addWidget(passwordLabel, 4, 0, 1, 1);
  layout->addWidget(passwordInput, 4, 1, 1, 1);
  layout->addWidget(saveButton, 5, 1, 1, 1);
  layout->addWidget(cancelButton, 5, 0, 1, 1);

  connect(saveButton, &QPushButton::clicked, this, [=, this]() {
    save_new();
    deleteInput();
  });
  connect(cancelButton, &QPushButton::clicked, this, [=, this]() {
    deleteInput();
  });

  connect(nameInput, &QLineEdit::returnPressed, this, [=, this]() {
    save_new();
    deleteInput();
  });
  connect(usernameInput, &QLineEdit::returnPressed, this, [=, this]() {
    save_new();
    deleteInput();
  });
  connect(passwordInput, &QLineEdit::returnPressed, this, [=, this]() {
    save_new();
    deleteInput();
  });

  setLayout(layout);
}

void PasswordWidget::save_new() {
  auto *layout = dynamic_cast<QGridLayout *>(this->layout());

  auto *nameInput = dynamic_cast<QLineEdit *>(layout->itemAtPosition(2, 1)->widget());
  auto *usernameInput = dynamic_cast<QLineEdit *>(layout->itemAtPosition(3, 1)->widget());
  auto *passwordInput = dynamic_cast<QLineEdit *>(layout->itemAtPosition(4, 1)->widget());

  std::string nName = nameInput->text().toStdString();
  std::string nUsername = usernameInput->text().toStdString();
  std::string nPassword = passwordInput->text().toStdString();

  json j = FileUtil::read_json(this->dataFilePath);
  j["data"].push_back({
                          {
                              "name",
                              StringUtil::aes_encrypt(
                                  nName + StringUtil::sha3_512(StringUtil::generate_salt()),
                                  this->password)
                          },
                          {
                              "username",
                              StringUtil::aes_encrypt(
                                  nUsername + StringUtil::sha3_512(StringUtil::generate_salt()),
                                  this->password)
                          },
                          {
                              "password",
                              StringUtil::aes_encrypt(
                                  nPassword + StringUtil::sha3_512(StringUtil::generate_salt()),
                                  this->password)
                          }
                      });
  FileUtil::write_json(this->dataFilePath, j);

  deleteInput();
}

void PasswordWidget::deleteInput() {
  auto *layout = dynamic_cast<QGridLayout *>(this->layout());

  findChild<QPushButton *>("addItemButton")->show();
  createList();

  auto *nameInput = layout->itemAtPosition(2, 1)->widget();
  auto *nameLabel = layout->itemAtPosition(2, 0)->widget();
  auto *usernameInput = layout->itemAtPosition(3, 1)->widget();
  auto *usernameLabel = layout->itemAtPosition(3, 0)->widget();
  auto *passwordInput = layout->itemAtPosition(4, 1)->widget();
  auto *passwordLabel = layout->itemAtPosition(4, 0)->widget();
  auto *saveButton = layout->itemAtPosition(5, 1)->widget();
  auto *cancelButton = layout->itemAtPosition(5, 0)->widget();

  nameInput->deleteLater();
  nameLabel->deleteLater();
  usernameInput->deleteLater();
  usernameLabel->deleteLater();
  passwordInput->deleteLater();
  passwordLabel->deleteLater();
  saveButton->deleteLater();
  cancelButton->deleteLater();
}