/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCERESISTBLEEDINGCOMMAND_H_
#define FORCERESISTBLEEDINGCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class ForceResistBleedingCommand : public JediQueueCommand {
public:

	ForceResistBleedingCommand(const String& name, ZoneProcessServer* server)
		: JediQueueCommand(name, server) {

		buffCRC = BuffCRC::JEDI_RESIST_BLEEDING;
/*
		skillMods.put("resistance_bleeding", 25);
		skillMods.put("absorption_bleeding", 25);
*/

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		
		int res = creature->hasBuff(buffCRC) ? NOSTACKJEDIBUFF : doJediSelfBuffCommand(creature);

		if (res == NOSTACKJEDIBUFF) {
			creature->sendSystemMessage("You already have this buff");
			return GENERALERROR;
}
		int resist = (creature->getSkillMod("force_resist");
			      
		if (resist > 0) {
			creature->setSkillModifier("resistance_bleeding", 25 + resist);
			creature->setSkillModifier("absorption_bleeding", 25 + resist);
		} else {
			creature->setSkillModifier("resistance_bleeding", 25);
			creature->setSkillModifier("absorption_bleeding", 25);
		}
			      
		return SUCCESS;
	}

};

#endif //FORCERESISTBLEEDINGCOMMAND_H_
