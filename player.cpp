#include "player.h"
#include <string.h>
#include <QDebug>

Player::Player(QObject *parent) : QObject(parent)
{
    _username = "";
    _wins = 0;
    _losses = 0;
    _draws = 0;
    objectID = "";
}

int Player::getWins() const
{
    return _wins;
}

void Player::setWins(int value)
{
    _wins = value;
}
int Player::getLosses() const
{
    return _losses;
}

void Player::setLosses(int value)
{
    _losses = value;
}

int Player::getDraws() const
{
    return _draws;
}

void Player::setDraws(int value)
{
    _draws = value;
}

void Player::loginPlayer(QString username)
{
    _username = username;
    ParseHelper *p = new ParseHelper(this);
    connect(p, SIGNAL(loginFinished(bool , QNetworkReply* )), this, SLOT(handleLogin(bool , QNetworkReply *)));
    p->login(username);
}


void Player::registerPlayer(QString username){
    _username = username;
    ParseHelper *p = new ParseHelper(this);
    connect(p, SIGNAL(registerFinished(bool , QNetworkReply* )), this, SLOT(handleRegister(bool , QNetworkReply *)));
    p->registerPlayer(username);
}

void Player::handleLogin(bool exists, QNetworkReply* reply){
    if (exists){
        QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
        _username = json.value("username").toString();
        objectID = json.value("objectId").toString();
        session = json.value("sessionToken").toString();
        _wins = json.value("win").toInt();
        _losses = json.value("loss").toInt();
        _draws = json.value("draw").toInt();
        emit loginFinished();
    }else{
        _username = "";
        emit loginFailed();
    }
}

void Player::handleRegister(bool exists, QNetworkReply* reply){
    if (exists){
        QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
        objectID = json.value("objectId").toString();
        session = json.value("sessionToken").toString();
        _wins = 0;
        _losses = 0;
        _draws = 0;
        emit registerFinished();
    }else{
        _username = "";
        emit registerFailed();
    }
}

void Player::updatePlayer(int wins, int losses, int draws){
    _wins += wins;
    _losses += losses;
    _draws += draws;
    ParseHelper *p = new ParseHelper(this);
    p->updatePlayer(objectID, session, _wins, _losses, _draws);
}

void Player::logout(){
    _username = "";
    objectID = "";
    session = "";
    _wins = 0;
    _losses = 0;
    _draws = 0;
}

void Player::deletePlayer(){
    ParseHelper *p = new ParseHelper(this);
    p->deletePlayer(objectID, session);
    logout();
}
