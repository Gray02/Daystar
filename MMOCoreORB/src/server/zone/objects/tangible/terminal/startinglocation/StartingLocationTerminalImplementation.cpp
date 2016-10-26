/*
 * StartingLocationTerminalImplementation.cpp
 *
 *  Created on: 12/08/2009
 *      Author: victor
 */

#include "server/zone/objects/tangible/terminal/startinglocation/StartingLocationTerminal.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/callbacks/ExpSelectSuiCallback.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/player/PlayerManager.h"

void StartingLocationTerminalImplementation::initializeTransientMembers() {
	TerminalImplementation::initializeTransientMembers();

	setLoggingName("StartingLocationTerminal");
	setInfoLogLevel();
}

int StartingLocationTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	//System::out << "entering start terminal radial call" << endl;
	if (selectedID != 20 || !authorizationState) { // not use object or not authorized
		if (!authorizationState)
			info("handleObjectMenuSelect: not authorized");

		return 1;
	}
	
	info("player->getSelectedExpMode() is "+ player->getSelectedExpMode());
	
	if(player->getSelectedExpMode() == 0 ){
		if(!player->getPlayerObject()->hasSuiBoxWindowType(SuiWindowType::XP_SELECT)){
			ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
			ManagedReference<SuiListBox*> xpBox = new SuiListBox(player, SuiWindowType::XP_SELECT, 0);
			xpBox->setCallback(new ExpSelectSuiCallback(player->getZoneServer()));
			xpBox->setPromptTitle("Select Experience");
			xpBox->setPromptText("Test text till proper is created.");
			xpBox->addMenuItem("Cupcake: 3x Exp",0);
			xpBox->addMenuItem("Normal: 1.5x Exp",1);
			xpBox->addMenuItem("THE GRIND: 0.5x Exp",2);

			ghost->addSuiBox(xpBox);
			player->sendMessage(xpBox->generateMessage());
			return 1;
		}else{
			return 1;
		}
	}
	
	server->getPlayerManager()->sendStartingLocationsTo(player);

	return 0;
}
