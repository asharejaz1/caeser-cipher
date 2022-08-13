Given: a cipher text (a text after application of a rotation cipher), and a dictionary of words, that covers all
words in the plain text;
Find: the key for the rotation cipher that can be used to decrypt the plain text, and print the key (if found)
together with the plain text.

Assumptions: The following assumptions can be made on the input text. All words in the original plain
text are lower case words and each word is contained in the given dictionary (in words.txt). The words
are separated by exactly one space symbol and there are no punctuation symbols in the plain text. The space
symbol is unmodified by the encryption/decryption (as can be seen from the given encryption function in the
template code). Note that the given encryption function can be used for both encryption and decryptions: if
the key for encrypting is N , then the key for decrypting is 26 − N .


Idea: The main idea of the algorithm to crack the cipher is to do a brute force search through a dictionary
for words, that, when encrypted, give the current word in cipher text. This needs to be checked for all
possible rotation values of 1–26. This is feasible, since typically only a few words will be mapped to a
cipher word for any of the possible keys. The correct key needs to map dictionary words to all of the words
in the cipher text.

An outer loop should iterate over all cipher words. In an
inner loop try all possible keys (1–26) on the current cipher word and check whether the resulting word is
in the dictionary. If this is the case, the key is a candidate for decrypting the entire cipher text. In this case,
use the candidate key to try and decrypt the entire ciphertext. If every decrypted word in the cipher text is in
the dictionary, then we can assume that the candidate key is valid decyption key, and we can terminate the
search. Return the key and print it as the result.

• int crack_rot(char **dict, char **text)
this is the top level function for cracking the rotation cipher; it takes the dictionary as an array of
strings (i.e. pointer to characters) and the cipher text, split into words, also represented as an array of
strings. It should return the key to use for decyption.

• int try_key(char **dict, char **text, int key)
this function takes the dictionary, as an array of words, the cipher text, also as an array of words, and
the candidate key as input, and tries do decipher each work int text with the key; if each deciphered
word is in the dictionary, we have found the key and return it as the result; otherwise a value of −1
should be returned.

• char** read_dictionary(char *fname)
this function takes the filename of the dictionary as input, reads all words from the file (one word per
line), and returns an array of words, containing all the words from the dictionay, as a result. Note:
This functions needs to allocate the memory for the dictionary it returns.

• void rotN(char *str, int n)
this is an implementation of the rotation cipher, and encripts the string str with the key n; it per-
forms the encryption in-place, i.e. the result of the execution is a modified string str containing the
ciphertext of the input.

• char** str_to_arr_words (char *str)
this is an auxiliary function that takes a string as an input, which is either a plain text or cipher text
string, breaks it into words and returns an array of these words as a result; the words in the input string
are separated by exactly one space, use only lower case letter, and there are no punctuation symbols
in the text.

• void show_arr_words (char **arr_words)
this is an auxiliary function that prints all words in the given array of words; it’s not needed for the
main functionality of the code, but very useful for testing.

The program should run from the command-line and use the following options
./crack_rot -f filein.txt -v
