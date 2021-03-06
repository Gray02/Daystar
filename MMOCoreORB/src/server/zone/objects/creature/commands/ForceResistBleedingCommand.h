/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCERESISTBLEEDINGCOMMAND_H_
#define FORCERESISTBLEEDINGCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"


class ForceResistBleedingCommand : public QueueCommand {
public:

	ForceResistBleedingCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		uint32 forceResistBleedingCRC = BuffCRC::JEDI_RESIST_BLEEDING;

		
		if(creature->hasBuff(forceResistBleedingCRC)) {
			creature->sendSystemMessage("@jedi_spam:force_buff_present"); //"You already have a similar Force enhancement active."
			return GENERALERROR;
		}
		
		// Force cost of skill.
		int forceCost = 100;
                
		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();
		int resist = (creature->getSkillMod("force_resist"));
		int resistBle = (creature->getSkillMod("resistance_bleeding"));
		int absorbBle = (creature->getSkillMod("absorption_bleeding"));

		if (playerObject->getForcePower() <= forceCost) {
			creature->sendSystemMessage("You don't have enough Force to preform this ability");
			return false;
		}
                              
                //Need updated strings
		StringIdChatParameter startStringId("jedi_spam", "apply_forceresistbleeding");
		StringIdChatParameter endStringId("jedi_spam", "remove_forceresistbleeding");

		int duration = 360 + resist;
		ManagedReference<Buff*> buff = new Buff(creature, forceResistBleedingCRC, duration, BuffType::JEDI);
		buff->setStartMessage(startStringId);
		buff->setEndMessage(endStringId);
		buff->setSkillModifier("resistance_bleeding", 25 + resist + resistBle);
		buff->setSkillModifier("absorption_bleeding", 25 + resist + absorbBle);
		creature->addBuff(buff);
		
	        playerObject->setForcePower(playerObject->getForcePower() - forceCost);
		creature->playEffect("clienteffect/pl_force_resist_bleeding_self.cef", "");
                return SUCCESS;
        }
};
		
#endif //FORCERESISTBLEEDINGCOMMAND_H_
