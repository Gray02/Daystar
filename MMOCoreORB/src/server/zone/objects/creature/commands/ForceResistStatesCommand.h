/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCERESISTSTATESCOMMAND_H_
#define FORCERESISTSTATESCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class ForceResistStatesCommand : public JediQueueCommand {
public:

	ForceResistStatesCommand(const String& name, ZoneProcessServer* server)
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

		uint32 forceResistStatesCRC = BuffCRC::JEDI_RESIST_STATES;

		
		if(creature->hasBuff(forceResistStatesCRC)) {
			creature->sendSystemMessage("@jedi_spam:force_buff_present"); //"You already have a similar Force enhancement active."
			return GENERALERROR;
		}
		
		// Force cost of skill.
		int forceCost = 100;
                
		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();
		int resist = (creature->getSkillMod("force_resist"));
		int resistBle = (creature->getSkillMod("resistance_states"));

		if (playerObject->getForcePower() <= forceCost) {
			creature->sendSystemMessage("You don't have enough Force to preform this ability");
			return false;
		}
                              
                //Need updated strings
		StringIdChatParameter startStringId("jedi_spam", "apply_forceresiststates");
		StringIdChatParameter endStringId("jedi_spam", "remove_forceresiststates");

		int duration = 360 + resist;
		ManagedReference<Buff*> buff = new Buff(creature, forceResistStatesCRC, duration, BuffType::JEDI);
		buff->setStartMessage(startStringId);
		buff->setEndMessage(endStringId);
		buff->setSkillModifier("resistance_states", 25 + resist + resistBle);
		creature->addBuff(buff);
		
	        playerObject->setForcePower(playerObject->getForcePower() - forceCost);
		creature->playEffect("clienteffect/pl_force_resist_states_self.cef", "");
                return SUCCESS;
        }
};

#endif //FORCERESISTSTATESCOMMAND_H_
