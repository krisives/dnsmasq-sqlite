#include "dnsmasq.h"
#ifdef HAVE_SQLITE

static sqlite3 *db = NULL;
static sqlite3_stmt *db_domain_exists = NULL;
static char *db_file = NULL;

void db_init(void)
{
  if (!db_file || db)
  {
    return;
  }
  
  atexit(db_cleanup);
  printf("Opening database %s\n", db_file);
  
  if (sqlite3_open(db_file, &db))
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(1);
  }
  
  if (sqlite3_prepare(
    db,
    "select count(*) from domain where Domain=?",
    -1,
    &db_domain_exists,
    NULL
  ))
  {
    fprintf(stderr, "Can't prepare statement: %s\n", sqlite3_errmsg(db));
    exit(1);
  }
}

void db_cleanup(void)
{
  printf("cleaning up database...\n");
  
  if (db_domain_exists)
  {
    sqlite3_finalize(db_domain_exists);
    db_domain_exists = NULL;
  }
  
  if (db)
  {
    sqlite3_close(db);
    db = NULL;
  }
  
  if (db_file)
  {
    free(db_file);
    db_file = NULL;
  }
}

void db_set_file(char *path)
{
  if (db_file) {
    free(db_file);
    db_file = NULL;
  }
  
  db_file = path;
}

int db_check_allow(const char *name)
{
  db_init();
  
  if (!db)
  {
    return 1;
  }
  
  sqlite3_reset(db_domain_exists);
  int row_exists = 0;
  
  if (
    sqlite3_bind_text(
      db_domain_exists,
      1,
      name,
      -1,
      SQLITE_TRANSIENT
    )
  )
  {
    fprintf(stderr, "Can't bind text parameter: %s", sqlite3_errmsg(db));
  }
  else if (sqlite3_step(db_domain_exists) == SQLITE_ROW)
  {
    row_exists = sqlite3_column_int(db_domain_exists, 0);
  }
  
  printf("exists: %s %d\n", name, row_exists);
  return row_exists;
}

#endif