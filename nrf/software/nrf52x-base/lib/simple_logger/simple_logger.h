#ifndef SIMPLE_LOGGER_H
#define SIMPLE_LOGGER_H

#include <stdint.h>
#include "chanfs/ff.h"
#include "chanfs/diskio.h"
#include "stdarg.h"

//////////////USAGE GUIDE////////////
//	//REQUIRES: -
//	//USES: -
//
//	//In initialization
//	//permissions
//	//"w" - write
//	//"a" - append (just like c files)
//	simple_logger_init(filename, permissions);
//	
//	//In main loop
//	simple_logger_update()
//
//	//to create one time header in append mode (if the file exists, this won't append)
//	simple_logger_log_header("%s,%d,%0f.2 This is a format string",...vars)
//
//	//to log data
//	simple_logger_log("%s,%d,%f",...vars);
//
//	//currently supports final strings with substituted vars
//	//of max length 256 chars
//	//To have longer strings
//	#define SIMPLE_LOGGER_BUFFER_SIZE N
////////////////////////////////////

enum {
	SIMPLE_LOGGER_SUCCESS = 0,
	SIMPLE_LOGGER_BUSY,
	SIMPLE_LOGGER_BAD_FPOINTER,
	SIMPLE_LOGGER_BAD_FPOINTER_INIT,
	SIMPLE_LOGGER_BAD_CARD,
	SIMPLE_LOGGER_BAD_CARD_INIT,
	SIMPLE_LOGGER_FILE_EXISTS,
	SIMPLE_LOGGER_FILE_ERROR,
	SIMPLE_LOGGER_ALREADY_INITIALIZED,
	SIMPLE_LOGGER_BAD_PERMISSIONS
} SIMPLE_LOGGER_ERROR; 


enum {
	READ_FILE_OK = 0,
	READ_FILE_EOF,
	READ_FILE_ERROR
}   READ_FILE_STATE;
uint8_t simple_logger_init(const char *filename, const char *permissions);
uint8_t simple_logger_ready(void);
void simple_logger_update();
uint8_t simple_logger_power_on();
uint8_t simple_logger_log(const char *format, ...)
		__attribute__ ((format (printf, 1, 2)));
uint8_t simple_logger_log_header(const char *format, ...)
		__attribute__ ((format (printf, 1, 2)));
uint8_t simple_logger_read(uint8_t* buf, uint8_t buf_len);


// Sheng-Jung added

uint8_t openfile(const char* filename, const char *permissions);
uint32_t getfilesize();
uint8_t readfile(uint8_t* buf, uint8_t buf_len);
uint8_t readline(uint8_t* buf, uint8_t buf_len);
uint8_t moveptr_head();
uint8_t closefile();

uint8_t mount_sd();
uint8_t openfile_complete(FIL*, const char* filename, const char *permissions);
uint8_t moveptr_head_complete(FIL*);
uint8_t closefile_complete(FIL*);
uint8_t readfile_complete(FIL*, uint8_t* buf, uint8_t buf_len);
#endif
