/*
 * tsh_exec.c - helper routines for tshlab exec support.
 * This file does not contain extensive documentation. For documentation
 * related to usage, see the corresponding header file at tsh_exec.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "tsh_helper.h"
#include "tsh_exec.h"
#include "csapp.h"

// keep track of all mappings ( in sorted order by extension )
static int maxEntry = 0;
static int numEntries = 0;
static ExecEntry** entries = NULL;

// return NULL if not found, else pointer to entry.  Simple linear
// search is good enough.
ExecEntry*
get_entry(const char *extension) {
  // sanity checking
  if (extension == NULL || *extension == '\0') {
    return NULL;
  }

  for (int i=0; i<numEntries; i++) {
    if (strcmp(entries[i]->extension, extension) == 0) return entries[i];
  }
  return NULL;
}

// helper function to keep entries in sorted order
static int
cmpext(const void* a, const void* b) {
  return strcmp((*(ExecEntry* const *)a)->extension, (*(ExecEntry* const *)b)->extension);
}

int 
set_exec_entry(const char *extension, int argc, char** argv)  {
  // sanity checking
  if (extension == NULL || extension[0] == '\0') {
    return MAP_ERROR_NEED_EXT_TO_SET;
  }
  if (get_entry(extension) != NULL) {
    return MAP_ERROR_ALREADY_MAPPED;
  }
  // see if we have room in our list of extensions
  if (numEntries >= maxEntry) {
    // we need more room for entries
    int newsize = maxEntry << 1;
    if (newsize < 16) newsize = 16;
    entries = realloc(entries, newsize*sizeof(ExecEntry*));
    maxEntry = newsize;
  }
  // allocate a new entry
  ExecEntry* entry = calloc(1, sizeof(ExecEntry));
  if (entry == NULL) return MAP_ERROR_EOM;
  entry->extension = strdup(extension);
  if (entry->extension == NULL) return MAP_ERROR_EOM;
  entry->argc = argc;
  entry->argv = calloc(argc, sizeof(char*));
  if (entry->argv == NULL) return MAP_ERROR_EOM;
  // and copy all arg strings
  for (int i=0; i<argc; i++) {
    entry->argv[i] = strdup(argv[i]);
    if (entry->argv[i] == NULL) return MAP_ERROR_EOM;
  }
  // now add to list and sort entries
  entries[numEntries] = entry;
  numEntries++;
  qsort(entries, numEntries, sizeof(ExecEntry*), cmpext);
  // all done with no errors
  return 0;
}

int
destroy_entry(struct exec_entry* entry) {
  if (entry == NULL) return MAP_ERROR_NEED_ENTRY;

  for (int i=0; i<numEntries; i++) {
    if (entries[i] == entry) {
      // we found entry to remove
      entries[i] = entries[numEntries-1];
      numEntries--;
      qsort(entries, numEntries, sizeof(ExecEntry*), cmpext);
      // now free up all unneeded memory
      free(entry->extension);
      for (int j=0; j<entry->argc; j++) {
	free(entry->argv[j]);
      }
      free(entry->argv);
      return 0;
    }
  }
  return MAP_ERROR_NO_ENTRY_TO_DEL;
}

void
show_maps(int fh) {
  for (int i=0; i<numEntries; i++) {
    ExecEntry* e = entries[i];
    Sio_fprintf(fh, "%s:", e->extension);
    for (int j=0; j<e->argc; j++) {
      Sio_fprintf(fh, " %s", e->argv[j]);
    }
    Sio_fprintf(fh, "\n");
  }
}


void
maperror(int fh, int code, char* ext) {
  if (ext) {
    Sio_fprintf(fh, "%s: ", ext);
  }
  switch(code) {
  case MAP_ERROR_EXPECT_EXACTLY_1:
    Sio_fprintf(fh, "This command expects exactly one argument\n");
    break;
  case MAP_ERROR_NO_EXTENSION:
    Sio_fprintf(fh, "No extension\n");
    break;
  case MAP_ERROR_EXTENSION_NOT_FOUND:
    Sio_fprintf(fh, "Extension not found\n");
    break;
  case MAP_ERROR_NEEDS_0_OR_2:
    Sio_fprintf(fh, "This command needs zero or at least 2 arguments\n");
    break;
  case MAP_ERROR_ALREADY_MAPPED:
    Sio_fprintf(fh, "Extension already mapped.\n");
    break;
  case MAP_ERROR_UNMAP_NEEDS_EXT:
    Sio_fprintf(fh, "Unmap needs an extension\n");
    break;
  case MAP_ERROR_UNMAP_NO_EXT:
    Sio_fprintf(fh, "No existing mapping for extension.\n");
    break;
  case MAP_ERROR_NEED_EXT_TO_SET:
    Sio_fprintf(fh, "set_exec_entry needs an extension\n");
    break;
  case MAP_ERROR_EOM:
    Sio_fprintf(fh, "Not enough memory\n");
    break;
  case MAP_ERROR_NEED_ENTRY:
    Sio_fprintf(fh, "destroy_entry needs an entry\n");
    break;
  case MAP_ERROR_NO_ENTRY_TO_DEL:
    Sio_fprintf(fh, "destroy_entry did not find the entry to delete\n");
    break;
  default:
    Sio_fprintf(fh, "Unknown Error Code %d\n", code);
  }
}
