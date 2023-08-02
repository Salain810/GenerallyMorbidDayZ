void main()
{
    //INIT WEATHER BEFORE ECONOMY INIT------------------------
    Weather weather = g_Game.GetWeather();

    weather.MissionWeather(false);    // false = use weather controller from Weather.c

    weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
    weather.GetRain().Set( 0, 0, 1);
    weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);

    //INIT ECONOMY--------------------------------------
    Hive ce = CreateHive();
    if ( ce )
        ce.InitOffline();

    //DATE RESET AFTER ECONOMY INIT-------------------------
    int year, month, day, hour, minute;
    int reset_month = 9, reset_day = 20;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);

    if ((month == reset_month) && (day < reset_day))
    {
        GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
    }
    else
    {
        if ((month == reset_month + 1) && (day > reset_day))
        {
            GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
        }
        else
        {
            if ((month < reset_month) || (month > reset_month + 1))
            {
                GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
            }
        }
    }
}

class CustomMission: MissionServer
{
    void SetRandomHealth(EntityAI itemEnt)
    {
        if ( itemEnt )
        {
            float rndHlt = Math.RandomFloat( 0.45, 0.65 );
            itemEnt.SetHealth01( "", "", rndHlt );
        }
    }

    override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
    {
        Entity playerEnt;
        playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
        Class.CastTo( m_player, playerEnt );

        GetGame().SelectPlayer( identity, m_player );

        return m_player;
    }

    //? this method is called when players respawn/spawn/login
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

    //? this method is called periodically by the server
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

            // update player's dogtags base on time
            float playtime = player.StatGet("playtime");

            if (playtime > 21599 && playtime < 43200)
            {
                player.ReplaceDogtag("Dogtag_Granite");
            }
            else if (playtime > 43199)
            {
                player.ReplaceDogtag("Dogtag_Cobalt");
            }
        }
    }

    override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
    {
        EntityAI itemClothing;
        EntityAI itemEnt;
        ItemBase itemBs;
        float rand;

        itemClothing = player.FindAttachmentBySlotName( "Body" );
        if ( itemClothing )
        {
            SetRandomHealth( itemClothing );

            itemEnt = itemClothing.GetInventory().CreateInInventory( "Rag" );
            if ( Class.CastTo( itemBs, itemEnt ) )
                itemBs.SetQuantity( 4 );

            SetRandomHealth( itemEnt );

            string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
            int rndIndex = Math.RandomInt( 0, 4 );
            itemEnt = itemClothing.GetInventory().CreateInInventory( chemlightArray[rndIndex] );
            SetRandomHealth( itemEnt );

            rand = Math.RandomFloatInclusive( 0.0, 1.0 );
            if ( rand < 0.35 )
                itemEnt = player.GetInventory().CreateInInventory( "Apple" );
            else if ( rand > 0.65 )
                itemEnt = player.GetInventory().CreateInInventory( "Pear" );
            else
                itemEnt = player.GetInventory().CreateInInventory( "Plum" );

            SetRandomHealth( itemEnt );
        }

        itemClothing = player.FindAttachmentBySlotName( "Legs" );
        if ( itemClothing )
            SetRandomHealth( itemClothing );

        itemClothing = player.FindAttachmentBySlotName( "Feet" );
    }
};

Mission CreateCustomMission(string path)
{
    return new CustomMission();
}
