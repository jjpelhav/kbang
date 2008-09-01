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
#ifndef STANZAACTION_H
#define STANZAACTION_H

#include <QHash>
#include <QMap>
#include <stanza.h>

/**
 * The StanzaAction class represents the action stanzas recieved from client.
 * @author MacJariel <echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil">
 */
class StanzaAction : public Stanza
{
friend class Stanza;
private:
    typedef void (StanzaAction::*ExecuteMethod)(QXmlStreamWriter&);

protected:
    StanzaAction(const QXmlStreamReader& xmlIn);

public:
    virtual void processToken(const QXmlStreamReader& xmlIn);
    virtual void execute(QXmlStreamWriter& xmlOut);
    virtual ~StanzaAction();

private:
    static void initializeMethods();
    void writeErrorElement(QXmlStreamWriter& xmlOut);
    //    void writeStanzaStartElement(QXmlStreamWriter& xmlOut);
    //    void writeStanzaEndElement(QXmlStreamWriter& xmlOut);

private:
    void createGame(QXmlStreamWriter& xmlOut);
    void joinGame(QXmlStreamWriter& xmlOut);
    void leaveGame(QXmlStreamWriter& xmlOut);
    void say(QXmlStreamWriter& xmlOut);


private:
    static QHash<QString, ExecuteMethod> sm_methods;
    static bool sm_initialized;

    QString              m_elementName;
    QXmlStreamAttributes m_attributes;
    QMap<QString, QXmlStreamAttributes> m_optElements;
};

#endif
