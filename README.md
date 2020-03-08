# Stream cipher

Refer to Wikipedia's explanation:

Stream cipher: https://en.wikipedia.org/wiki/RC4 

Binary-to-text encoding: https://en.wikipedia.org/wiki/Binary-to-text_encoding

### Introduction
This program is a RC4 stream cipher with addition binary-to-text encoding written in C++.
In addition, the code for both the encoding and decoding parts are included, which are both operated one after another to show that the program functions as intended.

### Encoding
There are 2 main components to the encoding segment of this program: the stream cipher and the binary-to-text encoding.

In the stream cipher section, a pseudorandom arangement of the bits from 0 to 255 is created in the form of an array from the 8-bit unsigned integer keycode.
Then, the algorithm 'pseodurandomly' finds a value between 0 and 255 from the counter variables from the first part of the stream cipher.
This value will then be bitwise exclusive ORed with the next byte of the plaintext to create the next bit for the ciphertext. This process is repeated for the entire length of the plaintext.

The issue arises when the ciphertext (as ASCII characters) contains characters which are not printable. Therefore, it becomes necessary to convert the 256-based characters into a new base which will be easier to output.
For this project, a 85-bit base was used, as 256 to the 4th power is close to 85 to the 5th power. This range begins from ASCII value 41 (the character !) to ASCII value 126 (the character ~), as all characters in this range are recognizably printable.
This was done by converting a group of 4 characters (in base 256) to 5 ciphertext characters (in base 85), which is done by summing the 4 bits into a 16-bit unsigned integer, then repeated dividing by 85 to get the corresponding base 85 characters.

### Decoding
For the decoding part of the program, the reverse of the above method was done. The base 85 characters is first converted to base 256 ASCII characters. After, the characters are exclusive ORed with the same permutated array from the same keycode that was used to encode the plaintext.
