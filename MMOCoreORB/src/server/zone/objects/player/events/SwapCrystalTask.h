
#ifndef SWAPCRYSTALTASK_H_
#define SWAPCRYSTALTASK_H_

#include "engine/engine.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "templates/params/creature/CreatureAttribute.h"

class SwapCrystalTask: public Task {
	ManagedReference<CreatureObject*> player;
	String moodString;

public:
	SwapCrystalTask(CreatureObject* pl) {
		player = pl;
	}

	void run() {
		Locker playerLocker(player);

		try {
			Reference<SwapCrysalTask*> swapcrystalTask = player->getPendingTask("swapcrystal").castTo<SwapCrystalTask*>();
			ManagedReference<WeaponObject*> weapon = cast<WeaponObject*>(_this.getReferenceUnsafeStaticCast()->getParent().get()->getParent().get().get());
			Reference<SwapCrystalTask*> swapcrystalTask = new SwapCrystalTask(player);
			int random = (System::random(11));
			weapon->setCustomizationVariable("/private/index_color_blade", random, true);

			if (swapcrystalTask != NULL) {
				swapcrystalTask->reschedule(500);
			}

		} catch (Exception& e) {
			player->error("unreported exception caught in CrystalSwapTask::activate");
		}
	}

};

#endif /* SWAPCRYSTALTASK_H_ */
