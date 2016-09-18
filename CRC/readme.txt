Nathan Guenther
n3197119
Program 2 Hill CRC
7-15-2014

Note: Inconsistent results across environments
Working Environment: UCF Eustis server

Steps to test:
1. Must use VPN to connect to UCF network (if on campus shouldn't need)
	a) For windows VPN connection via "Cisco AnyConnect Secure Mobility Client)
2. SFTP files to server my isolated home directory (/home/na752683)
	a) For windows SFTP via FileZilla
3. Terminal access
	a) For Windows remote terminal via PuTTY
4. Compile and run
	a) gcc crc.c -lm
	b)./a.out
	c) Alternatively run the pre-compiled exe
		i) ./nathangCRC.exe
5. Works in these situations
	1. Calculate CRC
	2. Verify CRC
	3. Calculate + Verify CRC
6. Memory issues if continuously looping menu with new tests


Thank you for all the help
