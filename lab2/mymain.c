mymain(int argc, char *argv[], char *env[]) {
	int i;
	/*my*/printf("in mymain(): argc=%d\n", argc);

	for(i = 0; i < argc; i++) {
		/*my*/printf("argv[%d] = %s\n", i, argv[i]);
	}

	i = 0;
	while(env[i] != NULL) {
		/*my*/printf("env[%d] = %s\n", i, env[i]);
		i++;
	}

	/*my*/printf("--------------testing YOUR /*my*/printf() ---------------\n");
	/*my*/printf("this is a test\n");
	/*my*/printf("testing a=%d b=%x c=%c s=%s\n", 123, 123, 'a', "testing");
	/*my*/printf("string=%s, a=%d  b=%u  c=%o  d=%x\n", "testing string", -1024, 1024, 1024, 1024);
	/*my*/printf("mymain() return to main() in assembly\n"); 
}