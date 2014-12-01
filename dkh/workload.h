/**
 * =====================================================================================
 *
 *          @file:  lru.h
 *         @brief:  LRU에 대한 라이브러리
 *
 *        Version:  1.0
 *          @date:  2014년 11월 23일 19시 16분 39초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        @author:  Park jun hyung (), google@dankook.ac.kr
 *       @COMPANY:  Dankopok univ.
 * =====================================================================================
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* READ WRITE FLAGS */
#define READ  1
#define WRITE 2

/* SIZE */
#define KB (1024)
#define MB (KB * KB)
#define GB (MB * KB)

#define DEBUG_OPTION 0

struct workload
{/*{{{*/
  char *time;
  char *host;
  int disk_num;
  int type;
  double offset;
  double size;
  long respone; //respones time

};/*}}}*/

struct stats
{/*{{{*/
  long max;
  long min;

  char max_time[20];
  char min_time[20];
};/*}}}*/

/**
 * Init stats memory.
 * @return : stats strcut pointer
 */
struct stats *init_stats(void)
{/*{{{*/
  struct stats *st = NULL;

  if (!(st = malloc(sizeof(struct stats))))
    return NULL;

  /* Init */
  st->max = LONG_MIN;
  st->min = LONG_MAX;
  strcpy(st->max_time, "\0");
  strcpy(st->min_time, "\0");

  return st;
}/*}}}*/

/**
 * Report result. 
 * @param st : stats struct
 */
void report_st(struct stats *st, char *fn)
{/*{{{*/
  printf("========== %15s report ==========\n", fn);
  printf("    %20s %15s \n", "time", "value");
  printf("Max %20s(%15ld)\n", st->max_time, st->max);
  printf("Min %20s(%15ld)\n", st->min_time, st->min);
  printf("============================\n");
}/*}}}*/

/**
 * Del stats
 * @param st : stats struct
 * @retustatsel_st(struct stats *st)
 */
int del_st(struct stats *st)
{/*{{{*/
  int i = 0;

  struct list_head *tmp = NULL;

  if (!st)
    return -1;

  if (!tmp)
    return -3;

  free(st);
  return 0;
}/*}}}*/

/**
 * read workload size
 * @param st : stats memory info strcut
 * @param wl : target workload struct
 * @return : error code
 */
int read_size(struct stats *st, struct workload *wl)
{/*{{{*/
  double start = 0;
  double end = 0;

  /* NULL arg */
  if (!st || !wl) {
    printf("[FAIL] arg NULL, %s \n", __func__);
    return -1;
  }

  /* start sector number and size(end) */
  start = floor(wl->offset);
  end = floor(wl->offset + wl->size);

  if (st->max < end) {
    st->max = end;
    memset(st->max_time, '\0', 20);
    strcpy(st->max_time, wl->time);
  } else if (st->min > start) {
    st->min = start;
    memset(st->min_time, '\0', 20);
    strcpy(st->min_time, wl->time);
  }

  return 0;
}/*}}}*/

/**
 * Just open file and return.
 * @param file : target file path
 * @return : file pointer
 */
FILE *open_workload(char *file)
{/*{{{*/
  FILE *fp = NULL;

  /* NULL arg */
  if (!file) {
    printf("[FAIL] arg NULL, %s \n", __func__);
    return NULL;
  }

  /* open FILE */
  if ((fp = fopen(file, "r")) < 0)
    return NULL;

  return fp;
}/*}}}*/

/**
 * read colum. (=Inscribe workload struct)
 * @param wl : workload struct
 * @param buf : buffer string
 * @return : error code
 */
int read_column(struct workload *wl, char *buf)
{/*{{{*/
  int column = 1;
  char *tmp = NULL;

  /* NULL arg */
  if (!wl || !buf) {
    printf("[FAIL] arg NULL, %s \n", __func__);
    return -1;
  }
  
  /* read file and, save workload struct */
  tmp = strtok(buf, ",");
  while (tmp != NULL) {

    switch (column) {
      /* 
       * TODO : strdup is cause of memory leak
       * But, This case is needless..
       */

      case 1 : wl->time = strdup(tmp); break;
      // case 2 : wl->host = strdup(tmp); break;
      // case 3 : wl->disk_num = atoi(tmp); break;
      case 4 : (wl->type) = (strstr(tmp, "Read") == 0) ? READ : WRITE; break;
      case 5 : wl->offset = atol(tmp); break;
      case 6 : wl->size = atol(tmp); break;
      // case 7 : wl->respone = atol(tmp); break;
    }

    /* Next */
    tmp = strtok(NULL, ",");
    column++;
  }

  free(tmp);
  return 0;
}/*}}}*/

/**
 * read worklosd and analysis..
 * @param fp : file pointer
 * @return : error code
 */
int read_workload(FILE *fp, char *file_name)
{/*{{{*/
  int ret = 0;
  char buf[100];
  struct stats *st = NULL;
  struct workload *wl = NULL;

  /* NULL arg test */
  if (!fp)
    printf("arg is NULL\n");

  wl = malloc(sizeof(struct workload));
  st = init_stats();
  if (!wl || !st)
    goto end;

  /* read line by line */
  while (!feof(fp)){

    /* read line by line. saved buf */
    if (fscanf(fp, "%s", buf) < 0)
      goto end;

    /* read buf and Inscribe wl */
    if (read_column(wl, buf) < 0)
      goto end;

    /* read workload size. MAX, MIN */
    read_size(st, wl);
  }

end:
  /* reprot stats */
  report_st(st, file_name);

  /* DEBUG.. PRINT LIST */
  if (ret < 0)
    printf("Err\n");

  del_st(st);
  printf("END\n\n");

  return 0;
}/*}}}*/

