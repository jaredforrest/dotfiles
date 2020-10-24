#include <libnotify/notify.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

/* This program outputs the current time as a notification in format
 * ## HH:MM ##
 * The time is currently HH:MM */

int main() {
    /* Get time info */
    time_t rawtime;
    struct tm * timeinfo;
    char notifybody[26] = "The current time is ";

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    /* 4 bytes for HH:MM + 1 for \0 */ 
    char currtime[6];
    strftime (currtime, 6, "%R", timeinfo);

    strcat (notifybody, currtime);
    
    /* Get local home environment variable 
    char * datadir = getenv("XDG_DATA_HOME");
    if (datadir == NULL) datadir = "/home/jalio";
    char clockfile[strlen(datadir) + 23];
    strcpy (clockfile, datadir);
    strcat (clockfile, "/icons/jalio/clock.svg");*/
    char * clockfile = "/home/jalio/.local/share/icons/jalio/clock.svg";

	notify_init ("Hello world!");
	NotifyNotification * notifytime = notify_notification_new (currtime, notifybody, clockfile);
	notify_notification_show (notifytime, NULL);
	g_object_unref(G_OBJECT(notifytime));
	notify_uninit();
	return 0;
}
