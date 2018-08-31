-# Gambit Chess Engine

[![Build Status](https://travis-ci.org/looak/Gambit.svg?branch=master)](https://travis-ci.org/looak/Gambit)

TODO								
=====
	[x] Material lists get out of sync at somepoint in time. - think I have fixed this now?
	[ ] fix memcpy issues when building with clang & gcc, why only in material but not in board?
	[ ] move blog?
	[ ] get last error error handling
	[ ] FEN Unit Tests
		[x] Give error on too short FEN
	[x] Google tests?
	[x] gcc & make instead of visual studio?

Dev Blog
=====
### (2018-08-30)
#### Long time since last entry...

	Guess I haven't been motivated to code on this project, that has recently changed. With new computer 
	purchase and just the urge to do something small isntead of the mastodont projects we have at work.

	Anyway, fixed an issue with my material grid got out of sync after promotion. Actually fixed two issues;
	One where I was writing to the a pointer which had moved since it was pointing to an array. And another
	where I never removed my pawns or added the promoted piece during a promotion.

	There are still issues though, quite clearly when running the Perft Tests and Move Generation, something
	to look into this weekend!

### (2017-12-27)
#### To many things to keep in sync
	Loads of things have been happening the past few days in my application. But I haven't felt like updating
	my blog.

	Currently I'm having issues with the Material becoming out of sync after captures and unmakes. Haven't 
	found exactly where yet but I'm getting closer.

	Though I could almost forsee this issue when writing my implementation, I have a ton of different ways of
	basically the same data which is asking for these type of out of sync issues. So, maybe I'll rreconsider 
	my implementation and redo it at some point in time.

### (2017-12-21)
#### Diving into the undo rabbit hole
	To fix some of the issus I wanted to write more tests. But I felt limited in not being able to undo moves
	in my test setup. So now I started writing Undo or 'Unmake' as I'm calling it in the engine.

	Update:
	Managed to fix unmake for a pawn chain and castling. Shouldn't be too much work to continue implementing 
	other special rules such as capture, promote and enpassant. Feels good at least to get this far! Now I can
	focus on what I was yesterday, fixing castling, but that's probably a task for tomorrow.

	Not super happy about my implementations, could probably do something cooler with nested nodes of
	different types. Maybe I'll be able to have reasons to do it later when I start looking at capture or 
	promotion.


### (2017-12-20)
#### Two updates in one day?
	Well, I'm on vacation and have really been longing to get going with this project again. I converted all my
	existing unit tests to gtest and added a few new ones too. Travis is awesome, my new BFF. 

	Coding wise, I've not been able to do much. I can't believe how complicated the code is to read in some
	places. Really need to do something about that. Been away from it for a month and now I'm having issues 
	understanding things. Says something about the code I write. Or just how much I over complicate things.
	For instance, I don't think I need to have both the Board & Bitboard, they're pretty much just duplicating
	eachothers behavior. I find the bitboards more powerfull and nice to do interesting comparisons on but they
	are hard to grasp and debug. It's weird to just see a giant value in the watch...

	The little time I did spend, I added some Perft test cases which obviously turned out to be wrong. So I 
	started testing the MoveGenerator and realized that my Castling isn't according to the rules of the game.
	So I updated my castling unit test and started looking at how to fix it...

### (2017-12-20)
#### Travis CI Green!!
	Haven't worked so much on this project in a while. But today and yesterday I spent most of my day hacking
	away at finalizing the CMake Migration (now I can compile in MSVC, CLion with MinGW, on unix with clang &
	gcc). Finally got everything running, I also added googletest and can run them from my IDE's but haven't
	been able to make Travis run them. That is the next step. Think I'll take a break from this for today 
	though, been working 12h straight on the same issues.	

### (2017-10-6)
#### Castling & En Passant
	The past few days I've been working on getting castling & en passant working. Not yet checking if I'm
	moving into check or not. This will be done in a step later. But the move generator picksup this too 
	so basically I can start testing this Perft as it's called. Might implement one just to try. Currently
	I can only do one depth 0 though.

	Oh I haven't done promotions yet, darn pawns with all their special cases. :)

	hmm... got a bug with enPassant.


### (2017-10-1)
#### Renovating living room.
	Melanie and I have been renovating the living room the past few days. We're doing some pretty descent
	progress with the wall putty, hilarious name btw. Wall Putty :D Going to paint it with the base color
	during the week. Probably on Wedneseday. Goal is to make the room green, my computer and desk is also 
	going to move into the living room. Probably for the better. 

	Preston & Aaton are doing super, they seem not to have any issues with the transition. Though our
	renovation hasn't been the most popular, nor the cleaning up after. But now it's basically just 
	painting left so I think they'll be just fine, they're getting a big-ass tree to climb in too, so they
	better be fine. :D

	The chess programming is going forward too, I've been able to get in a few hours during the weekend while
	putty(aaah, haha, I can't get over this word) has been drying. Got my move generation working, just need
	to add castling and en passant then I'm ready to start working on the evaluation, the brain. 
	
	Stupid special cases, ruining my overall very generic code, it's so pretty. Hurt a bit to add the pawn
	double step from starting position. Not super fond of that special case and how I'm handling it. En 
	passant and castling are going to be the same.

### (2017-9-28)
#### A few days have passed
	Aaton & Preston are moving in tomrrow, thinking of renaming them Tau & Myon (Elementary Particles). Who
	knows, time will tell. Been super busy at work and haven't hade enough motiviation to sit down and code
	at home after it. So been spending my time reading, trying to read some chess papers and chess books.
	Also watching break downs of "legendary" games between masters.

	Today I've been looking at adding a available moves method to my bitboard. Which the MoveGenerator can
	use later to push out moves from each piece. Though currently submitting it with a known issue where
	pawns will show up to have available attacks even though there doesn't sit a opponent piece in the 
	tested square, will fix this next time.

### (2017-9-24)
#### Productive weekend.
	Well not for my chess-coding project, but I did get some done with it to tonight. Wrapping my head around
	these bit boards was harder than I thought. But today I finally got it all working, fixed the bitboard
	console drawing too which was actually one of the reasons I got so confused. Odd thing, you think your
	debug data is trustworthy just to realize there is a bug in the debug-drawing. >.<
	
	Anyways been cleaning the apartment with Melanie to start renovating the living room some time next week.
	We'll see how much time I'll have for this project then too. Also visited Preston & Aaton for the "last"
	time this weekend. So they'll be moving in sometime next Week I believe. Crosing fingers. That was also
	one of the reasons I cleaned out my living room.

	Was quite interesting how quick it was to add the rook, bishop and queen once I had knight and king 
	figured	out. Basically wrapping the whole method with a do-while and it worked. :p The power of writing
	generic code I guess. :)
	

### (2017-9-22)
#### Overtime and weeknds
	Work is taking up most of my motivation to code. Or rather after work I feel exhausted and tierd of sitting
	in front of a computer and working. So progress the past few days have been slow. And it doesn't look like
	the workload is going down any time soon.

	Also I feel abit stuck about what I should do now, or what I should do next. I have the basics up. And I
	started writing some bitboards but not entierly sure what or where I should use them yet.

	Move Generation maybe?

	Mustered up some motivation to get started and once I'm going it's so much fun. Wrote some bitboard 
	operations. Started getting the hang of it, but I have a bug now where my Knight is attacking on the other
	end of the board. Hmm...

	Also my bitboard is mirrored, most likely due to how I print it out.

	But work tomorrow, working weekends now, that stage of the project, so better get some sleep.

### (2017-9-19)
#### Cleaning
	Feels better now.

### (2017-9-18)
#### Overwatch
	Why am I wasting time on this game? It just makes me uppset, I feel like I've wasted time which could have
	been better spent on for example coding chess.

	Feeling low... 23:15, descent time to go to bed.

	Was pretty easy to add the Knight once the king was working. Cleaned up the MovePiece and renamed it to
	MakeMove. Was reading about searching today. Will get my fingers dirty with that as soon as I can move 
	all the major and minor pieces and actually just read that pawns and the king don't count as pieces.

	Major Piecs being Queen & Rook
	Minor Pieces being Knight & Bishop
	Then King is a level of it's own.
	And Pawns are Pawns.

### (2017-9-17)
#### En Passant
	not really, but I can now move around my king. :) Also within the limits of what a king can do. The feedback is
	poor in my console application but that is OK for now.

#### Sister visiting
	...so maybe I'll work a bit later. Got the console visualization going, next step will be to make the king move around.

#### Trad Climbing
	Yea so a my buddy and I went to do some tradclimbing. It's not the easiest of places to reach, left home at about 
	9.45 and we did our first climb just before twelve. Then obviously it started raining on us so we called it and head 
	home again.

	Will try to write some visual representation now that I have a king and a board. Also partially done with the FEN
	deserialization. Will continue on that later when I start adding the state and logic behind it to the game.

	State:
	Castling state,
	En passant pawns,
	whos turn it is,
	ply counter.

### (2017-9-16)
#### Move them pieces!
	Well at least started and thinking about how the input should look, going with the standard algebraic notation (SAN)
	recognized by FIDE.
	
	1. e4 e5
	2. Nf3 Nc6
	3. Bb5 a6

![Chess Opening](https://upload.wikimedia.org/wikipedia/commons/thumb/b/b6/SCD_algebraic_notation.svg/242px-SCD_algebraic_notation.svg.png)
	
#### Presenting this work already?
	So at work we have this paper-reading-group, every now and then someone presents an interesting publication or 
	paper that they've read. To share and exchange knowledge. I haven't presented anything yet and mentioned that this 
	project and that I'm reading a bunch of publications about Chess Programming. So now I've basically promised 
	that I'm going to present a paper on this subject, it went so fast and I actually have a date for when I'm going 
	to do my presentation. 26th of October :o

#### Busy times...
	Not been able to work as much on my project as I might have wished. But I got to sit down an hour or two today,
	been working on piece placement and have a function that I'm quite happy with now. Wondering if I should remove 
	the validation and move the actual method to the public API. Or move the validation to some layer inbetween which 
	could be ignored.

	Got my tea and ready for another hour at least before I hit the hey, going trad climbing tomorrow.


### (2017-9-14)
#### So, here goes nothing. 
	Been the past few days on my spare time and between compiles at work reading up on writing a chess engine in c++11. 
	Found this amazing community online with a ton of information about chess engines. Which made me really excited and 
	wanted to start working on one of my own.

#### The important task of giving the project a Name.
	Yesterday I actually setup the Project on GitHub etc. Started out with calling it Gambit. We'll see if the name sticks. 
	Did a quick check online  if any other engine was called Gambit without finding anything so.

#### Insomnia
	Last night I had real issues falling a sleep, my head was spinning with ideas and of all
	the information I've been reading. I decided to go with test driven development of my 
	chess engine. I think it would be good approach for developing a chess engine and also
	to learn more indepth writing TDD.

	Started writing tests for setting up the board. Didnt get very var because I went and 
	looked at two cats I might adopt. Preston & Aaton, might keep the names. We'll see.