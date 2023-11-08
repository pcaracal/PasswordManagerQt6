//
// Created by caracal on 11/2/23.
//

#ifndef PASSWORDMANAGERQT6_PASSWORDWIDGET_HPP
#define PASSWORDMANAGERQT6_PASSWORDWIDGET_HPP


#include <qwidget.h>
#include <QGridLayout>

class PasswordWidget : public QWidget {
Q_OBJECT

public:
  explicit PasswordWidget(QWidget *parent, const std::string &username, const std::string &password,
                          const std::string &dataFilePath);

  ~PasswordWidget() override;

private:
  std::string username;
  std::string password;
  std::string dataFilePath;

  void createList();

  void createNewItem();

  void editItem(const int &index);

  void save_new();

  void deleteInput();

  void save_edit(const int &index);
};


#endif //PASSWORDMANAGERQT6_PASSWORDWIDGET_HPP
