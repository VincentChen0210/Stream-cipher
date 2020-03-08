//Author: Vincent Chen
#include <iostream>

int main();

char *encode( char *plaintext, unsigned long key);
char *decode( char *ciphertext, unsigned long key );
bool bitmask( unsigned long key, int k);
void ascii_armour( char input[], char output[], unsigned int pos, unsigned int aapos );
void unascii_armour( char input[], char output[], unsigned int pos, unsigned int aapos );
void scramble( char text[], unsigned long key, unsigned int len_st, char scr[]);

int main() {
	char str[]{"t43f9h934f974h9fu293uf293fh24uh"};
	unsigned long key{43381943};

	std::cout << str << std::endl;

	char *ciphertext{ encode( str, key )};

	std::cout << ciphertext << std::endl;

	char *plaintext{ decode ( ciphertext, key )};

	std::cout << plaintext;

	delete[] plaintext;
	plaintext = nullptr;
	delete[] ciphertext;
	ciphertext = nullptr;

	return 0;
}

char *encode( char *plaintext, unsigned long key) {
	char ch{plaintext[0]};
	unsigned int len_pt{0};
	unsigned int len_null{0};
	unsigned int aapos{0};

	while ( ch != '\0') {
		len_pt++;
		ch = plaintext[len_pt];
	}

	if (len_pt % 4 != 0) {
		len_null = 4 - (len_pt % 4);
	}

	char newtext[len_pt + len_null]{0};
	char s[len_pt + len_null + 1]{0};
	char *a_a{new char[ 5 * (len_pt + len_null) / 4 + 1]{0}};

	for (std::size_t t{0}; t < len_pt + len_null; ++t) {
		if (t >= len_pt) {
			newtext[t] = '\0';
		} else {
			newtext[t] = plaintext[t];
		}
	}

	scramble( newtext, key, len_pt + len_null, s);

	for (std::size_t x{0}; x < (len_pt + len_null); ++x) {
		if (x % 4 == 0) {
			ascii_armour( s, a_a, x , aapos );
			aapos += 5;
		}
	}
	return a_a;
}

void ascii_armour( char input[], char output[], unsigned int pos, unsigned int aapos ) {
	unsigned int sum{(static_cast<unsigned char>(input[pos]) << 24) + (static_cast<unsigned char>(input[pos + 1]) << 16) + (static_cast<unsigned char>(input[pos + 2]) << 8) + static_cast<unsigned char>(input[pos + 3])};
	unsigned char temp{0};

	for (std::size_t i{aapos}; i < aapos + 5; ++i) {
		output[i] = sum % 85 + '!';
		sum /= 85;
	}

	temp = output[aapos];
	output[aapos] = output[aapos + 4];
	output[aapos + 4] = temp;

	temp = output[aapos + 1];
	output[aapos + 1] = output[aapos + 3];
	output[aapos + 3] = temp;

}

bool bitmask (unsigned long key, int k) {
	unsigned long mask{1};

	mask = mask << k;

	return key & mask;
}

void scramble( char text[], unsigned long key, unsigned int len_st, char scr[]) {
	unsigned int i{0}, j{0}, k{0}, r{0};
	unsigned char s[256]{0}, rbits[256]{0}, temp{0};

	for (std::size_t t{0}; t < 256; ++t) {
		s[t] = t;
	}

	for (std::size_t x{0}; x < 256; ++x) {
			k = i % 64;
			j = (j + s[i] + bitmask(key, k)) % 256;
			temp = s[i];
			s[i] = s[j];
			s[j] = temp;
			i = (i + 1) % 256;
		}

		for (std::size_t y{0}; y < (len_st); ++y) {
			i = (i + 1) % 256;
			j = (j + s[i]) % 256;
			temp = s[i];
			s[i] = s[j];
			s[j] = temp;
			r = (s[i] + s[j]) % 256;
			rbits[r] = s[r];
			scr[y] = text[y] ^ rbits[r];
		}
}

void unascii_armour( char input[], char output[], unsigned int pos, unsigned int aapos) {
	unsigned int sum{0};
	unsigned char temp{0};
	unsigned int base{1};
	unsigned char convert{0};

	temp = input[aapos];
	input[aapos] = input[aapos + 4];
	input[aapos + 4] = temp;

	temp = input[aapos + 1];
	input[aapos + 1] = input[aapos + 3];
	input[aapos + 3] = temp;

	for (std::size_t i{aapos}; i < aapos + 5; ++i) {
		input[i] = static_cast<unsigned char>(input[i]) - '!';
		sum = sum + static_cast<unsigned char>(input[i]) * base;
		base = base * 85;
	}

	for (std::size_t i{pos}; i < pos + 4; ++i) {
		output[i] = sum | convert;
		sum >>= 8;
	}

	temp = output[pos];
	output[pos] = output[pos + 3];
	output[pos + 3] = temp;

	temp = output[pos + 1];
	output[pos + 1] = output[pos + 2];
	output[pos + 2] = temp;
}

char *decode ( char *ciphertext, unsigned long key ) {
	char ch{ciphertext[0]};
	unsigned int len_ct{0};
	unsigned int pos{0};

	while ( ch != '\0') {
		len_ct++;
		ch = ciphertext[len_ct];
	}

	char output[len_ct / 5 * 4 + 1]{0};
	char scr[len_ct + 1]{0};

	for (std::size_t t{0}; t < len_ct; ++t) {
		scr[t] = ciphertext[t];
	}

	for (std::size_t t{0}; t < len_ct; ++t) {
		if (t % 5 == 0) {
			unascii_armour( scr, output, pos, t );
			pos += 4;
		}
	}

	char *s{new char[len_ct / 5 * 4 + 1]{0}};

	scramble( output, key, len_ct / 5 * 4, s );

	return s;
}
