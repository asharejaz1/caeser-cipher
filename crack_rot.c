#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

/* lengths of some strings that we need */
/* length of words and line (when reading from file) both in bytes/chars */
#define MAX_WORD_LEN 30
#define MAX_LINE_LEN 120
/* max number of words in an array, same as max number of words in a sentence */
#define MAX_ARR_WORDS 30
// magic constant; max number of words supported in dictionary; works with given words.txt
#define MAX_WORDS_IN_DICT 25144
// BETTER use get_dictionary_len() below; needs a tmp file
#define TMPNAME "qlen.txt"
/* Global variables */
/* Command line options: */
int verbose = 0, help = 0, debug = 0, num = 0;
char *filein = NULL;


/* Caesar cipher of rotation-by-N-letters */
/*  covers only lower case letters */

void rotN(char *str, int n) { // NB: str will be modified in-place
  char *p;
  for (p=str; *p != '\0'; p++) {
    int currChar = (int)*p;
    if (currChar >= 'a' && currChar <= 'z') {
      currChar = currChar+n;
      if (currChar > 'z') {
	currChar = currChar-26;
      }
      *p = (char)currChar;
    }
  }
}

/* break up a string into an array of strings (the words), using space a separator */
char **arr;

char **str_to_arr_words(char *str)
{
 	int counter = 0;
	int spaces = 0;

  //counting number of words in the string by counting the total spaces
	while (str[counter] != '\0')
	{
	if (str[counter] == ' ')
	{
	    spaces++;
	}
	counter++;
	}
  //total number of words = spaces + 2
	int numWords = spaces + 2;
	size_t bufsize = 50;
	size_t characters;

  //allocating memory for the array to store each word from the string
  //the allocation size depends upon the number of words counted above 
	int j = 0;
	arr = (char **)malloc(numWords * sizeof(char*));
	while (j < numWords)
	{
	arr[j] = (char *)malloc(bufsize * sizeof(char));
	if (arr == NULL)
	{
	    perror("Unable to allocate buffer");
	    exit(1);
	}
	j++;
	}
	arr[numWords] = NULL;


	int wordNo = 0;
	int idx = 0;
	int k = 0;

  //storing elements from the string in the array until the pointer to string reaches null terminator
	while (str[k] != '\0') {
  
  //once the string pointer reaches a space, go to the next index
	if (str[k] == ' ') {
    //store null terminator at the end after storing the whole word
	    arr[wordNo][idx++] = '\0';
	    idx = 0;
	    wordNo += 1;
	    k++;
	    continue;
	}

	arr[wordNo][idx++] = str[k];
	k++;        
	}
	arr[wordNo][idx++] = '\0';

	return arr;
}

/* print all words contained in an array of words; one per line; only used for testing */
void show_arr_words (char **arr_words) {	
  //print all words from the arr_words array
	int i = 0;
	while(arr_words[i+1] != NULL) {
	    printf("%s\n",arr_words[i]);
	    i++;
	}
}


size_t get_dictionary_len(char *fname) {
  char cmd[MAX_WORD_LEN], file[MAX_WORD_LEN];
  int len;
  FILE *fd;
  sprintf(cmd, "wc -l %s > %s", fname, TMPNAME) ;
  if (verbose) fprintf(stderr, ".. SYSTEM CALL: '%s'\n", cmd);
  system(cmd);
  fd = fopen(TMPNAME, "r");
  if (fd==NULL) {
    fprintf(stderr, "Cannot open file %s\n", TMPNAME);
    exit(EXIT_FAILURE);
  }
  fscanf(fd, "%d %s", &len, file);
  if (verbose)  fprintf(stderr, ".. Dictionary length = %d\n", len);
  return (size_t)len;
}


char *fname = "words.txt";

char *dictArr[MAX_WORDS_IN_DICT];
// char *dictArr[get_dictionary_len(fname)];

/* Read the contents of the dictionary file in @fname@ line by line. */
/* Return a pointer to an array of all words in the dictionary. */

//recieves the name of the file through char-array pointer
char** read_dictionary(char *fname) {
    char ch[9];
    int count = 0;

    //store the name of the file into ch
    while (*fname != '\0') {
        ch[count] = *fname;
        count++;
        *fname++;
    }

    //open the file in read-mode
    FILE *fpoi;
    fpoi = fopen(ch, "r");

        
    count = 0;
    char *arr;

    
    char *buffer;
    size_t bufsize = 50;
    size_t characters;

    int j = 0;
    
    //allocating memory to the dictArr to store each word from the dictionary into the array
    //allocation size depends upon the total number of words in the dictionary file
    while(j < MAX_WORDS_IN_DICT) 
    {
        dictArr[j] = (char *)malloc(bufsize * sizeof(char));
        if( dictArr[j] == NULL)
        {
            perror("Unable to allocate buffer");
            exit(1);
        }
        j++;
    }
    
    //allocating memory for the buffer
    //buffer memory allocation size depends on the maximum number of words in each line of the dictarr
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    
    
    count = 0;
    
    //get the content from the dictionary file line by line using getline method
    //storing each line directly into the dictArr character by character
    while ((characters = getline(&buffer, &bufsize, fpoi)) != -1) 
    {
        int j = 0;
        while(buffer[j] != '\0') {
            dictArr[count][j] = buffer[j];
            j++;
        }
        dictArr[count][j] = '\0';
        count++;
    }

    free(buffer);
    fclose(fpoi);
    return dictArr;
}



//a manually built function to compare strings used in try_key
int compare_strings(char a[], char b[])
{

    int c = 0;

    while (a[c] == b[c]) {
        if (a[c] == '\0' || b[c] == '\0')
            break;
        c++;
    }

    if (a[c] == '\0' && b[c+1] == '\0') {
        return 0;
    }
    return -1;
}

/*
  Use the above rotation cipher with @key@ on the cipher text @text@ and check that
  all resulting words are elements in the dictionary @dict@.
  If so, @key@ is the decryption key.
*/

//called by the crack_rot function
int try_key(char **dict, char **text, int key) { 

    int counter = 0;
    int i = 0;

    //decrypting each word character by character by the key given from crack_rot function
    while (text[i+1] != NULL) {
        char temp[50];    
        int j = 0;
        while(text[i][j] != '\0') {         
          //use modulus so that the character+key should not exceed the ascii value 122 ('z') and loops back at the ascii value of 'a'    
            if (text[i][j] + key > 122) {
                temp[j] = ((text[i][j] + key) % 123) + 97;
            } else {
                temp[j] = text[i][j] + key;
            }
            j++;
        }
        temp[j] = '\0';
      
      //after decryption, compare the word to the words in dicitionary
        int ctr = 0;
        int isFound = 0;
        while(ctr < MAX_WORDS_IN_DICT) {
            int res = compare_strings(temp, dict[ctr]);
            if (res == 0) {
                //if the words matches, break the loop to apply encryption and match the next word from the cipher text
                isFound = 1;
                break;
            } 
            ctr++;
        }

        //if the word doesn't match, that means it is not the valid key, return to the crack_rot
        if(isFound == 0) {
            return -1;
        }
        i++;
    }
    return key;  
}

/* 
   Try to crack a rotation cipher by a brute force search: try to map all words
   in the cipher text to words in a dictionary, trying all possible keys.

   @param: dict ... dictionary of words to check against
   @param: text ... cipher text as a sequence of words (all lower case)
   @return: the rotation value that decrypts the cipher text
*/

//main function calling try_key
int crack_rot(char **dict, char **text) {	
    int result;
  //giving try_key, keys from 1 to 27 one by one
	for (int key = 1; key < 27; key++) {        
	    result = try_key(dict, text, key);

      // if the result is not -1 (key is found by try_key)
      // then return the encrpytion key
      // key = decryption key 
      // 26-key = encryption key
        if (result != -1) {
            return 26-key;
        }
    }
    // if none of the key decrypts the cipher text, return -1 
    return -1;
	

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// MAIN fct

int main (int argc, char **argv) {
  char *str;
  int key, cracked;

  // see: man 3 getopt for docu and an example of command line parsing
  { // see the CW spec for the intended meaning of these options
    int opt;
    while ((opt = getopt(argc, argv, "hvdf:")) != -1) {
      switch (opt) {
      case 'v':
	verbose = 1;
	break;
      case 'h':
	help = 1;
	break;
      case 'd':
	debug = 1;
	break;
      case 'f':
	filein = (char*)malloc(sizeof(char)*(strlen(optarg)+1)); 
	strcpy(filein, optarg);
	break;
      default: /* '?' */
	fprintf(stderr, "Usage: %s [-h] [-v] [-d] [-f <filename>] [<key> <plaintext>] \n", argv[0]);
	exit(EXIT_FAILURE);
      }
    }
  }

  if (verbose) {
    fprintf(stdout, "Settings for running the program\n");
    fprintf(stdout, "Help is %s\n", (help ? "ON" : "OFF"));
    fprintf(stdout, "Verbose is %s\n", (verbose ? "ON" : "OFF"));
    fprintf(stdout, "Debug is %s\n", (debug ? "ON" : "OFF"));
    if (filein)  fprintf(stdout, "Input file = %s\n", filein);
  }
  if (help) {
    fprintf(stdout, "%s: crack a rotation cipher, by finding the rotation key, given a plain and cipher text and using a dictionary\n", argv[0]);
    fprintf(stderr, "Usage: %s [-h] [-v] [-d] [-f <filename>] [<key> <plaintext>] \n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  if (optind < argc) { // Case: encrypting text with a key; for test case generation!
    if (verbose)  fprintf(stderr, "Encrypting provided input string with provided key\nUsage: %s [options] <key> <string>\n", argv[0]);
    str = (char*)malloc(strlen(argv[optind]));
    strcpy(str, argv[optind]);
    key = atoi(str);
    strcpy(str, argv[optind+1]);
    fprintf(stderr, "Rotation value:  '%d'\n", key);
    fprintf(stderr, "Input:  '%s'\n", argv[2]);
    rotN(str, key);
    fprintf(stderr, "Output: '%s'\n", str);
  } else {
    if (filein) { // CASE: with -f <filein> options, cipher-texts should be read from the file <filein> line by line
      char *line, *word=NULL;
      char **dict, **arr_words;
      int i, j, cracked;
      size_t len;
      FILE *fp;
      if (verbose)  fprintf(stderr,"Reading cipher texts from file %s ...\n", filein);

      // exit(EXIT_FAILURE);
      // the code below demonstrates how to use the functions above to crack the key
      // when reading cipher-text from filein with the -f option
      // AFTER implementing each of the functions, UNCOMMENT the exit() line above to test hard-coded input

      dict = read_dictionary("words.txt"); 
      fp = fopen(filein, "r");
      if (fp==NULL) {
        fprintf(stderr, "Cannot open file %s\n", filein);
        exit(EXIT_FAILURE);
      }
      line = (char*)malloc(sizeof(char)*MAX_LINE_LEN);
      while (getline(&line, &len, fp) != -1) {
        if (line[strlen(line)-1]=='\n')        // strip newlines
          line[strlen(line)-1]='\0';
        if (debug)   	
          fprintf(stderr, "line = %s\n", line); 
        if (verbose)   	
          fprintf(stderr, "Input cipher text:    '%s'\n", line);
        arr_words = str_to_arr_words(line);
        if (verbose)   
          show_arr_words(arr_words);
        cracked = crack_rot(dict, arr_words);
        if (cracked!=-1) {
          char decrypt[MAX_LINE_LEN];
          fprintf(stdout, "Key: %d\n", 26-cracked);
          strcpy(decrypt, line);
          rotN(decrypt, 26-cracked);
          fprintf(stdout, "Testing\nCipher text:  '%s'\nCracked text: '%s'\nwith cracked key: %d\n",
            line, decrypt, 26-cracked);
        } else {
          fprintf(stdout, "NO key found\n");
        }
      	// --------------------------------------------------------------
      }
      // free(dict);
      free(line);
      // fclose(fp);
    } else { // CASE: you can use this section for hard-coded test cases, just to get started with coding
      
      if (verbose)  fprintf(stderr, "Cracking rotation cipher for fixed plain- and cipher-text below.\n");
      /* ------------------------------------------------------- */
      /* Hard-coded inputs                                       */
      /* ------------------------------------------------------- */
      //char str0[] = "the word is this";
      //char exp0[] = "gur jbeq vf guvf";
      // from "Wives and Daughters": https://www.gutenberg.org/cache/epub/4274/pg4274.txt
      //char str0[] = "the little town fade away into country on one side close to the entranceway of a great park";
      //char exp0[] = "gur yvggyr gbja snqr njnl vagb pbhagel ba bar fvqr pybfr gb gur ragenaprjnl bs n terng cnex";
      //char str0[] = "erroneous code is the source of all evil";
      //char exp0[] = "yllihyiom wixy cm nby miolwy iz uff ypcf";
      char str0[] = "the quick brown fox jump over the lazy dog";
      char exp0[] = "gur dhvpx oebja sbk whzc bire gur ynml qbt";
      char **arr_words;
      char **dict;
      int key = 13, dict_len, arr_len;

      // -----------------------------------------------------------------------------
      // exit(EXIT_FAILURE);
      // the code below demonstrates how to use the functions above to crack the key
      // AFTER implementing each of the functions, UNCOMMENT the exit() line above to test hard-coded input
      dict = read_dictionary("words.txt"); // char** 
      // show_arr_words(dict);
      // fprintf(stderr, "Input:    '%s'\n", str0);
      arr_words = str_to_arr_words(exp0);
      if (verbose)   
        show_arr_words(arr_words);
      cracked = crack_rot(dict, arr_words);
      if (cracked!=-1) {
        char decrypt[MAX_LINE_LEN];
        fprintf(stdout, "Key: %d (expected %d)\n", 26-cracked, key);
        strcpy(decrypt, exp0);
        rotN(decrypt, 26-cracked);
        fprintf(stdout, "Testing\nCipher text:  '%s'\nCracked text: '%s'\nPlain text:   '%s'\nwith cracked key: %d\n",
          exp0, decrypt, str0, 26-cracked);
	      if (strcmp(decrypt,str0)==0)
	        fprintf(stdout, "OK decrypted text.\n");
	      else
	        fprintf(stdout, "WRONG decrypted text.\n");
      } else {
	      fprintf(stdout, "NO key found\n");
      }
      // free(dict);
      
      /*  Example of encryption: 
	  fprintf(stderr, "Rotation value:  '%d'\n", key);
	  fprintf(stderr, "Input:    '%s'\n", str0);
	  rotN_in_c(str0, key);
	  fprintf(stderr, "Output:   '%s'\n", str0);
	  fprintf(stderr, "Expected: '%s'\n", exp0);
	  if (strcmp(str0,exp0) == 0) {
	  printf(".. OK\n");
	  } else {
	  printf("** WRONG\n");
	  }
      */
    }
  }
  exit(EXIT_SUCCESS);
}
