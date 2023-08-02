//* Orignal method
override Dogtag_Base EquipDogtag(PlayerBase player)
{
	if (!player.HasDogtag()) // check if the player has a tag already
	{
		// create a new tag is the player doesn't have one
		int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
		return Dogtag_Base.Cast(player.GetInventory().CreateAttachmentEx("Dogtag_Survivor", slotId));
	}

	return null;
}


//* Random tags
override Dogtag_Base EquipDogtag(PlayerBase player)
{
	array<string> tags = {"Dogtag_Survivor", "Dogtag_Hero", "Dogtag_Bandit"}; // list of available dogtag variants

	Dogtag_Base tag;
	if (!player.HasDogtag()) // check if the player has a tag already
	{
		// create a new tag is the player doesn't have one
		int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
		return player.GetInventory().CreateAttachmentEx(tags.GetRandomElement(), slotId); // give the player a random dogtag variant
	}

	return null;
}


//* Update dogtag dynamically over time
override void UpdatePlayersStats()
{
    super.UpdatePlayersStats();

    array<Man> players = {};
    GetGame().GetPlayers(players);

    for (int i = 0; i < players.Count(); i++)
    {
        PlayerBase player;
        if (!PlayerBase.CastTo(player, players.Get(i)))
            continue;

        // update player's dogtag base on time
        float playtime = player.StatGet("playtime");

        if (playtime >= 21600 && playtime < 43200)
        {
            player.ReplaceDogtag("Dogtag_Granite");
        }
        else if (playtime >= 43200)
        {
            player.ReplaceDogtag("Dogtag_Cobalt");
        }
    }
}


//* Update dogtag dynamically on kills per life
override void UpdatePlayersStats()
{
    super.UpdatePlayersStats();

    array<Man> players = {};
    GetGame().GetPlayers(players);

    for (int i = 0; i < players.Count(); i++)
    {
        PlayerBase player;
        if (!PlayerBase.CastTo(player, players.Get(i)))
            continue;

        // update player's dogtag base on player kills
        float kills = player.StatGet("players_killed");

        if (kills >= 5 && kills < 10)
        {
            player.ReplaceDogtag("Dogtag_Granite");
        }
        else if (kills >= 10)
        {
            player.ReplaceDogtag("Dogtag_Cobalt");
        }
    }
}


//* HerosAndBandits spawning with specific dogtags (DOES NOT UPDATE WHEN HUMANITY IS CHANGED)
override Dogtag_Base EquipDogtag(PlayerBase player)
{
	string tag_variant;
	Dogtag_Base tag;

	if (!player.HasDogtag()) // check if the player has a tag already
	{
		// get HerosAndBandits bandit/hero status
		HeroesAndBanditsPlayer hab_player = GetHeroesAndBandits().GetPlayer(player.GetIdentity().GetPlainId());
		switch (hab_player.getHeroOrBandit())
		{
			case "hero":
				tag_variant = "Dogtag_Hero";
			break;
			case "bandit":
				tag_variant = "Dogtag_Bandit";
			break;
			default:
				tag_variant = "Dogtag_Survivor";
			break;
		}

		// create a new dogtag is the player doesn't have one
		int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
		return player.GetInventory().CreateAttachmentEx(tag_variant, slotId); // give the player dogtag based on their hero/bandit status
	}

	return null;
}
