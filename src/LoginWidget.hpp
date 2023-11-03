//
// Created by caracal on 11/1/23.
//

#ifndef PASSWORDMANAGERQT6_LOGINWIDGET_HPP
#define PASSWORDMANAGERQT6_LOGINWIDGET_HPP

#include <qwidget.h>
#include <QGridLayout>

class LoginWidget : public QWidget {
Q_OBJECT

public:
  explicit LoginWidget(QWidget *parent = nullptr);

  ~LoginWidget() override;

  std::string getUsername();

  std::string getPassword();

  void notifyText(const std::string &message, const std::string &color);

signals:

  void loginButtonClicked();

  void registerButtonClicked();

private:
  std::string username;
  std::string password;


};


#endif //PASSWORDMANAGERQT6_LOGINWIDGET_HPP
