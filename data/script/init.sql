--
-- MultiplayerOnlineGame - multiplayer game project
-- Copyright (C) 2008-2014 Michael Fink
--
-- Init script for game database
--

--
-- database settings
--
pragma foreign_keys = on;


--
-- tables
--

-- account; contains authentication infos
create table account (
   'id' integer primary key autoincrement not null,
   'username' text not null,
   'active' integer not null,
   'password' text not null,
   'salt' text not null
);

-- player character;
create table player (
   'id' integer primary key autoincrement not null,
   'uuid' text unique not null,
   'name' text unique not null,
   'pos' text not null
);

-- link table from player to account;
create table player_account_lnk (
   'player_id' integer not null,
   'account_id' integer not null,
   foreign key(player_id) references player(id),
   foreign key(account_id) references account(id)
);
