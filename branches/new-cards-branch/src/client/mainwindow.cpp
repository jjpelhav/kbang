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

#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>

#include "mainwindow.h"         // Class declaration
#include "ui_mainwindow.h"      // UI
#include "config.h"
#include "game.h"
#include "serverconnection.h"
#include "cardwidgetsizemanager.h"

#include "connecttoserverdialog.h"
#include "creategamedialog.h"
#include "joingamedialog.h"
#include "aboutdialog.h"

using namespace client;

MainWindow* MainWindow::smp_singleton(0);

MainWindow::MainWindow():
        mp_connectToServerDialog(0),
        mp_createGameDialog(0),
        mp_joinGameDialog(0),
        mp_aboutDialog(0),
        mp_ui(new Ui::MainWindow)
{
    Q_ASSERT(smp_singleton == 0);
    smp_singleton = this;
    mp_ui->setupUi(this);
    setStyleSheet(styleSheet() + "#mp_centralWidget { background-image: url(\""
                  + Config::systemDataLocation() + "gfx/misc/bang-artwork.png\");\n}\n\n");

    createActions();
    createWidgets();
    updateActions();

    connect(ServerConnection::instance(), SIGNAL(statusChanged()),
            this, SLOT(serverConnectionStatusChanged()));
    connect(ServerConnection::instance(), SIGNAL(enterGameMode(int, const QString&, ClientType)),
            this,                SLOT(enterGameMode(int, const QString&, ClientType)));
    connect(ServerConnection::instance(), SIGNAL(exitGameMode()),
            this,                SLOT(exitGameMode()));
}


MainWindow::~MainWindow()
{
    delete mp_ui;
    smp_singleton = 0;
}

LocalPlayerWidget*
MainWindow::localPlayerWidget() const
{
    return mp_ui->localPlayerWidget;
}

QWidget*
MainWindow::middleWidget() const
{
    return mp_ui->middleWidget;
}

LogWidget*
MainWindow::logWidget() const
{
    return mp_ui->logWidget;
}

void MainWindow::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QRadialGradient g(width() / 2, height() / 2, width() / 2, width() / 2 , height() / 2);
    g.setColorAt(0, QColor(239, 215, 179));
    g.setColorAt(0.5 , QColor(211, 179, 140));
    painter.fillRect(e->rect(), g);
    QMainWindow::paintEvent(e);
}


/* slot */ void
MainWindow::showConnectToServerDialog()
{
    if (!mp_connectToServerDialog) {
        mp_connectToServerDialog = new ConnectToServerDialog(this);
        connect(mp_connectToServerDialog, SIGNAL(connectToServer(QString, int)),
                ServerConnection::instance(), SLOT(connectToServer(QString, int)));
    }
    mp_connectToServerDialog->show();
}

/* slot */ void
MainWindow::showCreateGameDialog()
{
    if (!mp_createGameDialog) {
        mp_createGameDialog = new CreateGameDialog(this);
        connect(mp_createGameDialog,SIGNAL(createGame(CreateGameData, CreatePlayerData)),
                ServerConnection::instance(), SLOT(createGame(CreateGameData, CreatePlayerData)));
    }
    mp_createGameDialog->show();
}

/* slot */ void
MainWindow::showJoinGameDialog()
{
    if (!mp_joinGameDialog) {
        mp_joinGameDialog = new JoinGameDialog(this);
        connect(mp_joinGameDialog, SIGNAL(joinGame(int, int, QString, CreatePlayerData)),
                ServerConnection::instance(), SLOT(joinGame(int, int, QString, CreatePlayerData)));
    }
    mp_joinGameDialog->show();
}

/* slot */ void
MainWindow::showAboutDialog()
{
    if (!mp_aboutDialog) {
        mp_aboutDialog = new AboutDialog(this);
    }
    mp_aboutDialog->show();
}

void
MainWindow::showWarningMessage(const QString& message)
{
    QMessageBox* msgBox = new QMessageBox(0);
    msgBox->setWindowTitle(QApplication::applicationName());
    msgBox->setText(message);
    msgBox->setIcon(QMessageBox::Warning);
    connect(msgBox, SIGNAL(finished(int)), msgBox, SLOT(deleteLater()));
    QTimer::singleShot(0, msgBox, SLOT(exec()));
}

/* signal */ void
MainWindow::enterGameMode(GameId gameId, const QString& gameName, ClientType clientType)
{
    Game::enterGameMode(gameId, gameName, clientType);
#if 0
    connect(mp_game, SIGNAL(emitLogMessage(const QString&)),
            mp_logWidget, SLOT(appendLogMessage(const QString&)));
    mp_logWidget->appendLogMessage(tr("You have joined <i>%1</i>.").arg(gameName));
#endif
    updateActions();
}

/* signal */ void
MainWindow::exitGameMode()
{
    Game::currentGame()->leaveGameMode();
#if 0
    mp_chatWidget->clear();
#endif
    updateActions();
}

void MainWindow::serverConnectionStatusChanged()
{
    updateActions();
}

void
MainWindow::createActions()
{
    connect(mp_ui->actionConnectToServer, SIGNAL(triggered()),
            this, SLOT(showConnectToServerDialog()));
    connect(mp_ui->actionDisconnectFromServer, SIGNAL(triggered()),
            ServerConnection::instance(), SLOT(disconnectFromServer()));
    connect(mp_ui->actionCreateGame, SIGNAL(triggered()),
            this, SLOT(showCreateGameDialog()));
    connect(mp_ui->actionJoinGame, SIGNAL(triggered()),
            this, SLOT(showJoinGameDialog()));
    connect(mp_ui->actionLeaveGame, SIGNAL(triggered()),
            ServerConnection::instance(), SLOT(leaveGame()));
    connect(mp_ui->actionAbout, SIGNAL(triggered()),
            this, SLOT(showAboutDialog()));
    connect(mp_ui->actionCardSizeUp, SIGNAL(triggered()),
            &CardWidgetSizeManager::instance(), SLOT(cardSizeUp()));
    connect(mp_ui->actionCardSizeDown, SIGNAL(triggered()),
            &CardWidgetSizeManager::instance(), SLOT(cardSizeDown()));
}

void
MainWindow::createWidgets()
{
    m_opponentWidgets.append(mp_ui->opponent0);
    m_opponentWidgets.append(mp_ui->opponent1);
    m_opponentWidgets.append(mp_ui->opponent2);
    m_opponentWidgets.append(mp_ui->opponent3);
    m_opponentWidgets.append(mp_ui->opponent4);
    m_opponentWidgets.append(mp_ui->opponent5);
    m_opponentWidgets.append(mp_ui->opponent6);

    mp_ui->opponent0->hide();

    connect(mp_ui->chatWidget, SIGNAL(outgoingMessage(const QString&)),
            ServerConnection::instance(), SLOT(sendChatMessage(const QString&)));
    connect(ServerConnection::instance(), SIGNAL(incomingChatMessage(int, const QString&, const QString&)),
            mp_ui->chatWidget, SLOT(appendIncomingMessage(int, const QString&, const QString&)));

    connect(ServerConnection::instance(), SIGNAL(logMessage(QString)),
            mp_ui->logWidget, SLOT(appendLogMessage(QString)));
}

void
MainWindow::updateActions()
{
    if (ServerConnection::instance()->isConnected()) {
        mp_ui->actionConnectToServer->setEnabled(0);
        mp_ui->actionDisconnectFromServer->setEnabled(1);
        if (Game::isInGame()) {
            mp_ui->actionCreateGame->setEnabled(0);
            mp_ui->actionJoinGame->setEnabled(0);
            mp_ui->actionLeaveGame->setEnabled(1);
        } else {
            mp_ui->actionCreateGame->setEnabled(1);
            mp_ui->actionJoinGame->setEnabled(1);
            mp_ui->actionLeaveGame->setEnabled(0);
        }
    } else {
        mp_ui->actionConnectToServer->setEnabled(1);
        mp_ui->actionDisconnectFromServer->setEnabled(0);
        mp_ui->actionCreateGame->setEnabled(0);
        mp_ui->actionJoinGame->setEnabled(0);
        mp_ui->actionLeaveGame->setEnabled(0);
    }
}