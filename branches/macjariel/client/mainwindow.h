/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QList>
#include <QGridLayout>

#include "ui_mainwindow.h"
#include "serverconnection.h"



class ConnectToServerDialog;
class CreateGameDialog;
class JoinGameDialog;
class LogWidget;
class ChatWidget;
class QLabel;
class PlayerWidget;
class OpponentWidget;
class Game;

/**
 *@author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

private slots:
    void connectToServer();
    void disconnectFromServer();
    void showCreateGameDialog();
    void showJoinGameDialog();
    void leaveGame();


    void playerJoinedGame(int gameId, const StructPlayer& player, bool other, bool creator);
    void playerLeavedGame(int gameId, const StructPlayer& player, bool other);


    void serverConnectionStatusChanged();

private:
    ConnectToServerDialog* mp_connectToServerDialog;
    CreateGameDialog*      mp_createGameDialog;
    JoinGameDialog*        mp_joinGameDialog;
    LogWidget*             mp_logWidget;
    ChatWidget*            mp_chatWidget;
    ServerConnection       m_serverConnection;
    Game*                  mp_game;
    QLabel*                mp_labelStatusBarServerState;
    PlayerWidget*          mp_playerWidget;
    QList<OpponentWidget*> m_opponentWidgets;
    QGridLayout*           mp_layout;


private:
    void createStatusBar();
    void createMenu();
    void createActions();
    void createWidgets();

    void updateActions();

};

#endif
