#ifndef FCT_WATCHER_H
#define FCT_WATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <fstream>
#include <unistd.h>


void watcher_all_directory(std::string path){
  std::string event_str;
  //---------------------------

  int EVENT_SIZE = sizeof (struct inotify_event);
  int BUF_LEN = 1024 * ( EVENT_SIZE + 16 );

  int fd = inotify_init();
  int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);

  char buffer[BUF_LEN];
  int length = read( fd, buffer, BUF_LEN );

  struct inotify_event *event = ( struct inotify_event * ) &buffer[0];

  int i = 0;
  while ( i < length ) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) {
          event_str = "The directory " + (string)event->name + " was created.";
          printf( "The directory %s was created.\n", event->name );
        }
        else {
          event_str = "The file " + (string)event->name + " was created.";
          printf( "The file %s was created.\n", event->name );
        }
      }
      else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          event_str = "The directory " + (string)event->name + " was deleted.";
          printf( "The directory %s was deleted.\n", event->name );
        }
        else {
          event_str = "The file " + (string)event->name + " was deleted.";
          printf( "The file %s was deleted.\n", event->name );
        }
      }
      else if ( event->mask & IN_MODIFY ) {
        if ( event->mask & IN_ISDIR ) {
          event_str = "The directory " + (string)event->name + " was modified.";
          printf( "The directory %s was modified.\n", event->name );
        }
        else {
          event_str = "The file " + (string)event->name + " was modified.";
          printf( "The file %s was modified.\n", event->name );
        }
      }
    }
    i += EVENT_SIZE + event->len;
  }

  ( void ) inotify_rm_watch( fd, wd );
  ( void ) close( fd );

  //---------------------------
}
void watcher_created_file(std::string path, std::string& path_full, bool& flag){
  std::string event_str;
  //---------------------------

  int EVENT_SIZE = sizeof (struct inotify_event);
  int BUF_LEN = 1024 * ( EVENT_SIZE + 16 );

  int fd = inotify_init();
  int wd = inotify_add_watch(fd, path.c_str(), IN_CREATE);

  char buffer[BUF_LEN];
  int length = read(fd, buffer, BUF_LEN);

  struct inotify_event *event = ( struct inotify_event * ) &buffer[0];

  int i = 0;
  while(i < length){
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len && event->mask & IN_CREATE ) {
      //Terminal info
      //event_str = "The file " + (string)event->name + " was created.";
      //printf( "The file %s was created.\n", event->name );

      //Output
      path_full = path + event->name;
      flag = true;
    }
    i += EVENT_SIZE + event->len;
  }

  ( void ) inotify_rm_watch( fd, wd );
  ( void ) close( fd );

  //---------------------------
}

#endif
