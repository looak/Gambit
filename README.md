# Gambit Chess Engine

TODO								
=====
[ ] fix markup in readme...
[ ] move blog?
[ ] get last error error handling


Dev Blog
=====
### (2017-9-16)
#### Move them pieces!
	Well at least started and thinking about how the input should look, going with the standard algebraic notation (SAN) recognized by FIDE.
	
	1. e4 e5
	2. Nf3 Nc6
	3. Bb5 a6

	![alt text](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c7/Chess_pdt45.svg/26px-Chess_pdt45.svg.png "1. e4 e5 2. Nf3 Nc6	3. Bb5 a6")
	
#### Presenting this work already?
	So at work we have this paper-reading-group, every now and then someone presents an interesting publication or paper that they've read.
	To share and exchange knowledge. I haven't presented anything yet and mentioned that this project and that I'm reading a bunch of publications 
	about Chess Programming. So now I've basically promised that I'm going to present a paper on this subject, it went so fast and I actually
	have a date for	when I'm going to do my presentation. 26th of October :o

#### Busy times...
	Not been able to work as much on my project as I might have wished. But I got to sit down an hour or two today, been working on piece placement 
	and have a function that I'm quite happy with now. Wondering if I should remove the validation and move the actual method to the public API.
	Or move the validation to some layer inbetween which could be ignored.

	Got my tea and ready for another hour at least before I hit the hey, going trad climbing tomorrow.


### (2017-9-14)
#### So, here goes nothing. 
	Been the past few days on my spare time and between compiles at work reading up on writing 	a chess engine in c++11. Found this amazing	community 
	online with a ton of information about chess engines. Which made me really excited and wanted to start working on one of my	own.

#### The important task of giving the project a Name.
	Yesterday I actually setup the Project on GitHub etc. Started out with calling it Gambit. We'll see if the name sticks. Did a quick check online 
	if any other engine was called Gambit without finding anything so.

#### Insomnia
	Last night I had real issues falling a sleep, my head was spinning with ideas and of all
	the information I've been reading. I decided to go with test driven development of my 
	chess engine. I think it would be good approach for developing a chess engine and also
	to learn more indepth writing TDD.

	Started writing tests for setting up the board. Didnt get very var because I went and 
	looked at two cats I might adopt. Preston & Aaton, might keep the names. We'll see.
