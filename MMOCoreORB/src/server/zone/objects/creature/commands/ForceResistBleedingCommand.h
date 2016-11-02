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

		if (playerObject->getForcePower() <= forceCost) {
			creature->sendSystemMessage("You don't have enough Force to preform this ability");
			return false;
		}
                              
                //Need updated strings
		StringIdChatParameter startStringId("jedi_spam", "apply_forcerun1");
		StringIdChatParameter endStringId("jedi_spam", "remove_forcerun1");

		int duration = 360 + resist;

		ManagedReference<Buff*> buff = new Buff(creature, forceResistBleedingCRC, duration, BuffType::JEDI);
		buff->setSpeedMultiplierMod(1.5f);
		buff->setAccelerationMultiplierMod(1.5f);
		buff->setStartMessage(startStringId);
		buff->setEndMessage(endStringId);
		buff->setSkillModifier("resistance_bleeding", 25 + resist);
		buff->setSkillModifier("absorption_bleeding", 25 + resist);

                if (playerObject->getForcePower() >= forceCost) {
		        creature->addBuff(buff);
	                playerObject->setForcePower(playerObject->getForcePower() - forceCost);
		        //creature->playEffect("clienteffect/pl_force_run_self.cef", "");
                        return SUCCESS;
                }
		
		return GENERALERROR;
        }
};
		
#endif //FORCERESISTBLEEDINGCOMMAND_H_
