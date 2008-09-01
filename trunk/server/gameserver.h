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
#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QtCore>

class Game;
class Client;
class TcpServer;
class QXmlStreamWriter;

/**
 * The GameServer class is a singleton class that represent the state of the whole
 * server. It holds all the clients and games created on the server, as well as some
 * attributes of the server such as the server name, description, etc.
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class GameServer: public QObject
{
    Q_OBJECT;
private:
    GameServer();
    GameServer(GameServer&);
    GameServer& operator=(const GameServer&);

public:
    /**
     * This method returns reference to the GameServer (singleton) instance.
     */
    static inline GameServer& instance()
    {
        if (!sm_instance) sm_instance = new GameServer();
        return *sm_instance;
    }

    /**
     * This method creates a new game.
     * @returns pointer to newly created Game
     * @see Game constructor
     */
    Game* createGame(const QString& name,
                          const QString& description,
                          int creatorId,
                          int minPlayers,
                          int maxPlayers,
                          int maxObservers,
                          const QString& playerPassword,
                          const QString& observerPassword,
                          bool shufflePlayers);


    /**
     * Returns a list of games (Game instances).
     */
    inline QList<Game*> gameList()
    {
        return m_games.values();
    }

    /**
     * Returns a pointer to the Game with
     * given id.
     */
    inline Game* game(int id)
    {
        if (m_games.contains(id)) return m_games[id];
        return 0;
    }

    /**
     * Returns a list of clients.
     */
    inline QList<Client*> clientList()
    {
        return m_clients.values();
    }

    /**
     * Returns a pointer to the Client with given id.
     */
    inline Client* client(int id)
    {
        if (m_clients.contains(id)) return m_clients[id];
        return 0;
    }

    /**
     * Returns name of the server.
     */
    inline QString name() const
    {
        return m_name;
    }

    /**
     * Returns descriptive text of the server.
     */
    inline QString description() const
    {
        return m_description;
    }

    /**
     * Writes the information about the server to
     * given QXmlStreamWriter.
     */
    void writeXml(QXmlStreamWriter& xmlOut);

    /**
     * Tells the GameServer to listen for incoming connections.
     * @returns false in case of error, otherwise true
     */
    bool listen();

    /**
     * Prepares GameServer for exiting.
     */
    void exit();

signals:
    void aboutToQuit();

public slots:
    void createClient();
    void deleteClient(int clientId);

private:
    static GameServer*       sm_instance;
    QHash<int, Game*>   m_games;
    QHash<int, Client*>      m_clients;

    int m_nextClientId;
    int m_nextGameId;
    int m_maxClientCount; // is this necessary?
    QString m_name;
    QString m_description;
    TcpServer* mp_tcpServer;

};

#endif
