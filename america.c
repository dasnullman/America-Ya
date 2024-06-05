#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> // This is a POSIX header
#include <stdbool.h>
#include <string.h> 
#include <errno.h> // This is a POSIX header

#include <getopt.h> // For getopt_long, this is a GNU extension. 

#define MESSAGE_CHIYO (const char*)"Hallo :D" 
#define MESSAGE_OSAKA (const char*)"America-ya!" 

#define CHIYO_CHANS_COUNT 10 // Taken from the scene 
#define CHIYO_CHAN_REPITITION_COUNT 5 

#define CHIYO_CHAN_FINISHED_MESSAGE (const char*)"Welcome to our country!"
#define CHIYO_CHAN_FALL_COUNT 1 // Cannot exceed the total chiyo chans count 

#define MESSAGE_MAX_LENGTH 32

bool optionGood (struct option opt) {
  if (opt.name != 0) return true;
  if (opt.has_arg != 0) return true;
  if (opt.flag != 0) return true;
  if (opt.val != 0) return true;

  return false;
}

int main(int argc, char** argv) {
  int smileyflag = 0; // Whether or not to force print :D after the message.
  // 0 for unset and 1 for set

  char* chiyomessage = malloc(sizeof(char) * MESSAGE_MAX_LENGTH);
  char* osakamessage = malloc(sizeof(char) * MESSAGE_MAX_LENGTH);
  char* finishedmessage = malloc(sizeof(char) * MESSAGE_MAX_LENGTH);

  strcpy(chiyomessage, MESSAGE_CHIYO);
  strcpy(osakamessage, MESSAGE_OSAKA);
  strcpy(finishedmessage, CHIYO_CHAN_FINISHED_MESSAGE);

  long countchiyos = CHIYO_CHANS_COUNT, chiyorepitition = CHIYO_CHAN_REPITITION_COUNT, chiyofall = CHIYO_CHAN_FALL_COUNT;
  // They get supplied by strtol call.

  int c, oindex;

  static struct option long_options[] = {
    {"osaka_message", required_argument, 0, 'o'},
    {"chiyo_message", required_argument, 0, 'c'},
    {"chiyo_finished_message", required_argument, 0, 'f'},

    // Integral arguments

    {"count_chiyos", required_argument, 0, 'u'},
    {"repititions", required_argument, 0, 'r'},
    {"count_fallen", required_argument, 0, 'a'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0} // Marks the end of long_options
  };

  while ((c = getopt_long(argc, argv, ":s", long_options, &oindex)) != -1) {
    char* end;
    long value;
    
    switch (c) {
    case 's':
      // Smiley
      smileyflag = 1;
      break;

    case 'o':
      if (strlen(optarg) > MESSAGE_MAX_LENGTH) {
	fprintf(stderr, "Optarg %s with length of: %d exceeded max length of %d defaulting.\n", optarg, strlen(optarg), MESSAGE_MAX_LENGTH);
	continue;
      }
      
      strcpy(osakamessage, optarg);
      break;

    case 'c':
      if (strlen(optarg) > MESSAGE_MAX_LENGTH) {
	fprintf(stderr, "Optarg %s with length of: %d exceeded max length of %d defaulting.\n", optarg, strlen(optarg), MESSAGE_MAX_LENGTH);
	continue;
      }
      
      strcpy(chiyomessage, optarg);
      break;

    case 'f':
      if (strlen(optarg) > MESSAGE_MAX_LENGTH) {
	fprintf(stderr, "Optarg %s with length of: %d exceeded max length of %d defaulting.\n", optarg, strlen(optarg), MESSAGE_MAX_LENGTH);
	continue;
      }
      
      strcpy(finishedmessage, optarg);
      break;

    
    // Integral long arguments

    case 'u':
      errno = 0; // Reset just in case we had an error
      value = strtol(optarg, &end, 10); // 10 for base 10.
      
      if (end == optarg) {
	fprintf(stderr, "Could not find digits for chiyo-chan count, using default of %d\n", countchiyos);
	break;
      } else if (*end != '\0') {
	fprintf(stderr, "Integral conversion error for chiyo-chan count, characters found after count: %s\n", end);
	break;
      }

      countchiyos = value;
      
      break;

    case 'r':
      errno = 0;
      value = strtol(optarg, &end, 10);

      if (end == optarg) {
	fprintf(stderr, "Could not find digits for chiyo-chan repitition count, using default of %d\n", chiyorepitition);
	break;
      } else if (*end != '\0') {
	fprintf(stderr, "Integral conversion error for chiyo-chan repitition count, characters found after count: %s\n", end);
	break;
      }

      chiyorepitition = value;
      
      break;
    

  case 'a':
    errno = 0;
    value = strtol(optarg, &end, 10);

    if (end == optarg) {
      fprintf(stderr, "Could not find digits for chiyo-chans fallen count, using default of %d\n", chiyofall);
      break;
    } else if (*end != '\0') {
      fprintf(stderr, "Integral conversion error for chiyo-chans fallen count, characters found after count: %s\n", end);
      break;
    }

    chiyofall = value;
    
    break;

  case 'h':

    printf("America-ya program. Copyright (C) Nullman 2024.\n"
	   "This program is licensed under the GNU General Public License Version 3\n\n\n"


	   "Options:\n"
	   "\t--help\tDisplays this information\n\n"
	   "\t-s Whether or not to print a smiley face after the message, only set if you want to append a smiley to a custom chiyo message\n"
	   "\t--osaka_message <message>\tThe message the Osaka will say at the start of program execution\n"
	   "\t--chiyo_message <message>\tThe message the chiyo chans will say post osaka message\n"
	   "\t--chiyo_finished_message <message>\tThe message the chiyo chans will say post saying the chiyo message(s)\n\n"

	   "Integral options, all of these take an integer as their argument\n\n"

	   "\t--count_chiyos <count>\tThe number of chiyos who will say the chiyo message per repitition.\n"
	   "\t--repitions <count>\tThe number of times the chiyos will repeat the chiyo message.\n"
	   "\t--count_fallen <count>\tThe number of chiyos that will fall at the last repitition, cannot exceed count_chiyos.\n\n");

    return 0;
    
    break;

  case '?':
    // We encountered an argument that doesn't exist

    if (isprint(optopt)) {
      fprintf(stderr, "Unknown option `-%c'.\n", optopt);
    } else {
      fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
    }

    return 1;
    
    break;

  case ':':
    // You know when they say jokes but it be real?

    
    struct option opt = {1, 0, 0, 0};
    bool foundopt;

    int i = 0;

    while ( optionGood (opt)) {
      opt = long_options[i];
      if (opt.val == optopt) {
	// Found which long option this is.
	fprintf(stderr, "Option `--%s' wants an argument but didn't get one\n", opt.name);
	foundopt = true;
      }

      i++;
    }

    if (!foundopt)
      fprintf(stderr, "Option `-%c' wanted an argument but didn't get one.\n", optopt);


    return 1;
    
    break;

  default:
    abort();
  }

  
  }

  if (chiyofall >= countchiyos) {
    // Because they can be set whenever, we must check after they're set
    // Or enforce ordering through a legacy getopt mode

    fprintf(stderr, "Fallen Chiyos count is %d which is more than or equivalent to chiyo count of %d\n", chiyofall, countchiyos);
    return 1;
  }
  
  printf("%s\n", osakamessage);

  // First reformulate the chiyo message if we have the smiley
  if (smileyflag)
    strcat(chiyomessage, " :D");

  strcat(chiyomessage, " ");
  
  // Time for the chiyo message. Let's formulate a buffer

  int sizechiyomessage = strlen(chiyomessage);
  int sizechiyobuffer = sizeof(char) * ((sizechiyomessage * countchiyos) + countchiyos) + 2;
  
  char* chiyobuffer = malloc(sizechiyobuffer);

  int chiyobufferindex = 0;
  
  for (int i = 0; i < countchiyos; i++) {
    strcat(chiyobuffer, chiyomessage);
    strcat(chiyobuffer, " ");
  }

  chiyobuffer[sizechiyobuffer - 2] = '\n';
  chiyobuffer[sizechiyobuffer - 1] = '\0';
  
  // One chiyobuffer is one repitition

  // chiyorepitition - 1 to handle the last on our own.
  int repititionsLeft = chiyorepitition;
  while (repititionsLeft > 1) {
    printf("%s", chiyobuffer);
    repititionsLeft--;
  }

  for (int i = 0; i < countchiyos - chiyofall; i++) {
    if (i == ((countchiyos - chiyofall) - 1))
      printf("%s", chiyomessage);
    else
      printf("%s ", chiyomessage);
    
  }

  // Now handle the fallen chiyos

  for (int i = 0; i < chiyofall; i++) {
    printf(" Uehhhh");
  }
  printf("\n");

  // Time for finished message

  printf("%s\n", finishedmessage);

  return 0;
}

