#include <Arduino.h>
#undef F

#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define G 8
#define SEG1 9
#define SEG2 10
#define SEG3 11
#define SEG4 12
#define RESET 13
#define DELAY 500
#define DIRECTION 1
#define CC

#ifdef CC
#define SEG_SET(X, val) digitalWrite(X, (val >> (7 - X + A)) & HIGH)
#define SEG_ON(segX) digitalWrite(segX, LOW);
#define SEG_OFF(segX) digitalWrite(segX, HIGH);
#endif

#ifdef CA
#define SEG_SET(X, val) digitalWrite(X, !((val >> (7 - X + A)) & HIGH))
#define SEG_ON(segX) digitalWrite(segX, HIGH);
#define SEG_OFF(segX) digitalWrite(segX, LOW);
#endif

char text[] = "hello ";
int segs[] = {SEG1, SEG2, SEG3, SEG4};
int seg_count = sizeof(segs) / sizeof(segs[0]);
int char_count = strlen(text);
int start_index = DIRECTION ? 0 : char_count - 1 - seg_count;
int index = start_index;

uint8_t char2seg(char c) {
	switch (c) {
		case 'a': return B11101110;
		case 'b': return B00111110;
		case 'c': return B00011010;
		case 'd': return B01111010;
		case 'e': return B10011110;
		case 'f': return B10001110;
		case 'g': return B10111110;
		case 'h': return B00101110;
		case 'i': return B00001100;
		case 'j': return B01111000;
		case 'k': return B00001110;
		case 'l': return B00011100;
		case 'm': return B11101100;
		case 'n': return B00101010;
		case 'o': return B00111010;
		case 'p': return B11001110;
		case 'r': return B00001010;
		case 's': return B10110110;
		case 't': return B11100000;
		case 'u': return B00111000;
		case 'v': return B01000110;
		case 'y': return B01100110;
		case 'z': return B11011010;
		default: return 0;
	}
}

uint8_t *str2seg(char *str) {
	size_t len = strlen(str);
	uint8_t *seg = new uint8_t[len + 1];
	for (size_t i = 0; i < len; i++) {
		seg[i] = char2seg(str[i]);
	}
	return seg;
}

void set_seg(uint8_t val) {
	SEG_SET(A, val);
	SEG_SET(B, val);
	SEG_SET(C, val);
	SEG_SET(D, val);
	SEG_SET(E, val);
	SEG_SET(F, val);
	SEG_SET(G, val);
}

void seg_on(int segX) {
	for (auto i : segs) {
		SEG_OFF(i);
	}
	SEG_ON(segX);
}

uint8_t *seg_text = str2seg(text);

void setup() {
	Serial.begin(9600);
	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
	pinMode(D, OUTPUT);
	pinMode(E, OUTPUT);
	pinMode(F, OUTPUT);
	pinMode(G, OUTPUT);
	for (auto i : segs) {
		pinMode(i, OUTPUT);
		SEG_OFF(i);
	}
	pinMode(RESET, INPUT);
	set_seg(0);
}

void loop() {
	if (digitalRead(RESET) == HIGH) {
		index = start_index;
		set_seg(0);
		return;
	}
	auto start = millis();
	auto end = start;
	while ((end - start) <= DELAY) {
		for (auto segX : segs) {
			set_seg(0);
			seg_on(segX);
			set_seg(seg_text[((index + segX - segs[0]) + char_count) % char_count]);
		}
		end = millis();
	}
	if (DIRECTION) index++;
	else index--;
	if (index < -1 * seg_count || index >= char_count) index = start_index;
}