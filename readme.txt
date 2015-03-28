====================
Instructions
Build and execute
====================

--------------------
Client
--------------------
- build 
	make client
- exec
	client arg1 arg2

arg1: host server address

arg2: requested URL
	note that when arg2 is "@stats", will get the statistics of the server cache	

--------------------
Sever
--------------------
- build 
	make server
- exec
	server arg1 arg2

arg1: cache policy	
	1 -> FIFO
	2 -> LRU
	3 -> MAXS
	4 -> Random

arg2: cache size (in MB)