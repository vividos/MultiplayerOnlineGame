--
-- MultiplayerOnlineGame - multiplayer game project
-- Copyright (C) 2008-2014 Michael Fink
--
-- 0001: Init test accounts
--

-- michi pass1
insert into account
   ('id', 'username', 'active', 'password', 'salt')
   values (1, 'michi', 1, '88D60A34770813E4B5FB7735DBDB1FF1FAEC2F3A6376926D6A72F8CD26BB2ADAE1DA8A41F6B3B019863738C7E9AE168CA1A79A52269619E65D9C234B395A08BC41A5776F0FA92095CFF4A1AAEC9955F274A1C76E547CB300434A12D3E1A0AEA4C37E4C67EC7369A7FF2D5FBACCF1AC4A1B3C3932607F661D2E1A4DB620612D00', '26F3DF6AD81481E2761DB9EFE56E3BD8');

insert into player
   ('id', 'uuid', 'name', 'pos')
   values (1, '09da8f95-b7ee-4238-a13b-89db0c19308d', 'Vividos', '10.0,0.0,10.0');

insert into player_account_lnk
   ('player_id', 'account_id')
   values (1, 1);
