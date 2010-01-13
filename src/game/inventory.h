#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "inv_shared.h"

typedef struct inventoryInterface_s
{
	invList_t* invUnused;

	item_t cacheItem;

	csi_t* csi;

	qboolean (*RemoveFromInventory) (struct inventoryInterface_s* self, inventory_t* const i,
			const invDef_t * container, invList_t *fItem) __attribute__((nonnull(1), nonnull(2), warn_unused_result));

	invList_t* (*AddToInventory) (struct inventoryInterface_s* self, inventory_t * const i, item_t item,
			const invDef_t * container, int x, int y, int amount) __attribute__((nonnull(1), nonnull(2), warn_unused_result));

	int (*MoveInInventory) (struct inventoryInterface_s* self, inventory_t* const i, const invDef_t * from,
			invList_t *item, const invDef_t * to, int tx, int ty, int *TU, invList_t ** icp) __attribute__((nonnull(1), nonnull(2)));

	qboolean (*TryAddToInventory) (struct inventoryInterface_s* self, inventory_t* const inv, item_t item,
			const invDef_t * container);

	void (*DestroyInventory) (struct inventoryInterface_s* self, inventory_t* const i) __attribute__((nonnull(1), nonnull(2)));

	void (*EmptyContainer) (struct inventoryInterface_s* self, inventory_t* const i, const invDef_t * container)__attribute__((nonnull(1), nonnull(2)));

	void (*EquipActor) (struct inventoryInterface_s* self, inventory_t* const inv, const equipDef_t *ed,
			character_t* chr) __attribute__((nonnull(1), nonnull(2)));

	void (*EquipActorMelee) (struct inventoryInterface_s* self, inventory_t* const inv, character_t* chr) __attribute__((nonnull(1)));

	void (*EquipActorRobot) (struct inventoryInterface_s* self, inventory_t* const inv, character_t* chr,
			objDef_t* weapon) __attribute__((nonnull(1), nonnull(2)));

} inventoryInterface_t;

void INV_InitInventory(inventoryInterface_t *interface, csi_t* csi, invList_t* invList, size_t length);

#endif /* INVENTORY_H_ */
