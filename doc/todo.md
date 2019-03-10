TODO								
=====
## March 2019
Maybe figure out a better way to do track TODO?

Should add paragrafs to README:
```
## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.
```

maybe this one too:
```
### And coding style tests

Explain what these tests test and why

```
Give an example
```
```

Should update CLI to output 
```    <program>  Copyright (C) <year>  <name of author>

    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
```
And add the commands for it.

Also add the license terms to each of my source files.

## OLD
	[ ] MoveGenerator legality validation without by make/unmake?
	[ ] Check move counter for more perftests.
		[ ] Castle counting isn't correct. it's checking state, and state also changes when king moves.
	[/] MoveGenerator, count move types
		[x] promotions
		[x] captures
		[ ] en passants // missing tests
		[x] check
		[ ] mates // missing tests
	[x] Material lists get out of sync at somepoint in time. - think I have fixed this now?
		[x] Material list unmake doesn't remove captured pieces
	[x] fix memcpy issues when building with clang & gcc, why only in material but not in board?
	[ ] move blog?
	[ ] get last error error handling
	[ ] FEN Unit Tests
		[x] Give error on too short FEN
	[x] Google tests?
	[x] gcc & make instead of visual studio?