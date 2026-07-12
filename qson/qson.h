//
// Qson is a json serializer/deserializer based on RFC 8259
//

#ifndef _qson_qson_h_
#define _qson_qson_h_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QSON_BEGIN_ARRAY	'['
#define QSON_END_ARRAY		']'
#define QSON_BEGIN_OBJECT	'{'
#define QSON_END_OBJECT		'}'
#define QSON_NAME_SEPARATOR	':'
#define QSON_VALUE_SEPARATOR	','
#define QSON_QUOTATION_MARK	'"'
#define QSON_STRING_ESCAPE_CHAR	'\\'
#define QSON_BOOL_TRUE		"true"
#define QSON_BOOL_FALSE		"false"
#define QSON_NULL		"null"
#define QSON_WHITESPACES	"\n\r\t "
#define QSON_COMMENT_PREFIX	'/'
#define QSON_COMMENT_LINE	'/'
#define QSON_COMMENT_AREA	'*'

typedef enum {
	QSON_RESULT_OK = 0,			// successfull
	QSON_RESULT_UNEXPECTED_EOF = 1,		// buffer ended where it shouldnt
	QSON_RESULT_INVALID_CHAR = 2,		// a char is where it shouldnt be
	QSON_RESULT_INVALID_STATE = 3,		// current state of context is invalid for this function
	QSON_RESULT_BUFFER_TOO_SMALL = 4,	// value dont fit in given buffer
	QSON_RESULT_INVALID_TYPE = 5,		// invalid qson type is given
	QSON_RESULT_INVALID_CONTEXT = 6,	// invalid context is given, returned when ctx has invalid flags or ...
} qson_result_t;

typedef enum {
	QSON_TYPE_AUTO = 0,		// Type should be auto detected
	QSON_TYPE_STRING = 1,		// Type is a string
	QSON_TYPE_NUMBER = 2,		// Type is a number (double)
	QSON_TYPE_NULL = 3,		// Type is null
	QSON_TYPE_BOOL = 4,		// Type is a boolean
	QSON_TYPE_OBJECT = 5,		// Type is an object
	QSON_TYPE_ARRAY = 6,		// Type is an array
} qson_type;

#ifdef __cplusplus
}
#endif

#include "deserialize/deserialize.h"
#include "serialize/serialize.h"

#endif
