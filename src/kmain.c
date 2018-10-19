/* The C function */
#define FB_GREEN     2
#define FB_DARK_GREY 8

char *fb = (char *)0x000B8000;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	fb[i] = c;
	fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void kmain() {
	char text[] = "It works!";
	int i = 0;
	for (; text[i]; i++) {
		fb_write_cell(i * 2, text[i], FB_GREEN, FB_DARK_GREY);
	}
	for (i = i * 2; i < 500; i++) {
		fb_write_cell(i, ' ', FB_GREEN, FB_DARK_GREY);
	}
	while (1);
}
