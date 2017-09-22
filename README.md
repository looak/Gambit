# Gambit Chess Engine

TODO								
=====
	[x] fix markup in readme...
	[ ] move blog?
	[ ] get last error error handling
	[ ] FEN Unit Tests
		[x] Give error on too short FEN

Dev Blog
=====
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
