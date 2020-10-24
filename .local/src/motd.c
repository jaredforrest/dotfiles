#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

int main(void) {

  /* Checks last login time : wtmp */

  /* Allocate memory for wtmp */
  struct utmp wtmp;

  /* Open wtmp */
  FILE *fp;
  fp = fopen("/var/log/wtmp", "rb");
  if (fp == NULL) { fprintf(stderr, "Failed to open wtmp\n");return(1);}

  /* Find size of wtmp */
  fseek(fp, 0L, SEEK_END);
  size_t wtmp_size = ftell(fp);

  /* Find number of logs */ 
  size_t lognum = wtmp_size / sizeof(struct utmp);
  
  /* Get username */
  char * uname;
  if ((uname = getlogin()) == NULL) { fprintf(stderr, "Failed to get username\n"); return(1);}

  /* When we find a matching login match = 1 */
  int match = 0;

/*


  if (!lognum) { fprintf( stderr, "First Login\n"); return(1);}
  
  /* We skip the most recent log as that is the current login *//*
  lognum--; 

  do {
    /* We are at the end of the file and we are reading it backwards.
     * 2 steps backward one step forward *//*
    fseek(fp, -2 * sizeof(struct utmp), SEEK_CUR);
    fread(&wtmp, sizeof(struct utmp), 1, fp);
    lognum--;
    int i;
    for(i = 0; i < 32; i++)
    {
        /* Check for username is current log value *//*
        if(*(uname+i) != wtmp.ut_user[i]) break;

        /* Check if at end of string *//*
        else if( !(*(uname+i)) || i==31) {match=1; break;}
    }
  }while(lognum && !match);*/
  fclose(fp);
  
  const char *format = "%a %b %d, %T";
  char res[64];

  time_t temptime = wtmp.ut_tv.tv_sec;

  /* Convert time to readable format */
  if (strftime(res, sizeof(res), format, localtime(&temptime)) == 0) {
    fprintf(stderr, "cannot formatdate/time into buffer");
  }

  /* Calculates number current user instances using utmp file */

  fp = fopen("/var/run/utmp", "rb");
  if (fp == NULL) {fprintf( stderr, "Failed to open utmp\n");return(1);}

  /* obtain file size: */
  fseek(fp, 0, SEEK_END);
  size_t utmp_size = ftell(fp);
  rewind(fp);

  /* Sets logsize to the correct size */
  lognum = utmp_size / sizeof(struct utmp);
  struct utmp ulog;

  int logins = 0;
  

  while(lognum > 0){
    fread(&ulog, sizeof(struct utmp), 1, fp);
    lognum--;
    int i;
    for(i = 0;i < 32; i++)
    {
        if (*(uname+i) != ulog.ut_user[i]) break;

        else if ( !(*(uname+i)) || i==31) {logins++; break;}
    }
  };
  fclose(fp);

  char user_s[6] = "users\0";
  char is_are[4] = "are\0";
  if (logins == 1) {
    user_s[4] = 0;
    is_are[0] = 105;
    is_are[1] = 115;
    is_are[2] = 0;
  }
  /* Calcualates current uptime using /proc/uptime */

  fp = fopen("/proc/uptime", "r");
  if (fp == NULL) { fprintf(stderr, "Failed to open uptime\n");return (1);}
  char buff[10];
  fscanf(fp, "%9s", buff);
  fclose(fp);

  /* Convert buff to integer */
  int pow_of_ten = 100000000;
  int i = 0;
  int uptime = 0;
  while (buff[i] != 46 && i < 9) {
    /* ASCII digit & 15 is value of digit */
    uptime += pow_of_ten * (buff[i] & 15);
    i++;
    pow_of_ten /= 10;
  }
  uptime = uptime / pow_of_ten / 10;

  /* Check if day/s singular or plural */
  char dayz[5] = "days";
  if (uptime / 86400 == 1) {
    dayz[3] = 0;
  }

  printf(
      "\033[1;38;5;2m      THIS IS A \033[38;5;1mPRIVATE"
      "\033[38;5;2m SERVER. ALL CONNECTIONS ARE MONITORED AND"
      " RECORDED\n           DISCONNECT \033[38;5;1mIMMEDIATELY"
      "\033[38;5;2m IF YOU ARE NOT AN AUTHORIZED USER!\n\n"
      "\033[38;5;12m"
      "                       d88b  .d8b.  db      d888888b  .d88b.  \n"
      "                       `8P' d8' `8b 88        `88'   .8P  Y8. \n"
      "                        88  88ooo88 88         88    88    88 \n"
      "                        88  88~~~88 88         88    88    88 \n"
      "                    db. 88  88   88 88booo.   .88.   `8b  d8' \n"
      "                    Y8888P  YP   YP Y88888P Y888888P  `Y88P'\n\n"
      "\033[0;38;5;5m"
      "  Last Login....: "
      "\033[38;5;6m%s from \033[38;5;3m%s\033[38;5;5m\n"
      "  Uptime........: \033[38;5;6m%d %s %d:%02d:%02d\033[38;5;5m\n"
      "  SSH Logins....: \033[38;5;6mThere %s currently %i %s logged in\033[0m\n",
      res, wtmp.ut_host, uptime / 86400, dayz, uptime / 3600 % 24,
      uptime / 60 % 60, uptime % 60, is_are, logins, user_s);

return(0);
}
