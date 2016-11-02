/*
 				Copyright <SWGEmu>
		See file COPYING for copying conditions. */


#ifndef FORCEMEDITATETASK_H_
#define FORCEMEDITATETASK_H_

#include "engine/engine.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "templates/params/creature/CreatureAttribute.h"

class ForceMeditateTask: public Task {
	ManagedReference<CreatureObject*> player;
	String moodString;

public:
	ForceMeditateTask(CreatureObject* pl) {
		player = pl;
	}

	void setMoodString(const String& ms) {
		moodString = ms;
	}

	String getMoodString() {
		return moodString;
	}

	void run() {
		Locker playerLocker(player);

		try {
			Reference<ForceMeditateTask*> fmeditateTask = player->getPendingTask("forcemeditate").castTo<ForceMeditateTask*>();

			if (!player->isMeditating())
				return;
			
			if (player->isBleeding() || player->isPoisoned() || player->isDiseased()) {
				if (player->isBleeding())
					player->healDot(CreatureState::BLEEDING, (15));
				else if (player->isPoisoned())
					player->healDot(CreatureState::POISONED, (15));
				else if (player->isDiseased())
					player->healDot(CreatureState::DISEASED, (15));
			}
			
			StringIdChatParameter healParams;

			//Here we are checking to see which pools have wounds, and we add them to a vector...
			Vector<uint8> woundedPools;
			for (uint8 i = 0; i < 9; ++i) {
				if (player->getWounds(i) > 0)
					woundedPools.add(i);
			}
					
			if (woundedPools.size() > 0) {
				int heal = 30 + System::random(20);

				// Select a random Attribute that has wounds...
				uint8 pool = woundedPools.get(System::random(woundedPools.size() - 1));

				int wounds = player->getWounds(pool);

				//Cap the heal at the amount of wounds the creature has.
				heal = MIN(wounds, heal);
				player->healWound(player, pool, heal, true, false);

				// Sending System healing Message (wounds)
				healParams.setStringId("teraskasi", "prose_curewound"); // [meditation] Your %TO wounds heal by %DI points.
				healParams.setTO(CreatureAttribute::getName(pool));
				healParams.setDI(heal);
				player->sendSystemMessage(healParams);
			}
				
			int bf = player->getShockWounds();
			if (bf > 9) {
				player->addShockWounds(-10, true);
				player->sendSystemMessage("You heal 10 Battle Fatigue");
			} else {
				player->setShockWounds(0);
				player->sendSystemMessage("You finish healing your Battle Fatigue");
			}

			if (fmeditateTask != NULL)
				fmeditateTask->reschedule(5000);
				player->playEffect("clienteffect/pl_force_meditate_self.cef", "");

		} catch (Exception& e) {
			player->error("unreported exception caught in ForceMeditateTask::activate");
		}
	}

};

#endif /* FORCEMEDITATETASK_H_ */
